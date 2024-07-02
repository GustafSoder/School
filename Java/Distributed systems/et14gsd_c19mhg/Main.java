import ki.types.ds.StreamInfo;
import se.umu.cs._5dv186.a1.client.DefaultStreamServiceClient;
import se.umu.cs._5dv186.a1.client.FrameAccessor;
import se.umu.cs._5dv186.a1.client.StreamServiceClient;
import se.umu.cs._5dv186.a1.client.StreamServiceDiscovery;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Arrays;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Main {
    public static final int DEFAULT_TIMEOUT = 1000;
    static File file = new File("output.txt");
    static FileWriter fr; // parameter 'true' is for append mode

    static {
        try {
            fr = new FileWriter(file, true);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public Main() throws IOException {
    }

    public static void main(String[] args) throws IOException, InterruptedException {

        final String host = (args.length > 0) ? args[0] : "itchy.cs.umu.se";
        final int timeout = (args.length > 1) ? Integer.parseInt(args[1]) : DEFAULT_TIMEOUT;
        final String username = (args.length > 2) ? args[2] : "test";
        Factory factory = new Factory();

        writeToFile("Sequential Client Beginning");
        StreamServiceClient[] clients = new StreamServiceClient[4];
        String[] hosts = StreamServiceDiscovery.SINGLETON.findHosts();
        for (int i = 0; i < clients.length; i++) {
            clients[i] = DefaultStreamServiceClient.bind(hosts[i], 750, username);
        }

        StreamInfo[] streams = clients[0].listStreams();
        for (int i = 0; i < 8; i++) {
            runSequentialClient(factory,streams,clients[0]);
        }
        for (int i = 0; i < 8; i++) {
            runSequentialClient(factory,streams,clients[1]);
        }
        for (int i = 0; i < 8; i++) {
            runSequentialClient(factory,streams,clients[2]);
        }
        for (int i = 0; i < 8; i++) {
            runSequentialClient(factory,streams,clients[3]);
        }
        writeToFile("Sequential Client End");



        writeToFile("Parallel Client Beginning");
        for (int i = 0; i < 8; i++) {
            runParallelclient(factory, streams, clients);
        }
        for (int i = 0; i < 8; i++) {
            runParallelclient(factory, streams, Arrays.copyOfRange(clients, 0, 3));
        }
        for (int i = 0; i < 8; i++) {
            runParallelclient(factory, streams, Arrays.copyOfRange(clients, 0, 2));
        }

        writeToFile("Parallel Client End");
        fr.close();
        System.exit(0);
    }

    /**
     * Method to run the sequentialclient
     * @param factory
     * @param streams
     * @param client
     * @throws IOException
     */
    private static void runSequentialClient(Factory factory, StreamInfo[] streams, StreamServiceClient client) throws IOException {
        for (StreamInfo stream : streams) {
            FrameAccessor fa = factory.getFrameAccessor(client, stream.getName());
            if (stream.getName().equals("stream10")) { // Temporary for testing

                for (int j = 0; j < fa.getStreamInfo().getLengthInFrames(); j++) {
                    fa.getFrame(j);
                }

                writeToFile(fa.getPerformanceStatistics().getPacketDropRate(client.getHost())*100 + " % Host: " + client.getHost());
                writeToFile(fa.getPerformanceStatistics().getPacketLatency(client.getHost()) + " ms Host: " + client.getHost());
                writeToFile(fa.getPerformanceStatistics().getFrameThroughput() + " fps");
                writeToFile(fa.getPerformanceStatistics().getBandwidthUtilization() + " bps");
                //Print the performance
                //                System.out.println(fa.getPerformanceStatistics().getPacketDropRate(client.getHost())*100 + "% Packet drop rate on host: " + client.getHost());
                //                System.out.println(fa.getPerformanceStatistics().getPacketLatency(client.getHost()) + " ms latecny on host: " + client.getHost());
                //                System.out.println(fa.getPerformanceStatistics().getFrameThroughput() + " fps");
                //                System.out.println(fa.getPerformanceStatistics().getBandwidthUtilization() + " bps");
            }
        }
    }

    /**
     * Method to run parallelclient
     * @param factory
     * @param streams
     * @param clients
     */
    private static void runParallelclient(Factory factory, StreamInfo[] streams, StreamServiceClient[] clients) throws IOException, InterruptedException {
        FrameAccessor fa = factory.getFrameAccessor(clients, "stream10");
        for (StreamInfo stream: streams) {
            if (stream.getName().equals("stream10")) {
                for (int i = 0; i < stream.getLengthInFrames(); i++) {
                    FrameAccessor.Frame frame = fa.getFrame(i);
                }
                for (StreamServiceClient client: clients) {
                    writeToFile(fa.getPerformanceStatistics().getPacketDropRate(client.getHost())*100 + " % Host: " + client.getHost() + " NumHosts: " + clients.length);
                    writeToFile(fa.getPerformanceStatistics().getPacketLatency(client.getHost()) + " ms Host: " + client.getHost() + " NumHosts: " + clients.length);
//                    System.out.println(fa.getPerformanceStatistics().getPacketDropRate(client.getHost())*100 + "% Packet drop rate on host: " + client.getHost());
//                    System.out.println(fa.getPerformanceStatistics().getPacketLatency(client.getHost()) + " ms latecny on host: " + client.getHost());
                }
                writeToFile(fa.getPerformanceStatistics().getFrameThroughput() + " fps" + " NumHosts: " + clients.length);
                writeToFile(fa.getPerformanceStatistics().getBandwidthUtilization() + " bps" + " NumHosts: " + clients.length);
//                System.out.println(fa.getPerformanceStatistics().getFrameThroughput() + " fps");
//                System.out.println(fa.getPerformanceStatistics().getBandwidthUtilization() + " bps");
            }
        }
    }

    private static void writeToFile(String s) throws IOException {
        fr.write(s + "\n");
    }
}
