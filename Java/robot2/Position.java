package se.umu.cs.et14gsd.robot; /**
 * @author Gustaf Söderlund -et14gsd
 * @since 2020/06/08
 * @version 2.00
 */
import java.util.Objects;

/**
 * A point representing a location in {@code (x, y)} coordinate space,
 * specified in integer precision.
 */
public class Position{

    private int x;
    private int y;

    /**
     * Constructs and initializes a point at the specified
     * {@code (x, y)} location in the coordinate space.
     * @param x the x-coordinate
     * @param y the y-coordinate
     */
    public Position(int x, int y) {
        this.x = x;
        this.y = y;
    }

    /**
     * Returns the x-coordinate of the current position.
     *
     * @return the x-coordinate of the current position
     */
    public int getX() {

        return this.x;
    }

    /**
     * Returns the y-coordinate of the current position
     *
     * @return the y-coordinate of the current position
     */
    public int getY() {

        return this.y;
    }

    /**
     * Returns the position to the south of the current position
     * @return new position to south
     */
    public Position getPosToSouth() {
        return new Position(x,y+1);
    }

    /**
     * Returns the position to the west of the current position
     * @return new position to West
     */
    public Position getPosToWest() {
        return new Position(x-1,y);
    }


    /**
     * Returns the position to the east of the current position
     * @return new position to East
     */
    public Position getPosToEast() {
        return new Position(x+1,y);
    }


    /**
     * Returns the position to the north of the current position
     * @return new position to North
     */
    public Position getPosToNorth() {
        return new Position(x,y-1);
    }

    /**
     * Chec if a position is equal to the current position.
     * @param o
     * @return bool, true if the object is equal, otherwise false
     */
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Position position = (Position) o;
        return x == position.x &&
                y == position.y;
    }

    /**
     * Returns the integer hash code value of the object
     *
     * @return an integer
     */
    @Override
    public int hashCode() {

        return Objects.hash(x, y);
    }
}
