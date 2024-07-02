/*
* Program: RobotTest
* Created by: Gustaf Söderlund
* ID: et14gsd
* */
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.Reader;

// Read a file that contains a maze. Create the robot and make the robot walk to Goal.
public class RobotTest {
    public static void main(String[] args) throws FileNotFoundException {
        try {
            Reader inputFile = new FileReader(args[0]);
            Maze maze = new Maze(inputFile);

            //Create a robot named Gurra.
            RandomRobot robotGurra = new RandomRobot(maze);

            //Make the robot walk to goal. Loop this until robot has reached goal.
            while(!robotGurra.hasReachedGoal()) {

                robotGurra.printCoordinates();
                robotGurra.move();
            }
            //Print this when robot reached goal "G".
            System.out.println("Congrats. You reached the goal.");

            //if the program is unable to find the file. Tell user "error" and exit program.
        } catch (FileNotFoundException e) {
            System.out.println("ERROR. File not found, Exiting the program.");
            e.printStackTrace();
            System.exit(0);
        }
    }
}





