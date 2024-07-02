/**
 * @author Gustaf Söderlund -et14gsd
 * @since 2020/06/09
 * @version 2.00
 */

import org.junit.jupiter.api.Test;
import se.umu.cs.et14gsd.robot.Maze;
import se.umu.cs.et14gsd.robot.Position;
import se.umu.cs.et14gsd.robot.RandomRobot;
import java.io.*;
import static org.junit.jupiter.api.Assertions.*;

class RobotTest {

   @Test
   void move() throws IOException{
       File file = new File("testfil.maze");
       FileInputStream inputFile = new FileInputStream(file);
       InputStreamReader reader = new InputStreamReader(inputFile);
       Maze maze = new Maze(reader);

       RandomRobot testRobot = new RandomRobot(maze);
       Position testPos1 = testRobot.getPosition();
       testRobot.move();

       assertNotEquals(testRobot.getPosition(), testPos1);
   }

    @Test
    void getPosition() throws IOException {
        File file = new File("testfil.maze");
        FileInputStream inputFile = new FileInputStream(file);
        InputStreamReader reader = new InputStreamReader(inputFile);
        Maze maze = new Maze(reader);

        RandomRobot testRobot = new RandomRobot(maze);
        Position testPos = testRobot.getPosition();
        assertEquals(testPos, maze.getStart());
    }

    @Test
    void hasReachedGoal()throws IOException {
        File file = new File("testfil.maze");
        FileInputStream inputFile = new FileInputStream(file);
        InputStreamReader reader = new InputStreamReader(inputFile);
        Maze maze = new Maze(reader);
        RandomRobot testRobot = new RandomRobot(maze);

        while(!testRobot.hasReachedGoal()){
            testRobot.move();
        }
        Position goal = new Position(8,6);
        assertEquals(maze.isGoal(goal), testRobot.hasReachedGoal());
    }
}