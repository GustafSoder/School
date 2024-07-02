/*
 * Programmeringsteknik med C och Matlab
 * Fall 19
 * Assignment 3

 * File:         ou3.c
 * Description:  A simple implementation of Conway's Game of Life.
 * Author:       Gustaf Söderlund
 * CS username:  et14gsd
 * Date:         2019-10/22
 * Input:        Choice of initial configuration and then instruction to step
 *               or exit.
 * Output:       Prints the game field in each step.
 * Limitations:  No validation of input.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Constants, representation of states */
#define ALIVE 'X'
#define DEAD '.'

/* Declaration of data structure */
typedef struct{
    char current;
    char next;
} cell;

/* Declaration of functions */
void initField(const int rows, const int cols, cell field[rows][cols]);
void loadGlider(const int rows, const int cols, cell field[rows][cols]);
void loadSemaphore(const int rows, const int cols, cell field[rows][cols]);
void loadRandom(const int rows, const int cols, cell field[rows][cols]);
void loadCustom(const int rows, const int cols, cell field[rows][cols]);
void printWorld(const int rows, const int cols, cell field[rows][cols]);
void worldUpdate(const int rows, const int cols, cell field[rows][cols]);

/* Function:    main
 * Description: Start and run games, interact with the user.
 * Input:       About what initial structure and whether to step or exit.
 * Output:      Information to the user, and the game field in each step.
 */

int main(void) {
  const int rows = 20;
  const int cols = 20;

  char choice;
  cell field[rows][cols];
  initField(rows, cols, field );

  do {

    printWorld(rows, cols, field);

    printf("Select one of the following options:\n");
    printf("\t(enter) Step\n");
    printf("\t(any)   Exit\n");
    scanf("%c", &choice);

    worldUpdate(rows,cols,field);

  } while(choice == '\n');

  return 0;
}

/* Function:    initField
 * Description: Initialize all the cells to dead, then asks the user about
 *              which structure to load, and finally load the structure.
 * Input:       The field array and its size.
 * Output:      The field array is updated.
 */

void initField(const int rows, const int cols, cell field[rows][cols]) {

    for (int r = 0 ; r < rows ; r++) {
        for (int c = 0 ; c < cols ; c++) {
            field[r][c].current = DEAD;
        }
    }

    printf("Select field spec to load ([G]lider, [S]emaphore, [R]andom ");
    printf("or [C]ustom): ");

    int ch = getchar();

	/* Ignore following newline */
    if (ch != '\n') {
        getchar();
    }

    switch (ch) {
        case 'g':
        case 'G':
            loadGlider(rows, cols, field);
            break;
        case 's':
        case 'S':
            loadSemaphore(rows, cols, field);
            break;
        case 'r':
        case 'R':
            loadRandom(rows, cols, field);
            break;
        case 'c':
        case 'C':
        default:
            loadCustom(rows, cols, field);
            break;
    }
}

/* Function:    loadGlider
 * Description: Inserts a glider into the field.
 * Input:       The field array and its size.
 * Output:      The field array is updated.
 */

void loadGlider(const int rows, const int cols, cell field[rows][cols]) {

    field[0][1].current = ALIVE;
    field[1][2].current = ALIVE;
    field[2][0].current = ALIVE;
    field[2][1].current = ALIVE;
    field[2][2].current = ALIVE;
}

/* Function:    loadSemaphore
 * Description: Inserts a semaphore into the field.
 * Input:       The field array and its size.
 * Output:      The field array is updated.
 */

void loadSemaphore(const int rows, const int cols, cell field[rows][cols]) {

    field[8][1].current = ALIVE;
    field[8][2].current = ALIVE;
    field[8][3].current = ALIVE;
}

/* Function:    loadRandom
 * Description: Inserts a random structure into the field.
 * Input:       The field array and its size.
 * Output:      The field array is updated. There is a 50 % chance that a cell
 *              is alive.
 */

void loadRandom(const int rows, const int cols, cell field[rows][cols]) {

  srand(time(NULL));

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++){
      int n = rand()%2;

      if(n == 0){
        field[r][c].current = DEAD;
      }
      else{
        field[r][c].current = ALIVE;
      }
    }
  }
}

/* Function:    loadCustom
 * Description: Lets the user specify a structure that then is inserted into
 *              the field.
 * Input:       The field array and its size.
 * Output:      The field array is updated.
 */

void loadCustom(const int rows, const int cols, cell field[rows][cols]) {

    printf("Give custom format string: ");
    do {
        int r, c;
        scanf("%d,%d", &r, &c);
        field[r][c].current = ALIVE;
    } while (getchar() != '\n');
}

/* Function:     printWorld
 * Description:  Print out the world.
 *
 * Input:       The field array and its size.
 * Output:      The updated version of the field array.
 */

void printWorld(const int rows, const int cols, cell field[rows][cols]) {

  for(int r = 0; r < rows; r++) {
    for(int c = 0; c < cols; c++) {
      printf("%c ", field[r][c].current);
    }
    printf("\n");
  }
}

/* Function:    worldUpdate
 * Description: Calculates the dead and alive cells to create the next field
 *              array.
 *
 * Input:       The current field array.
 * Output:      The next field array version.
 */

void worldUpdate(const int rows, const int cols, cell field[rows][cols]){

  int locNeighbour[rows][cols];

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      locNeighbour[r][c] = 0;

      if (!(r+1 == rows || c+1 == cols) && (field[r + 1][c + 1]).current == ALIVE){
        locNeighbour[r][c] += 1;
      }
      if(!(r-1 == -1 || c+1 == cols) && (field[r - 1][c + 1]).current == ALIVE){
        locNeighbour[r][c] += 1;
      }
      if(!(r+1 == rows || c-1 == -1) && (field[r + 1][c - 1]).current == ALIVE){
        locNeighbour[r][c] += 1;
      }
      if (!(r-1 == -1 || c-1 == -1) && (field[r - 1][c - 1]).current == ALIVE){
        locNeighbour[r][c] += 1;
      }
      if (!(r-1 == -1) && (field[r - 1][c]).current == ALIVE){
        locNeighbour[r][c] += 1;
      }
      if (!(c+1 == cols) && (field[r][c + 1]).current == ALIVE){
        locNeighbour[r][c] += 1;
      }
      if (!(r+1 == rows) && (field[r + 1][c]).current == ALIVE){
        locNeighbour[r][c] += 1;
      }
      if (!(c-1 == -1) && (field[r][c - 1]).current == ALIVE){
        locNeighbour[r][c] += 1;
      }
    }
  }

  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {

      if  (locNeighbour[r][c] < 2){
        field[r][c].next = DEAD;
        field[r][c].current = field[r][c].next;
      }
      if  (locNeighbour[r][c] == 2){
        field[r][c].next = field[r][c].current;
        field[r][c].current = field[r][c].next;
      }
      if (locNeighbour[r][c] > 3){
        field[r][c].next = DEAD;
        field[r][c].current = field[r][c].next;
      }
      if (locNeighbour[r][c] == 3){
        field[r][c].next = ALIVE;
        field[r][c].current = field[r][c].next;
      }
    }
  }
}
