package Model;
/**
 * @author Gustaf Söderlund - et14gsd
 * @since 2023/01/31
 * @version 1.00
 */
import java.time.LocalDateTime;

/**
 * This class will have the information about a program
 */
public class ProgramInfo {
    private String programName;
    private String tagline;
    private LocalDateTime startTime;
    private LocalDateTime endTime;
    private String image;

    /**
     * create a program-objective
     * @param programName the name of the program
     * @param tagline the description of a program
     * @param startTime start time of the program
     * @param endTime end time of the program
     * @param image image realated to the program.
     */
    public ProgramInfo(String programName, String tagline, LocalDateTime startTime, LocalDateTime endTime, String image ) {
        this.programName = programName;
        this.tagline = tagline;
        this.startTime = startTime;
        this.endTime = endTime;
        this.image = image;
    }

    /**
     * get the tagline of a program
     * @return tagline
     */
    public String getTagline(){
        return tagline;
    }

    /**
     * get the name of the program
     * @return programName
     */
    public String getProgramName(){
        return programName;
    }

    /**
     * get the image related to the program
     * @return image
     */
    public String getImage(){
        return image;
    }

    /**
     * get the start time of a program
     * @return startTime
     */
    public LocalDateTime getStartTime(){
        return startTime;
    }

    /**
     * get the end time of a program
      * @return endTime
     */
    public LocalDateTime getEndTime(){
        return endTime;
    }

}


