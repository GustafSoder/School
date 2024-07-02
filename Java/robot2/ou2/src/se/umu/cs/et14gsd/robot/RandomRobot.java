package se.umu.cs.et14gsd.robot;
import java.util.ArrayList;
import java.util.Random;

/**
 * Randomrobot class is a robot class where the robot moves in a random direction every step.
 * @author Gustaf Söderlund -et14gsd
 * @since 2020/06/09
 * @version 2.00
 */

public class RandomRobot extends AbstractRobot {


    /**
     * Construct the randomrobot and initializes the robot at the start position.
     * @param maze the maze that the robot will orient through.
     */

    public RandomRobot(Maze maze) {
        super(maze);
    }

    /**
     * A method to make the robot move through the maze
     * @return the method return nothing.
     */

    public void move() {
        //making directions to make it easier to read.
        Position north = this.getPosition().getPosToNorth();
        Position south = this.getPosition().getPosToSouth();
        Position east = this.getPosition().getPosToEast();
        Position west = this.getPosition().getPosToWest();

        boolean check = false;

        // create list that wil contain directions
        ArrayList<String> direction = new ArrayList();

        //Check where the robot is able to move and add it to a list
        if (maze.isMovable(north) && !north.equals(getPreviousPosition())) {
            check = true;
            direction.add("north");

        } if (maze.isMovable(south) && !south.equals(getPreviousPosition())) {
            check = true;
            direction.add("south");

        } if (maze.isMovable(west) && !west.equals(getPreviousPosition())) {
            check = true;
            direction.add("west");
        }  if (maze.isMovable(east) && !east.equals(getPreviousPosition())) {
            check = true;
            direction.add("east");
        }


        // Use the randomDir to get a random direction from the list that equals a direction.
        // Move to that direction.
        if(check) {

            Random rand = new Random();
            String randomDir = direction.get(rand.nextInt(direction.size()));

            if(randomDir == "north") {
                this.setPreviousPosition(this.getPosition());
                this.setPosition(getPosition().getPosToNorth());
            }
            else if("south" == randomDir) {
                this.setPreviousPosition(this.getPosition());
                this.setPosition(getPosition().getPosToSouth());
            }
            else if("west" == randomDir) {
                this.setPreviousPosition(this.getPosition());
                this.setPosition(getPosition().getPosToWest());
            }
            else if("east" == randomDir) {
                this.setPreviousPosition(this.getPosition());
                this.setPosition(getPosition().getPosToEast());
            }
        }
        // If the robot gets cornered. Take a step back to previousPosition.
        else {
            this.setPreviousPosition(getPosition());
            this.setPosition(getPreviousPosition());
        }
    }
}