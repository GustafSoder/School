/**
 * @author Gustaf Söderlund - et14gsd
 * @since 2023/01/31
 * @version 1.00
 */

import javax.imageio.ImageIO;
import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.*;
import java.io.IOException;
import java.net.MalformedURLException;
import java.net.URL;

/**
 * The public View class extends JFrame and has the purpose to constructs the graphical user interface.
 * The class constructs panels with diffrent components stored inside of them. The panels are placed in the frame.
 * This class represents the View in the MVC-model.
 */
public class View extends JFrame {

    private JPanel panelNorth;
    private JMenuBar menuBar;
    DefaultTableModel model;
    private JTable table;
    private JScrollPane jps;
    private JMenu jMenu1;
    public JPanel centerPanel;
    private JMenuItem update;
    private JMenuItem quit;
    private JComboBox comboBox;
    private JLabel channelLabel;
    private JOptionPane channelJOP;
    private ListSelectionModel lsm;
    private URL image;

    /**
     * View()  - The constructor add the components to the JFrame.
     */
    public View(){
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setTitle("RadioInfo");
        this.pack();
        this.setSize(1200,1000);
        this.setMinimumSize(new Dimension(400,400));
        this.setLocationRelativeTo(null);

        this.add(buildNorthPanel(), BorderLayout.NORTH);
        this.add(buildCenterPanel(), BorderLayout.CENTER);

        this.setJMenuBar(menuBar());

    }

    /**
     * This method is used to make the frame visible
     */
    public void showFrame(){
        this.setVisible(true);
    }

    /**
     * menuBar() - Build a JMenuBar that will be placed in the northborder of the JFrame.
     * The JMenubar contains one JMenu "JMenu1".
     * The JMenu contain four JMenuItems, "Update" and "Quit"
     * @return menuBar
     */
    public JMenuBar menuBar(){

        menuBar = new JMenuBar();
        menuBar.setBackground(new Color(0,0,0,0));
        jMenu1 = new JMenu("Menu");
        update = new JMenuItem("Update");
        quit = new JMenuItem("Quit");
        update.setEnabled(true);

        menuBar.add(jMenu1);
        jMenu1.add(update);
        jMenu1.add(quit);
        return menuBar;
    }

    /**
     * getQuit() - A public method used to get the JMenuItem Quit.
     *
     * @return this.update
     */
    public JMenuItem getQuit(){
        return this.quit;
    }

    /**
     * getUpdate() - A public method used to get the JMenuItem update.
     *
     * @return this.update
     */
    public JMenuItem getUpdate(){
        return this.update;
    }


    /**
     * buildCenterPanel() - Builds the Jpanel that will be placed in the center border of the frame.
     * In this Jpanel a Table is added.
     *
     * @return centerPanel
     */
    public JPanel buildCenterPanel(){
        centerPanel = new JPanel();
        centerPanel.setLayout(new BoxLayout(centerPanel, BoxLayout.X_AXIS));
        centerPanel.setBackground(Color.WHITE);
        centerPanel.setPreferredSize(new Dimension(600,1000));
        centerPanel.add(Box.createHorizontalGlue());
        centerPanel.add(buildTable());
        centerPanel.add(Box.createHorizontalGlue());

    return centerPanel;
    }

    /**
     * buildTable() - Builds the DefaultTableModel and creates a JTable component. The Jtable
     * has three columns. Program, start and end. The Jtable is used when creating the JScrollPane,
     * It will make the user be able to scroll.
     *
     * @return jps
     */

    public JScrollPane buildTable(){
            model = new DefaultTableModel(){
                @Override
                public boolean isCellEditable(int row, int column) {
                    return false;
                }
            };

            table = new JTable(model);
            //collumns
            model.addColumn("Program");
            model.addColumn("start");
            model.addColumn("end");
            //Make it able to scroll
            jps = new JScrollPane(table);
            //make it able to click in the table
            table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
            lsm= table.getSelectionModel();

            return jps;
    }

    /**
     * getModel() - This method is used to get the DefaultTableModel model.
     * @return model.
     */
    public DefaultTableModel getModel(){
        return model;
    }

    /**
     * getTable() - this method is used to get the JTable table.
     * @return table.
     */
    public JTable getTable(){
        return table;
    }

    /**
     * getLsm() - this method is used to get the ListSelectionModel lsm.
     * @return lsm
     */
    public ListSelectionModel getLsm(){
        return lsm;
    }


    /**
     * BuildNorthPanel() - This method is used to construct a JPanel in the northern border of the JFrame.
     * The panel uses The layoutmanager BoxLayout.
     * In the panel the components JCombobox and JLabel are inserted.
     * @return panelNorth.
     */
    public JPanel buildNorthPanel(){
        panelNorth = new JPanel();
        panelNorth.setLayout(new BoxLayout(panelNorth, BoxLayout.X_AXIS));
        panelNorth.setBackground(Color.WHITE);
        panelNorth.setPreferredSize(new Dimension(50,100));

        panelNorth.add(Box.createHorizontalGlue());

        panelNorth.add(setChannelLabel());

        panelNorth.add(buildComboBox());

        panelNorth.add(Box.createHorizontalGlue());
        return panelNorth;
    }

    /**
     * setChannelLabel() - This method creates the JLAbel that will be placed in the northern jpanel.
     *
     * @return channelLabel
     */
    public JLabel setChannelLabel(){
        channelLabel = new JLabel();
        channelLabel.setText("Select radio station: ");
        return channelLabel;
    }

    /**
     * buildComboBox() - This method Creates a JCombobox that will be placed in the northern Jpanel.
     * The Combobox purpose is to store the name of the radiostations.
     *
     * @return comboBox
     */
    public JComboBox buildComboBox(){
        comboBox = new JComboBox();
        comboBox.setMaximumSize(new Dimension(250,20));
        return comboBox;
    }

    /**
     * getComboBox() - This method is used to get the JComboBox combobox.
     * @return comboBox
     */
    public JComboBox getComboBox(){
        return comboBox;
    }


    /**
     * programInformation() - Creates an JOptionPane to show the user a picture and some information about the
     * chosen program.
     * @return channelJOP
     */
    public JOptionPane programInformation(String tagLine, String name, String imgURL){
        channelJOP = new JOptionPane();
        if (!imgURL.equals("No Image")) {
            try {
                image = new URL(imgURL);

            } catch (MalformedURLException e) {
                e.printStackTrace();
            }

            Image i = getRadioImage(image);
            ImageIcon icon = new ImageIcon(i.getScaledInstance(250, 250, Image.SCALE_SMOOTH));

            JOptionPane.showMessageDialog(null,
                    tagLine,
                    name, JOptionPane.INFORMATION_MESSAGE, icon);
        }else{
            JOptionPane.showMessageDialog(null,
                    tagLine,
                    name,JOptionPane.INFORMATION_MESSAGE);
        }

        return channelJOP;
    }

    /**
     * Convert the url to image
     * @param img
     * @return
     */
    public Image getRadioImage(URL img)  {
        try{
            Image pic = ImageIO.read(img);
            return pic;
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }
}