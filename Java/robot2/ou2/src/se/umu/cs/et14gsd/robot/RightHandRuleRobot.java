package se.umu.cs.et14gsd.robot;
/**
 * The Righthandrulerobot class responsibility is to move the robot
 * through the maze, but the robot always need to keep it's Right hand on a wall at all time.
 * @author Gustaf Söderlund -et14gsd
 * @since 2020/06/09
 * @version 2.00
 */

public class RightHandRuleRobot extends AbstractRobot{
    private Position hand;

    enum Compass {
        NORTH, SOUTH, EAST, WEST
    }

    /**
     * Constructing the RighthandruleRobot at the start position.
     */
    public RightHandRuleRobot(Maze maze){
        super(maze);
    }

    /**
     * A public method that makes the robot move around the maze,
     * The robot is going to have it's right hand on a wall at all time.
     * only time the robot wont have it's hand on a wall is if the
     * starting position is located next to no walls.
     * @return nothing.
     */
    public void move(){

        //Create variables for the wind directions.
        Position north = this.getPosition().getPosToNorth();
        Position south = this.getPosition().getPosToSouth();
        Position east = this.getPosition().getPosToEast();
        Position west = this.getPosition().getPosToWest();

        boolean moved = false;

        //When the robot need to walk around a corner.
        if (hand != null){
            while(!maze.map.get(hand).equals('*')){
                if(getPreviousPosition().equals(north)){
                    corner(Compass.WEST);
                    moved= true;
                }
                else if(getPreviousPosition().equals(south)){
                    corner(Compass.EAST);
                    moved = true;
                }
                else if(getPreviousPosition().equals(west)){
                    corner(Compass.SOUTH);
                    moved = true;
                }
                else if(getPreviousPosition().equals(east)){
                    corner(Compass.NORTH);
                    moved = true;
                }
            }
        }

        // check if the robot is able to move and place the hand on the wall.
        if(!moved) {
            if (maze.isMovable(north) && maze.map.get(east).equals('*') && !north.equals(this.getPreviousPosition())) {
                setPreviousPosition(getPosition());
                setPosition(north);
                hand = this.getPosition().getPosToEast();
                moved = true;

            } else if (maze.isMovable(south) && maze.map.get(west).equals('*') && !south.equals(this.getPreviousPosition())) {
                setPreviousPosition(getPosition());
                setPosition(south);
                hand = this.getPosition().getPosToWest();
                moved = true;
            } else if (maze.isMovable(west) && maze.map.get(north).equals('*') && !west.equals(this.getPreviousPosition())) {
                setPreviousPosition(getPosition());
                setPosition(west);
                hand = this.getPosition().getPosToNorth();
                moved = true;

            } else if (maze.isMovable(east) && maze.map.get(south).equals('*') && !east.equals(this.getPreviousPosition())) {
                setPreviousPosition(getPosition());
                setPosition(east);
                hand = this.getPosition().getPosToSouth();
                moved = true;
            }
        }
        //If the robot has not moved and need to get out from a corner or dead end.
        if(!moved){
            //Check all directions and move the direction thats possible.
            if(maze.map.get(north).equals('*') && maze.map.get(west).equals('*') && maze.map.get(south).equals('*') && maze.isMovable(east)){
                setPreviousPosition(getPosition());
                setPosition(east);
                hand = this.getPosition().getPosToSouth();
                moved = true;
            }
             //Check all directions and move the direction thats possible.
             if(maze.map.get(north).equals('*') && maze.map.get(west).equals('*') && maze.map.get(east).equals('*') && maze.isMovable(south)){
                setPreviousPosition(getPosition());
                setPosition(south);
                hand = this.getPosition().getPosToWest();
                moved = true;
            }
              //Check all directions and move the direction thats possible.
             if(maze.map.get(west).equals('*') && maze.map.get(east).equals('*') && maze.map.get(south).equals('*') && maze.isMovable(north)){
                setPreviousPosition(getPosition());
                setPosition(north);
                hand = this.getPosition().getPosToEast();
                moved = true;
            }
             //Check all directions and move the direction thats possible.
             if( maze.map.get(north).equals('*') && maze.map.get(east).equals('*') && maze.map.get(south).equals('*') && maze.isMovable(west)) {
                setPreviousPosition(getPosition());
                setPosition(west);
                hand = this.getPosition().getPosToNorth();
                moved = true;
            }
        }

        // If the start position is not next to any walls just walk north until a robot reach a wall.
        if(!moved){
            if ((maze.map.get(south).equals(' ') && maze.map.get(east).equals(' ')
                    && maze.map.get(west).equals(' ') && maze.map.get(north).equals(' '))
                    || maze.map.get(south).equals('S')){
                setPosition(getPreviousPosition());
                setPosition(north);
            }
        }
    }

    /**
     * This private method is used to make the robot take a step around a corner.
     * @param windDirection direction to move
     */
    private void corner(Compass windDirection){
        switch (windDirection){

            case WEST:
                this.setPreviousPosition(this.getPosition());
                this.setPosition(this.getPosition().getPosToWest());
                hand = getPosition().getPosToNorth();
                break;
            case SOUTH:
                this.setPreviousPosition(this.getPosition());
                this.setPosition(this.getPosition().getPosToSouth());
                hand = getPosition().getPosToWest();
                break;
            case NORTH:
                this.setPreviousPosition(this.getPosition());
                this.setPosition(this.getPosition().getPosToNorth());
                hand = getPosition().getPosToEast();
                break;
            case EAST:
                this.setPreviousPosition(this.getPosition());
                this.setPosition(this.getPosition().getPosToEast());
                hand = getPosition().getPosToSouth();
                break;
        }
    }
}