/**
 * @author Gustaf Söderlund - et14gsd
 * @since 2023/01/30
 * @version 1.00
 */

import Model.Model;
import Model.ProgramInfo;
import javax.swing.*;
import java.util.ArrayList;
import java.util.Timer;
import java.util.TimerTask;
import java.util.concurrent.ExecutionException;

/**
 * This class is the controller. Where the work between View and model is done.
 */
public class Controller{

    private View view;
    private Timer timer;
    private final int ONEHOUR = 1000*60*60;
    private Model model;

    /**
     * The constructor Initializes variables.
     * @param view
     */
    public Controller(View view)  {
        this.view = view;
        model = new Model();
        timer = new Timer();
    }

    /**
     * Get the channels from SR and add them to the combobox.
     *
     */
    private void setUpChannels(){
            ArrayList<String> c = model.getChannels();

                for(String i :c) {
                    view.getComboBox().addItem(i);
                }
    }

    /**
     * This method starts the updatetimer, once each hour the already chosen channel get its data updated.
     */
    private void updateTimer(){
        timer.scheduleAtFixedRate(new TimerTask() {
            @Override
            public void run() {
                SwingUtilities.invokeLater(() -> updateProgram());
            }
        },ONEHOUR,ONEHOUR);
    }

    /**
     * This method creates the actionlistners used to interact with the buttons in the program,
     *  and adds the channels to the dropdownbox, then we show the frame and start the update timer.
     */
    public void setUp(){

        createActionListeners();
        setUpChannels();
        view.showFrame();
        updateTimer();
    }

    /**
     * This method creates the actionlisteners to the buttons in the program
     */
    private void createActionListeners(){
        view.getUpdate().addActionListener(e->updateProgram());
        view.getQuit().addActionListener(e->quitProgram());
        view.getComboBox().addActionListener(e->showPrograms());
        view.getLsm().addListSelectionListener(e->programInfo());
    }

    /**
     * This method is used when the update button is selected and the hourly task
     * is done.
     */
    private void updateProgram() {
        String selectedValue = (String) view.getComboBox().getSelectedItem();


        SwingWorker<ArrayList<ProgramInfo>,Void> swingWorker = new SwingWorker<>() {
            @Override
            protected ArrayList<ProgramInfo> doInBackground() throws Exception {
                model.parser();
                ArrayList<ProgramInfo> pi = model.getPrograms(selectedValue);
                return pi;
            }
            @Override
            protected void done(){
                try {
                    ArrayList<ProgramInfo> pi = get();
                    view.getModel().setRowCount(0); // clear existing data
                    for (ProgramInfo program : pi) {
                        Object[] row = {program.getProgramName(),
                                program.getStartTime().toString().replace('T', ' '),
                                program.getEndTime().toString().replace('T', ' ')};
                        view.getModel().addRow(row);
                    }
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } catch (ExecutionException e) {
                    e.printStackTrace();
                }
            }
        };  swingWorker.execute();
    }

    /**
     * This method is preformed when a program has been selected from the table.
     * The method gets the information and displays it to the User.
     */
    private void programInfo() {
        int selectedRow = view.getTable().getSelectedRow();
        String channel = (String) view.getComboBox().getSelectedItem();
        SwingWorker<ProgramInfo,Void> swingWorker = new SwingWorker<>() {
            @Override
            protected ProgramInfo doInBackground() throws Exception {


                if (selectedRow != -1){
                    String selectedValue = view.getTable().getValueAt(selectedRow,0).toString();
                    ProgramInfo p = model.getProgramInfo(selectedValue,channel);
                    return p;
                }
                return null;
            }
            @Override
            protected void done(){
                try {
                    ProgramInfo p = get();
                    if(p!=null) {
                        view.programInformation(p.getTagline(), p.getProgramName(), p.getImage());
                    }
                }catch (ExecutionException e) {
                    e.printStackTrace();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }; swingWorker.execute();

    }


    /**
     *  This method is used when a channel has been selected in the dropdownbox.
     *  The method gets the channelinformation and displays it to the user.
     *
     */
    private void showPrograms(){
        String selectedValue = (String) view.getComboBox().getSelectedItem();
        SwingWorker<ArrayList<ProgramInfo>,Void> swingWorker = new SwingWorker<>() {
            @Override
            protected ArrayList<ProgramInfo> doInBackground() throws Exception {

               // System.out.println(selectedValue);
                ArrayList<ProgramInfo> pi = model.getPrograms(selectedValue);
               // System.out.println("size of pi: "+ pi.size());
               return pi;
            }
            @Override
            protected void done(){
                try{
                    ArrayList<ProgramInfo> pi = get();
                    view.getModel().setRowCount(0); // clear existing data

                    for (ProgramInfo program : pi) {
                        Object[] row = {program.getProgramName(),
                                program.getStartTime().toString().replace('T', ' '),
                                program.getEndTime().toString().replace('T', ' ')};
                        view.getModel().addRow(row);
                    }
                } catch (ExecutionException | InterruptedException e) {
                    System.out.println("problem adding programs to table");
                }
            }
        }; swingWorker.execute();

    }

    /**
     * Method used to Exit the program.
     */
    private void quitProgram() {
        System.exit(0);
    }
}
