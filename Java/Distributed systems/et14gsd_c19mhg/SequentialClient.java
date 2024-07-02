import ki.types.ds.Block;
import ki.types.ds.StreamInfo;
import se.umu.cs._5dv186.a1.client.DefaultStreamServiceClient;
import se.umu.cs._5dv186.a1.client.FrameAccessor;
import se.umu.cs._5dv186.a1.client.StreamServiceClient;

import java.io.IOException;
import java.net.SocketTimeoutException;
import java.util.HashMap;

public class SequentialClient implements FrameAccessor {
    double timeouts;
    double totalBlocks;
    double totalTime;
    double totalFrames = 0;
    double bits;
    HashMap<String, Double> packetDropRate;
    HashMap<String, Double> packetLatency;

    StreamServiceClient client;
    String stream;
    public static final int DEFAULT_TIMEOUT = 1000;

    public SequentialClient(StreamServiceClient client, String stream) throws IOException {
        this.stream = stream;
        this.client = client;
        packetDropRate = new HashMap<>();
        packetLatency = new HashMap<>();
    }

    @Override
    public StreamInfo getStreamInfo() throws IOException, SocketTimeoutException {
        StreamInfo[] streamInfos = client.listStreams();
        for (StreamInfo si: streamInfos) {
            if (stream.equals(si.getName()))
                return si;
        }
        return null;
    }

    @Override
    public Frame getFrame(int numFrame) throws IOException, SocketTimeoutException {
        totalFrames++;
        Frame frame = (blockX, blockY) -> {
            Block block = null;
            long startTime = 0;
            long endTime = 0;
            boolean receivedBlock = false;
            startTime = System.currentTimeMillis();
            do {
                try {
                    block = client.getBlock(stream, numFrame, blockX, blockY);
                    endTime = System.currentTimeMillis();
                    bits += block.getPixels().length * 8 * 3;
                    totalTime+=(endTime - startTime);
                    receivedBlock = true;
                } catch (SocketTimeoutException e) {
                    timeouts++;
                }
            } while(!receivedBlock);

            totalBlocks++;
            return block;
        };
        final int blockX = getStreamInfo().getWidthInBlocks();
        final int blockY = getStreamInfo().getHeightInBlocks();
        for (int k = 0; k < blockX; k++) {
            for (int l = 0; l < blockY; l++) {
                frame.getBlock(k, l);
            }
        }
        packetDropRate.put(client.getHost(), timeouts / totalBlocks);
        packetLatency.put(client.getHost(), totalTime / totalBlocks);
        return frame;
    }

    @Override
    public PerformanceStatistics getPerformanceStatistics() {
        return new PerformanceStatistics() {
            @Override
            public double getPacketDropRate(String s) {
                if (packetDropRate.containsKey(s)) {
                    return packetDropRate.get(s);
                }
                return -1;
            }

            @Override
            public double getPacketLatency(String s) {
                if (packetLatency.containsKey(s)) {
                    return packetLatency.get(s);
                }
                return -1;
            }

            @Override
            public double getFrameThroughput() {
                return totalFrames / (totalTime / 1000);
            }

            @Override
            public double getBandwidthUtilization() {
                System.out.println("bits: " + bits + "totalTime: " + totalTime / 1000);
                return bits / (totalTime / 1000);
            }
        };
    }
}
