package se.umu.cs.et14gsd.robot;

/**
 * @author Gustaf Söderlund -et14gsd
 * @since 2020/06/09
 * @version 2.00
 */


/**
 * se.umu.cs.et14gsd.robot.Robot.java
 * Inteface class that has following methods
 *
 */
public interface Robot  {


    /**
     * A method to make the robot move through the maze
     * @return the method return nothing.
     */
    void move();

    /**
     * A method to get the current position.
     * @return the current position.
     */
    Position getPosition();

    /**
     * A method used to check if the robot has reached the goal position.
     * @return true if the robot reached goal.
     * @return false if the robot is not at goal.
     */
    boolean hasReachedGoal ();
}
