import javax.swing.*;
/**
 * @author Gustaf Söderlund - et14gsd
 * @since 2023/01/31
 * @version 1.00
 */

public class RadioInfo {
    public static void main(String[] args){
        SwingUtilities.invokeLater(() -> {
            View view = new View();
            Controller controller = new Controller(view);
            controller.setUp();
        });
    }
}
