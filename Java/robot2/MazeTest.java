/**
 * @author Gustaf Söderlund -et14gsd
 * @since 2020/06/09
 * @version 2.00
 */

import org.junit.jupiter.api.Test;
import se.umu.cs.et14gsd.robot.Maze;
import se.umu.cs.et14gsd.robot.Position;
import java.io.*;
import static org.junit.jupiter.api.Assertions.*;

class MazeTest {
    /**
     * Testing a incorrect file. The file contains no start/goal positions.
     * @throws IOException
     */
    @Test
    void testIncorrectMaze()throws IOException {
        try{
            InputStream badfile = getClass().getResourceAsStream("badfile.maze");
            InputStreamReader badreader = new InputStreamReader(badfile);
            Maze badMaze = new Maze(badreader);

        }catch (Exception e){
            System.out.println("The maze got wrong input.");
        }
    }
    @Test
    void isMovable() throws IOException {
        File file = new File("testfil.maze");
        FileInputStream inputFile = new FileInputStream(file);
        InputStreamReader reader = new InputStreamReader(inputFile);
        Maze maze = new Maze(reader);

        Position posTrue = new Position(2,1);
        assertTrue(maze.isMovable(posTrue));
    }

    @Test
    void isGoal() throws IOException{
        File file = new File("testfil.maze");
        FileInputStream inputFile = new FileInputStream(file);
        InputStreamReader reader = new InputStreamReader(inputFile);
        Maze maze = new Maze(reader);

       Position goalpos = new Position(8,6);
        assertTrue(maze.isGoal(goalpos));
    }

    @Test
    void getStart() throws IOException {
        File file = new File("testfil.maze");
        FileInputStream inputFile = new FileInputStream(file);
        InputStreamReader reader = new InputStreamReader(inputFile);
        Maze maze = new Maze(reader);

        Position startPos = new Position(1,0);
        assertEquals(maze.getStart(), startPos);
    }

    @Test
    void getNumRows() throws IOException{
        File file = new File("testfil.maze");
        FileInputStream inputFile = new FileInputStream(file);
        InputStreamReader reader = new InputStreamReader(inputFile);
        Maze maze = new Maze(reader);

        int rows = 7;
        assertEquals(maze.getNumRows(), rows);
    }

    @Test
    void getNumColumns()throws IOException {
        File file = new File("testfil.maze");
        FileInputStream inputFile = new FileInputStream(file);
        InputStreamReader reader = new InputStreamReader(inputFile);
        Maze maze = new Maze(reader);

        int columns = 12;
        assertEquals(maze.getNumColumns(), columns);
    }
}