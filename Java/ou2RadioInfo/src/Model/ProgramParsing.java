package Model;
/**
 * @author Gustaf Söderlund - et14gsd
 * @since 2023/01/31
 * @version 1.00
 */
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import java.io.IOException;
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.ZonedDateTime;
import java.util.ArrayList;

/**
 * This class has the responsibility to parse the programs from a given channel.
 */
public class ProgramParsing {
    private String paginationFalse;
    private String xmlFile;

    /**
     * Initialize the url
     */
    public ProgramParsing(){
        xmlFile = "http://api.sr.se/api/v2/scheduledepisodes?channelid=";
        paginationFalse = "&pagination=false&date=";
    }

    /**
     * This method parses the program.
     *  @param cID id for the channel
     * @param date date
     * @return a list containing all the programs.
     */
    public ArrayList<ProgramInfo> parseProgram(int cID, String date){
        ArrayList<ProgramInfo> programs = new ArrayList<>();
        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
        try{
            DocumentBuilder db = factory.newDocumentBuilder();
            Document document = db.parse(xmlFile + cID + paginationFalse + date);
            Element data = document.getDocumentElement();
            data.normalize();
            NodeList nList = document.getElementsByTagName("scheduledepisode");
            for (int i = 0; i < nList.getLength(); i++) {
                Node node = nList.item(i);
                if (node.getNodeType() == Node.ELEMENT_NODE) {
                    Element ele = (Element) node;
                    String name = getName(ele);
                    //System.out.println(name);
                    String tagline = getTagline(ele);
                    LocalDateTime startTime = getStartTime(ele);
                    if(startTime == null){
                        continue;
                    }
                    LocalDateTime endTime = getEndTime(ele);
                    String image = getImage(ele);
                    ProgramInfo pi = new ProgramInfo(name,tagline,startTime,endTime,image);
                   // System.out.println(pi.getStartTime() + pi.getProgramName());
                    programs.add(pi);
                }
            }
        }catch(ParserConfigurationException | SAXException | IOException e){
            System.out.println("Problem parsing a program");
        }
        return programs;
    }

    /**
     * Get the image from the xml, if there's no image set text to "no image".
     * @param ele element from xml
     * @return the image URL as a string
     */
    private String getImage(Element ele) {
        Node image = ele.getElementsByTagName("imageurltemplate").item(0);
        if(image == null){
            return "No Image";
        }
        return image.getTextContent();
    }

    /**
     * get the end time of a program.
     * @param ele elements of xml
     * @return LocalDateTime the time else null
     */
    private LocalDateTime getEndTime(Element ele) {
        Node end = ele.getElementsByTagName("endtimeutc").item(0);
        if(end != null){
            ZonedDateTime zdt = ZonedDateTime.parse(end.getTextContent());
            LocalDateTime ldt = LocalDateTime.ofInstant(zdt.toInstant(), ZoneId.systemDefault());
            return ldt;
        }
        return null;
    }

    /**
     * get the start time of a program, it has to be between the 12 hour interval
     * @param ele elements of xml
     * @return the time else null
     */
    private LocalDateTime getStartTime(Element ele) {
        Node start = ele.getElementsByTagName("starttimeutc").item(0);
        if (start != null){
            ZonedDateTime zdt = ZonedDateTime.parse(start.getTextContent());
            LocalDateTime ldt = LocalDateTime.ofInstant(zdt.toInstant(), ZoneId.systemDefault());
            LocalDateTime currentTime = LocalDateTime.now();
            LocalDateTime TimeMinus12 = currentTime.minusHours(12);
            LocalDateTime TimePlus12 = currentTime.plusHours(12);
            if (ldt.isBefore(TimePlus12) && ldt.isAfter(TimeMinus12)){
                return ldt;
            }
        }
        return null;
    }

    /**
     * get the name of the program. If  there exists no name, set the name to "no name Available"
     * @param ele elements of xml
     * @return the name of the program
     */
    private String getName(Element ele){
        Node name = ele.getElementsByTagName("title").item(0);
        if(name == null){
            return "No Name Available";
        }
        return name.getTextContent();
    }

    /**
     * Get the tagline of the program. If there's no description set the text to "no description"
     * @param ele elements of xml
     * @return the tagline of the program
     */
    private String getTagline(Element ele){
        Node tagline = ele.getElementsByTagName("description").item(0);
        if(tagline == null){
            return "No Description Available";
        }
        return tagline.getTextContent();
    }



}
