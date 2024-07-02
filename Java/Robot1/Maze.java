/*
 * Created by: Gustaf Söderlund
 * ID: et14gsd
 * */
import java.util.List;
import java.util.ArrayList;
import java.io.Reader;
import java.util.Scanner;

// The class purpose is to create an array with the positions of start, goal, wall and road.
// The class can also count the number of rows and columns of the file, that we use for y and x axis.
public class Maze {
    static int colNumber;
    static int rowNumber = 0;
    private Position startPos;
    private Position goalPos;
    private List<Position> array1 = new ArrayList<Position>();
    private List<Position> array2 = new ArrayList<Position>();

    //Scan the maze file and get positions of walls '*',road ' ', start 'S' and goal 'G'
    public Maze(Reader file){

        Scanner reading = new Scanner(file);
            try{
                //Loop until the file got no more lines.
                while (reading.hasNextLine()) {

                    String row = reading.nextLine();
                    // Calculate columns x axis
                    if( colNumber < row.length()){ //
                        colNumber = row.length();
                    }

                    //Iterate the columns and get positions on 'S','G','*' and ' '.
                    for (int j = 0; j < row.length(); j++) {  // columns
                        char pos = row.charAt(j);
                        if (pos == 'S') {
                            this.startPos = new Position(j, rowNumber);
                        }
                        if (pos == 'G') {
                            this.goalPos = new Position(j, rowNumber);
                        }
                        if (pos == '*') {
                            array1.add(new Position(j, rowNumber));
                        }
                        if (pos == ' ') {
                            array2.add(new Position(j, rowNumber));
                        }
                        System.out.print(pos);
                    }
                        rowNumber = rowNumber + 1; // calculate rows y axis

                    System.out.println(); //This sout is used to make "enter" after the row is printed.
            }
         //If the program cant read the file, exit the program.
        }catch(Exception e){
                System.out.println("Error. Could not scan the file, exiting the program.");
                System.exit(0);
                reading.close();

        }

    }
        //Check if the position is movable, if there is any walls on that position '*'.
        public boolean isMovable (Position pos) {
            if (array1.contains(pos)){
                return false;
        }
            else {
                return true;
            }
        }
        //Checks if the position is goal 'G'
        public boolean isGoal(Position compPos){
            return this.goalPos.equals(compPos);
        }

        //Get the start position 'S'
        public Position getStart () {
            return this.startPos;
        }

        //Counts the number of row and return an int of the value.
        public int getNumRows () {
            return rowNumber;
        }

        //Counts the number of columns and return and in of the value.
        public int getNumColumns () {
            return colNumber;


        }
    }






