/*
This program will let the user read in the number of judges and the score
from each judge. The program will calculate the average score
without regard to the lowest and highest score. The program will also print
out the highest, lowest and the final average score in the end of the program.

Namn: Gustaf Söderlund
UmuID: Gusa0038   et14gsd@cs.umu.se
CS-användare: et14gsd
Date: 2019-10-15
 */
#include <stdio.h>

/* Declaration of functions */
void printText();
int numJudge();
void scoreJudge(const int size, double arrScore[]);
void loadedScores(const int size, double arrScore[]);
void finalScore(const int size, double arrScore[], double *high, double *low, double *mean);
void printFinalScore(double high, double low, double mean);

int main(void){

  double high, low, mean;
  printText();

  const int size = numJudge();
  double arrScore[size];

  scoreJudge(size, arrScore);
  loadedScores(size, arrScore);
  finalScore (size, arrScore, &high, &low, &mean);
  printFinalScore(high, low, mean);

  return 0;
}

/* Function:    printText
 * Description: This function will write out the program info.
 * Input:       void.
 * Output:      Print out program information in the terminal.
 */

void printText(void) {

  printf("Program information\n");
  printf("The program reads in the number of judges and the score from each judge.\n");
  printf("Then it calculates the average score without regard to the lowest and\n");
  printf("highest judge score. Finally it prints the results (the highest, the\n");
  printf("lowest and the final average score).\n\n");

}

/* Function:    numJudge
 * Description: This function will calculate the number of judges.
 * Input:       No input
 * Output:      The number of judges will be printed out in the terminal, decided by the user.
 */

int numJudge(){

  int num = 0;
  while(num < 3 || num > 10 ){
    printf("Number of judges (min 3 and max 10 judges)? ");
    scanf("%d", &num);
  }

  return num;
}

/* Function:    scoreJudge
 * Description: This function receive the score from the user.
 * Input:       Amount of judges and the arrayScore field.
 * Output:      The score from each judge will be received in the terminal.
 */

void scoreJudge(const int size, double arrScore[]){

  printf("\n");

  for(int i = 0; i < size; i++){
    printf("Score from judge %d? ", (i + 1));
    scanf("%lf", &arrScore[i]);
  }
}

/* Function:    loadedScores
 * Description: This function print out the score that the user scanned for each judge.
 * Input:       Amount of judges and arrayScore field.
 * Output:      The received score from each judge will be printed out in the terminal
 */

void loadedScores(const int size, double arrScore[]){

  printf("\nLoaded scores: \n");

  for (int i = 0; i < size; i++) {
    printf("Judge %d: %.1lf \n", (i + 1), arrScore[i]);

  }
}

/* Function:    finalScore
 * Description: This function calculate the highest, lowest and the average score.
 * Input:       The score from each judge.
 * Output:      The highest score, lowest score and the average score minus the highest and the lowest will be calculated.
 */

void finalScore(const int size, double arrScore[], double *high,
                double *low, double *mean){

  double sum = 0;

  *high = arrScore[0];
  *low = arrScore[0];

  for (int i = 0; i < size; i++) {
    if (arrScore[i] > *high) {
      *high = arrScore[i];

    }
    if (arrScore[i] < *low){
      *low = arrScore[i];
    }
    sum += arrScore[i];
  }
  *mean = (sum -( *low + *high)) / (size - 2);
}

/* Function:    printFinalScore
 * Description: This function will print out the highest, lowest and the average score.
 * Input:       The highest, lowest and the average score.
 * Output:      The highest, lowest and average score will be printed out in the termnal.
 */

void printFinalScore(double high, double low, double mean){

  printf("\nFinal result:\n");
  printf("Highest judge score: %.1lf\n",high);
  printf("Lowest judge score: %.1lf\n",low);
  printf("Final average score: %.1lf\n",mean);
}
