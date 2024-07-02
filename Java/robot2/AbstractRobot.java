package se.umu.cs.et14gsd.robot;

/**
 *  @author Gustaf Söderlund -et14gsd
 *  @since 2020/06/09
 *  @version 2.00
 */
public abstract class AbstractRobot implements Robot {
    private Position previousPosition;
    private Position position;
    protected Maze maze;

    public AbstractRobot(Maze maze){
        this.maze = maze;
        this.position = maze.getStart();
        this.previousPosition = maze.getStart();
    }

    @Override
    public abstract void move();

    /**
     * A method to get the current position.
     * @return the current position.
     */
    @Override
    public Position getPosition() {
        return this.position;
    }

    public Position getPreviousPosition() {
        return this.previousPosition;
    }

    /**
     * A method used to check if the robot has reached the goal position.
     * @return true if the robot reached goal.
     * @return false if the robot is not at goal.
     */
    @Override
    public boolean hasReachedGoal () {
        return this.maze.isGoal(position);
    }

    /**
     * A method to change the current position to another position.
     * @param nextPos
     */
    public void setPosition (Position nextPos){
        this.position = nextPos;
    }
    public void setPreviousPosition(Position pos){
         this.previousPosition = this.getPosition();
    }

}
