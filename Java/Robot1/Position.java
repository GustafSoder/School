/*
 * Created by: Gustaf Söderlund
 * ID: et14gsd
 * */
import java.util.Objects;

// The class purpose is to make cordinates at x and y.
public class Position{
    //Create x and y variable.
    public int x;
    public int y;

    //Constructs and initializes the x and y variabels.
    public Position(int x, int y) {
        this.x = x;
        this.y = y;
    }

    //return the x value
    public int getX() {

        return x;
    }

    //return the y value.
    public int getY() {

        return y;
    }

    //Get the position to south
    public Position getPosToSouth() {
        return new Position(x,y+1);
    }

    // Get the position to West
    public Position getPosToWest() {
        return new Position(x-1,y);
    }

    //Get the position to east
    public Position getPosToEast() {
        return new Position(x+1,y);
    }

    // Get the position to North.
    public Position getPosToNorth() {
        return new Position(x,y-1);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Position position = (Position) o;
        return x == position.x &&
                y == position.y;
    }

    @Override
    public int hashCode() {

        return Objects.hash(x, y);
    }
}
