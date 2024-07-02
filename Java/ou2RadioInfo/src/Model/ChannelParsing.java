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
import java.time.LocalDate;
import java.time.LocalTime;
import java.util.ArrayList;

/**
 * this class will parse all the channels and store the channels in an array.
 */
public class ChannelParsing {
    private String xmlFile;

    /**
     *  Constructor initialize the url to the XML
     */
    public ChannelParsing() {
        xmlFile = "http://api.sr.se/api/v2/channels/?pagination=false";
    }

    /**
     * This method is used to parse a channel
     * @return a list of all the channels.
     */
    public ArrayList<ChannelInfo> parseChannel(){
        ArrayList<ChannelInfo> channelInfos = new ArrayList<>();
        DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
        int programId = 0;
        try {
            DocumentBuilder db = factory.newDocumentBuilder();
            Document document = db.parse(xmlFile);
            Element data = document.getDocumentElement();
            data.normalize();
            NodeList nList = document.getElementsByTagName("channel");
            for (int i = 0; i < nList.getLength(); i++) {
                Node node = nList.item(i);
                if (node.getNodeType() == Node.ELEMENT_NODE) {
                    Element e = (Element) node;
                    String name = e.getAttribute("name");
                    programId = Integer.parseInt(e.getAttribute("id"));
                    ChannelInfo ci = new ChannelInfo(name);
                    ci.addProgram(checkPrograms(programId)); // get the valid programs
                    channelInfos.add(ci);
                }
            }
        }catch(ParserConfigurationException | SAXException | IOException e){
            System.out.println("problem parsing channel");
        }
        return channelInfos;
    }

    /**
     * Get valid programs with programid @id
     * @param programId of the channel
     */
    private ArrayList<ProgramInfo> checkPrograms(int programId) {
        LocalDate currentDate = java.time.LocalDate.now();
        LocalTime currentTime = LocalTime.now();
        ProgramParsing pp = new ProgramParsing();
        ArrayList<ProgramInfo> today = pp.parseProgram(programId,currentDate.toString());
        ArrayList<ProgramInfo> programs = new ArrayList<>(today);
        //programs tomorrow
        if (currentTime.getHour() >= 12) {
            String date = currentDate.plusDays(1).toString();
            ArrayList<ProgramInfo> tomorrow = pp.parseProgram(programId,date);
            programs.addAll(tomorrow);

        }
        //programs yesterday
        else {
            String date = currentDate.minusDays(1).toString();
            ArrayList<ProgramInfo> yesterday = pp.parseProgram(programId,date);
            programs.addAll(yesterday);
        }
        return programs;
    }
}