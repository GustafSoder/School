package se.umu.cs.et14gsd.robot;
/**
 * @author Gustaf Söderlund -et14gsd
 * @since 2020/06/09
 * @version 2.00
 */

import java.util.Stack;

/**
 * a Public class to move the se.umu.cs.et14gsd.robot.MemoryRobot through
 * a maze. The robot need to remember the positions
 * where the robot has visited and is able to go the same way it
 * traveled.
 *
 *
 */

public class MemoryRobot extends AbstractRobot {

    // StackPos keeps track on the robots current position.
    private Stack<Position> stackPos = new Stack<Position>();
    //StackPosSeen keeps track on the position that the robot has visited.
    private Stack<Position> stackPosSeen = new Stack<Position>();
    //Compass, used later in the move method.
    enum compass{
        NORTH,
        EAST,
        WEST,
        SOUTH,
    }

    /**
     * Constructs and initializes the robot at the start position.
     * and push the start positions.
     */
    public MemoryRobot(Maze maze){
        super(maze);
        stackPos.push(this.getPosition());
        stackPosSeen.push(this.getPosition());
    }

    /**
     * A method to make the robot move through the maze
     * @return the method return nothing.
     */
    @Override
    public void move() {

        //Check where the robot is able to move and move the robot in that direction
        if (checkIfMovable(this.getPosition().getPosToNorth())) {
            moveRobot(compass.NORTH);
        }

        else if (checkIfMovable(this.getPosition().getPosToSouth())) {
            moveRobot(compass.SOUTH);
        }

        else if (checkIfMovable(this.getPosition().getPosToWest())){
            moveRobot(compass.WEST);
        }

        else if (checkIfMovable(this.getPosition().getPosToEast())) {
            moveRobot(compass.EAST);
        }
        // if robot gets cornered go back to last position and remove the top position in stack.
        else {
            stackPos.pop();
            this.setPosition(stackPos.peek());
        }
    }

    /**
     * a Private method used to check if the robot can move.
     * @param position
     * @return bool
     */
    private boolean checkIfMovable(Position position){
        return maze.isMovable(position) && stackPosSeen.search(position) <0;

    }

    /**
     * A method used to move the robot, and push the position into two stacks.
     * @param dir
     */
    private void moveRobot(compass dir){
        switch (dir){
            case EAST:
                this.setPosition(this.getPosition().getPosToEast());
                stackPosSeen.push(this.getPosition());
                stackPos.push(this.getPosition());
                break;
            case WEST:
                this.setPosition(this.getPosition().getPosToWest());
                stackPosSeen.push(this.getPosition());
                stackPos.push(this.getPosition());
                break;

            case NORTH:
                this.setPosition(this.getPosition().getPosToNorth());
                stackPosSeen.push(this.getPosition());
                stackPos.push(this.getPosition());
                break;

            case SOUTH:
                this.setPosition(this.getPosition().getPosToSouth());
                stackPosSeen.push(this.getPosition());
                stackPos.push(this.getPosition());
                break;
        }
    }
}
