package Model;
/**
 * @author Gustaf Söderlund - et14gsd
 * @since 2023/01/31
 * @version 1.00
 */
import java.util.ArrayList;

/**
 * This class has the responsibility to do the "work" in the model.
 */
public class Model {
    private ArrayList<ChannelInfo> ci;

    /**
     * The constructor will parse the channels from SR.
     */
    public Model(){
        ChannelParsing cp = new ChannelParsing();
        ci = cp.parseChannel();
    }

    /**
     * This method will update all the channels with new data.
     */
    public synchronized void parser(){
        ChannelParsing cp = new ChannelParsing();
        ci = cp.parseChannel();
    }

    /**
     * This method is used to get all the names of a channel and store them in
     * an string array
     * @return array of names of the channels.
     */
    public synchronized ArrayList<String> getChannels(){
        ArrayList<String> names = new ArrayList<>();
        for (ChannelInfo i : ci){
            names.add(i.getChannelName());
        }
        return names;
    }

    /**
     * Method used to get all the programs of a channel.
     * @param channel
     * @return programs of a channel in a list
     */
    public synchronized ArrayList<ProgramInfo> getPrograms(String channel){
        ArrayList<ProgramInfo> pi = new ArrayList<>();
        for(ChannelInfo i : ci){
            if(i.getChannelName().equals(channel)){
                int channelSize = i.getSizeOfChannel();
                for (int j = 0; j < channelSize; j++) {
                    pi.add(i.getProgram(j));
                }
            }
        }
        return pi;
    }

    /**
     * Method used to get the information from a selected program.
     * @param program
     * @param channel
     * @return the programinfo
     */
    public synchronized ProgramInfo getProgramInfo(String program, String channel){
        ProgramInfo p=null;
        for(ChannelInfo i : ci){
            if(i.getChannelName().equals(channel)){
                for(int j = 0; j < i.getSizeOfChannel(); j++){
                    if(i.getProgram(j).getProgramName().equals(program)){
                        p = i.getProgram(j);
                        break;
                    }
                }
            }
        }
        return p;
    }
}
