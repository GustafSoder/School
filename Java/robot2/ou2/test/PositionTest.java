/**
 * @author Gustaf Söderlund -et14gsd
 * @since 2020/06/09
 * @version 2.00
 */
import org.junit.jupiter.api.Test;
import se.umu.cs.et14gsd.robot.Position;
import static org.junit.jupiter.api.Assertions.*;

class PositionTest {

    @Test
    void getX() {

        Position pos = new Position(0,1);
        assertEquals(pos.getX(),0);
    }

    @Test
    void getY() {
        Position pos = new Position(1,0);
        assertEquals(pos.getY(),0);
    }

    @Test
    void getPosToSouth() {
        Position pos = new Position(1,1);
        Position south = new Position( 1,2);
        assertEquals(pos.getPosToSouth(), south);
    }

    @Test
    void getPosToWest() {
        Position pos = new Position(1,1);
        Position west = new Position( 0,1);
        assertEquals(pos.getPosToWest(), west);
    }

    @Test
    void getPosToEast() {
        Position pos = new Position(1,1);
        Position east = new Position( 2,1);
        assertEquals(pos.getPosToEast(), east);
    }


    @Test
    void getPosToNorth() {
        Position pos = new Position(1,1);
        Position north= new Position( 1,0);
        assertEquals(pos.getPosToNorth(), north);
    }
}