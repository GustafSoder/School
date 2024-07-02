package se.umu.cs.et14gsd.robot;

/**
 * @author Gustaf Söderlund -et14gsd
 * @since 2020/06/09
 * @version 2.00
 */


import java.util.HashMap;
import java.io.Reader;
import java.util.Scanner;

/**
 * A public class used to create a HashMap with the positions of
 * 'S' start, 'G' Goal and '*' Wall. The symbols is readed from a textfile.
 */
public class Maze{

    private int rowNumber = 0;
    private String quote = "";
    private Position position;
    private int counter = 0;
    private char start = 'S';
    private char goal = 'G';
    private int maxRowlength = 0;
    private boolean boolGoal = false;
    public HashMap<Position, Character> map = new HashMap<Position, Character>();

    /**
     * Scan the maze file and get positions of walls '*',road ' ', start 'S' and goal 'G'.
     * @param file
     */

    public Maze(Reader file){
        Scanner reading = new Scanner(file);

            // Do the while loop as long as the file got a next line.
            while(reading.hasNextLine()) {
                quote = reading.nextLine();
                // Iterrate the row and look for symbols. When symbol found put it in the hashmap.
                for (int i = 0; i < quote.length(); i++) {
                    this.position = new Position(i, rowNumber);

                    char charInMaze = quote.charAt(i);
                    if (charInMaze == start) {
                        counter++;
                    }
                    if (goal == charInMaze) {
                        boolGoal = true;
                    }
                    if (maxRowlength <= quote.length()) {
                        maxRowlength = quote.length();
                    }
                    map.put(position, charInMaze);
                }
                rowNumber ++;
                this.position = new Position(0, rowNumber);
            }
            reading.close();
            //Throw if the maze file in incorrect.
            if (counter != 1 || !boolGoal){
                throw new RuntimeException("The maze is Incorrect. Please configure the file with correct input.");
            }
        }

    /**
     * A method used to check if a position got a wall '*'
     * @param pos
     * @return bool, true if there is no wall '*' and false otherwise.
     */
    public boolean isMovable (Position pos) {
        try{
             return this.map.get(pos) != ('*');
        }catch (NullPointerException e){
            return false;
        }
    }

    /**
     * A method used to check if the given position is the goal position.
     * @param compPos
     * @return bool, True if the given position is goal 'G', otherwise false.
     */
    public boolean isGoal(Position compPos){
      try{
            return this.map.get(compPos).equals('G');
      }catch(NullPointerException  e){
            return false;
      }
    }

    /**
     * A method used to get the start position for the maze.
     * @return the start position in the maze.
     */
    public Position getStart () {
        for (Position j : map.keySet()) {
            Character status = map.get(j);
            if (status == 'S') {

                return j;
            }
        }
        return null;
    }

    /**
     * A method used to return the amount of rows in the maze
     * @return amount of rows.
     */
    public int getNumRows () {
        return rowNumber;
    }

    /**
     * A method used to return the amount of columns in the maze.
     * @return the amount of columns.
     */
    public int getNumColumns () {
        return maxRowlength;
    }
}
