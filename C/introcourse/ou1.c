/*
This program will read in prices in current foreign currency and present the
sum of the prices in current currency and in swedish crowns. The user is able
to set the exchange rate on it's own.
Namn: Gustaf Söderlund
UmuID: Gusa0038   et14gsd@cs.umu.se
CS-användare: et14GSD
Date: 2019-10/10
*/

#include <stdio.h>

int main(void){
  
/*variables*/
  int choice = 0;
  double sek = 1.00, foreignCurrency, sumForeign = 0, sumsek = 0;

  printf("Your shopping assistant\n");

  /* Aslong as the user has not ended the program, repeat the following.*/
  while (choice != 3){

    /*Print out the menu and read choice.*/
    printf("\n1. Set exchange rate in SEK (current rate: %.2lf)\n", sek);
    printf("2. Read prices in the foreign currency\n");
    printf("3. End\n\n");
    printf("Give your choice (1 - 3): ");
    scanf("%d", &choice);

    switch (choice){

        /*Set exchange rate.*/
      case 1:
        printf("\nGive exchange rate: ");
        scanf("%lf", &sek);
      break;

      /*If the choice is to read prices, Ask for prices
        and also if it's a valid price.*/
      case 2:
        printf("\n");

        do {
          printf("Give price (finish with < 0): ");
          scanf("%lf", &foreignCurrency);

          if (foreignCurrency > 0){
              sumForeign += foreignCurrency;
          }

        } while(foreignCurrency >= 0);

        /*Print out the sum in foreign currency and in swedish crowns.*/
        printf("\nSum in foreign currency: %.2lf\n", sumForeign);
        printf("Sum in SEK: %.2lf\n", sumsek = sumForeign * sek);

        /* Reset the sum of the foreign currency and swedish crowns. */
        sumsek = 0;
        sumForeign = 0;
      break;

      /* If user want to end program.*/
      case 3:
        printf("\nEnd of program!\n\n");
      break;
      /*If not a valid choice.*/
      default:
        printf("\nNot a valid choice!\n");
    }
  }
return 0;
}
