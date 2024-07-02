package Model;
/**
 * @author Gustaf Söderlund - et14gsd
 * @since 2023/01/31
 * @version 1.00
 */
import java.util.ArrayList;

/**
 * This class will hold information about a channel
 */
public class ChannelInfo {

    private String channelName;
    private ArrayList<ProgramInfo> pi;

    /**
     * Creates a channel.
     * @param channelName
     */
    public ChannelInfo(String channelName){
        this.channelName = channelName;
    }

    /**
     * get the name of the channel
      * @return name of the channel
     */
    public String getChannelName(){
        return channelName;
    }

    /**
     * adds a program to the channel
     * @param program
     */
    public void addProgram(ArrayList<ProgramInfo> program){
        pi = program;
    }

    /**
     * get the size of a channel. in other words the number of programs in a channel.
     * @return the number of programs in a channel
     */
    public int getSizeOfChannel(){
        return pi.size();
    }

    /**
     * Get a program from the channel with index i
     * @param index index
     * @return a specific program
     */
    public ProgramInfo getProgram(int index) {
        return pi.get(index);
    }



}