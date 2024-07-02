/*
 * Created by: Gustaf Söderlund
 * ID: et14gsd
 * */
import java.util.ArrayList;

// The class purpose is to move the robot through the given maze.
public class RandomRobot {
    private Position position;
    private Position previousPosition;
    private Maze maze;

    // Construct the robot and sets the start position for the robot.
    public RandomRobot(Maze maze) {
        this.maze = maze;
        this.position = maze.getStart();
        this.previousPosition = maze.getStart();
    }

    //The move methods purpose is to move the robot. Not able to move diagonaly.
    public void move() {

        Maze maze = this.maze;
        boolean check = false;

        // create list of directions
        ArrayList direction = new ArrayList();
        //Keeps count for the random later
        int n = 0;

        //Check where the robot is able to move and add it to a list
        if (maze.isMovable(position.getPosToNorth()) && !position.getPosToNorth().equals(previousPosition) && position.getPosToNorth().getY() >= 0) {
            check = true;
            direction.add("north");
            n++;
        }

        if (maze.isMovable(position.getPosToSouth()) && !position.getPosToSouth().equals(previousPosition) && position.getPosToSouth().getY() < maze.getNumRows()) {
            check = true;
            direction.add("south");
            n++;
        }

        if (maze.isMovable(position.getPosToWest()) && !position.getPosToWest().equals(previousPosition) && position.getPosToWest().getX() >= 0) {
            check = true;
            direction.add("west");
            n++;
        }

        if (maze.isMovable(position.getPosToEast()) && !position.getPosToEast().equals(previousPosition) && position.getPosToEast().getX() < maze.getNumColumns()) {
            check = true;
            direction.add("east");
            n++;
        }
            // Use the randomDir to get a random number from the list that equals a direction.
            //Move to that direction.
            if(check) {
                int randomDir = (int)(Math.random() * (n) + 0);

                if("north" == direction.get(randomDir)) {
                    previousPosition = position;
                    position = position.getPosToNorth();
                }
                else if("south" == direction.get(randomDir)) {
                    previousPosition = position;
                    position = position.getPosToSouth();
                }

                else if("west" == direction.get(randomDir)) {
                    previousPosition = position;
                    position = position.getPosToWest();
                }
                else if("east" == direction.get(randomDir)) {
                    previousPosition = position;
                    position = position.getPosToEast();
                }
            }
            // If the robot gets cornered. Take a step back to previousPosition.
            else {
                position = previousPosition;
            }
    }
    // Get the current position, i never use this method.
    public Position getPosition() {
        return position;
    }
    // Set the next position, I never use this method
    private void setPosition (Position nextPos){ //
        position = nextPos;
    }

    // Check if robot has reached the goal
    public boolean hasReachedGoal () {
        if (this.maze.isGoal(position)) {
            return true;
        }
        else {
            return false;
        }
    }

    //This method prints out the positions that the robot is walking.
    public void printCoordinates () {
            System.out.println("X:" + this.position.getX() + " Y:" + this.position.getY());
    }

}



