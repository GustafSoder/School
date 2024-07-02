import ki.types.ds.Block;
import ki.types.ds.StreamInfo;
import se.umu.cs._5dv186.a1.client.FrameAccessor;
import se.umu.cs._5dv186.a1.client.StreamServiceClient;

import java.io.IOException;
import java.net.Socket;
import java.net.SocketTimeoutException;
import java.util.LinkedList;
import java.util.Queue;
import java.util.Stack;
import java.util.Timer;
import java.util.concurrent.Semaphore;
import java.util.concurrent.locks.ReentrantLock;

public class Factory implements FrameAccessor.Factory {
    public Factory() {
    }

    @Override
    public FrameAccessor getFrameAccessor(StreamServiceClient client, String stream) {
        try {
            return new SequentialClient(client, stream);
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }
    @Override
    public FrameAccessor getFrameAccessor(StreamServiceClient[] clients, String stream) {
        return new ParallelClient(clients, stream);
    }
}
