import ki.types.ds.Block;
import ki.types.ds.StreamInfo;
import se.umu.cs._5dv186.a1.client.FrameAccessor;
import se.umu.cs._5dv186.a1.client.StreamServiceClient;

import java.io.IOException;
import java.net.SocketTimeoutException;
import java.util.*;
import java.util.concurrent.*;


public class ParallelClient implements FrameAccessor {

    public static final int DEFAULT_TIMEOUT = 1000;
    private static final double BLOCK_SIZE_IN_BIT = 6144;
    StreamServiceClient[] clients;
    String stream;
    double timeouts;
    double totalFrames = 0;
    double totalTime;
    double bits;
    double totalBlocks = 0;
    ConcurrentHashMap<String, Double> totalBlocksMap;
    ConcurrentHashMap<String, Double> totalTimeMap;
    ConcurrentHashMap<String, Double> totalTimeouts;

    public ParallelClient(StreamServiceClient[] clients, String stream) {
        this.clients = clients;
        this.stream = stream;
        totalBlocksMap = new ConcurrentHashMap<>();
        totalTimeouts = new ConcurrentHashMap<>();
        totalTimeMap = new ConcurrentHashMap<>();
        for (StreamServiceClient client: clients) {
            totalBlocksMap.put(client.getHost(), (double) 0);
            totalTimeouts.put(client.getHost(), (double) 0);
            totalTimeMap.put(client.getHost(), (double) 0);
        }
    }

    @Override
    public StreamInfo getStreamInfo() throws IOException, SocketTimeoutException {
        StreamInfo[] streamInfos = clients[0].listStreams();
        for (StreamInfo si: streamInfos) {
            if (stream.equals(si.getName()))
                return si;
        }
        return null;
    }



    @Override
    public Frame getFrame(int numFrame) throws IOException, SocketTimeoutException {
        final ExecutorService executorService = Executors.newFixedThreadPool(clients.length);
        final int blockX = getStreamInfo().getWidthInBlocks();
        final int blockY = getStreamInfo().getHeightInBlocks();
        final int chunkSize = (int) Math.ceil((double) blockX / clients.length);
        totalFrames++;
        Frame frame = (i, i1) -> {
            Block block = null;
            long startTime = 0;
            long endTime = 0;
            boolean receivedBlock = false;
            int clientIndex = 0;
            do {
                synchronized(this) {
                    startTime = System.currentTimeMillis();
                    try {
                        clientIndex = i % clients.length;
                        block = clients[clientIndex].getBlock(stream, numFrame, i, i1);
                        endTime = System.currentTimeMillis();
                        totalTime += (endTime - startTime);
                        totalTimeMap.put(clients[clientIndex].getHost(),
                                totalTimeMap.get(clients[clientIndex].getHost()) + (endTime - startTime));
                        totalBlocksMap.put(clients[clientIndex].getHost(), totalBlocksMap.get(clients[clientIndex].getHost()) + 1);
                        totalBlocks++;
                        receivedBlock = true;
                    } catch (SocketTimeoutException e) {
                        totalTimeouts.put(clients[clientIndex].getHost(),
                                totalTimeouts.get(clients[clientIndex].getHost()) + 1);
                        timeouts++;
                    }
                }
            } while(!receivedBlock);
            return block;
        };
        List<Callable<Object>> todo = new ArrayList<Callable<Object>>(clients.length);
        for (int i = 0; i < clients.length; i++) {

            int start = i * chunkSize;
            int end = start + chunkSize;
            Runnable task = new GetBlockFragment(frame, start, end, blockY);
            todo.add(Executors.callable(task));
        }
        try {
            List<Future<Object>> answers = executorService.invokeAll(todo);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        return frame;
    }

    @Override
    public PerformanceStatistics getPerformanceStatistics() {
        return new PerformanceStatistics() {
            @Override
            public double getPacketDropRate(String s) {
                System.out.println("Total time for Host " + s + " is " + totalTimeMap.get(s) + " ms");
                return totalTimeouts.get(s) / totalBlocksMap.get(s);
            }

            @Override
            public double getPacketLatency(String s) {
                System.out.println();
                System.out.println("Total blocks for Host " + s + " is " + totalBlocksMap.get(s) + " ms");
                return totalTimeMap.get(s) / totalBlocksMap.get(s);
            }

            @Override
            public double getFrameThroughput() {
                return totalFrames / (totalTime / 1000);
            }

            @Override
            public double getBandwidthUtilization() {
                return (totalBlocks * BLOCK_SIZE_IN_BIT) / (totalTime / 1000);
            }
        };
    }

    static class GetBlockFragment implements Runnable {
        private final int minBlockX;
        private final int maxBlockX;
        private final int blockY;
        private final FrameAccessor.Frame frame;
        public GetBlockFragment(FrameAccessor.Frame frame, int minBlockX, int maxBlockX, int blockY) {
            this.frame = frame;
            this.minBlockX = minBlockX;
            this.maxBlockX = maxBlockX;
            this.blockY = blockY;
        }

        public void run() {
            for (int i = minBlockX; i < maxBlockX; i++) {
                for (int j = 0; j < blockY; j++) {
                    try {
                        frame.getBlock(i, j);
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }
    }
}
