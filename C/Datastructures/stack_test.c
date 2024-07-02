/*
 * Datastrukturer och algoritmer
 * Spring 2020
 * Assignment 2
 * File:         stack_test.c
 * Description:  Stack testing
 * Author: Gustaf Söderlund
 * CS username: et14gsd
 * Date: 2020-02-18
 * Input:        None
 * Output:       Successfull steps using stack.
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include "stack.h"

/*
 * Verify that a newly created stack is empty. Prints an
 * error message to stderr and exits with an error signal if the test
 * fails.
 */
void stack_empty_test()
{
	//create empty stack.
	stack *s=stack_empty(NULL);

	// Check if stack is empty
	if (!stack_is_empty(s))
  {

		//If stack is not empty Send error message to stderr and Exit the test.
		fprintf(stderr, "FAIL: New stack is not empty!\n");
		exit(EXIT_FAILURE);
	}

	// Delete the stack.
	stack_kill(s);
}

/*
 * Verify that it's possible to push an element into the empty stack.
 * If the test fails. Print an error message to stderr and exit with an error
 * signal if test fails.
 */
void push_test()
{
	// create empty stack.
	stack *s=stack_empty(NULL);

	//Allocte memory with value 4 into the empty stack
	int *v=malloc(sizeof(*v));
	*v=4;

	// Push the allocated memory into the stack.
	s = stack_push(s, v);

	// Check if stack is empty. If stack is empty write error message to stderr and exit.
	if (stack_is_empty(s))
	{
		fprintf(stderr, "FAIL: stack is empty!\n");
		exit(EXIT_FAILURE);
	}

	// Deallocate the memory and delete the stack.
	free(v);
	stack_kill(s);
}

/*
 * Verify that it's possible to remove the top element in the stack.
 * If the test fails. Print an error message to stderr and exit with an error
 * signal if test fails.
 */
void pop_test()
{
	// Create empty stack.
	stack *s=stack_empty(NULL);

	// Allocate memory with value 4.
	int *v=malloc(sizeof(*v));
	*v=4;

	// Push the value into the stack
	s = stack_push(s, v);

	// Pop the top value.
	s = stack_pop(s);

	//if stack is not empty. Send error message to stderr and exit the test.
	if (!stack_is_empty(s))
	{
		fprintf(stderr, "FAIL: Stack is not empty.\n");
		exit(EXIT_FAILURE);
	}
	// Deallocate the memory and delete the stack.
  free(v);
	stack_kill(s);
}

/*
 * Verify that it's possible to check the top element of the stack.
 * If the test fails. Print and error message to stderr and exit with an error
 * signal if test fails.
 */
void top_test()
{
	//Create empty stack
	stack *s=stack_empty(NULL);

	//create element with value 4
	int *v = malloc(sizeof(*v));
	*v=4;

	//insert the value 4 into the stack.
	s = stack_push(s, v);

	// Check the top value and save it to *topvalue.
	int *topvalue = stack_top(s);

	//if topvalue is not value 4 send error message to stderr and exit
	if (*topvalue != *v )
	{
		fprintf(stderr, "FAIL: Test failed there's no value on top of stack .\n");
		exit(EXIT_FAILURE);
	}

	// Deallocate the memory and delete the stack.
	free(v);
	stack_kill(s);
}
/*
 * Verify that it's possible to use axiom five on the created stack.
 * If the test fails. Print and error message to stderr and exit with an error
 * signal if test fails.
 */
void equal_test()
{
  //Create empty stack
  stack *s=stack_empty(NULL);

  for (size_t i = 0; i < 10; i++)
  {
    int *v = malloc(sizeof(*v));
    *v = 1;
    s = stack_push(s, v);
  }

  if(stack_is_empty(s))
  {
    fprintf(stderr, "FAIL: The stack is empty.\n");
    exit(EXIT_FAILURE);
  }
  int *v = malloc(sizeof(*v));
  *v = 1;

  int *top_1 = stack_top(s);
  s = stack_pop(s);
  s = stack_push(s, v);
  int *top_2 = stack_top(s);

  // Compare the top_1 value and top_2 value. If they are not the same value
  // Send error message to stderr and exit.
  if(*top_1 != *top_2)
  {
    fprintf(stderr, "FAIL: Test failed the stack is not the same.\n");
    exit(EXIT_FAILURE);
  }

  while(!stack_is_empty(s))
  {
    free(stack_top(s));
    s = stack_pop(s);
  }

  // Deallocate the memory and delete the stack.
  free(top_1);
  stack_kill(s);
}


int main (void)
{
	fprintf(stderr,"Running TEST1: stack_empty_test(): \n");
	stack_empty_test();
	fprintf(stderr,"OK.\n");

	fprintf(stderr,"Running TEST2: push_test(): \n");
	push_test();
	fprintf(stderr,"OK.\n");

  fprintf(stderr,"Running TEST3: pop_test(): \n");
	pop_test();
  fprintf(stderr,"OK.\n");

	fprintf(stderr,"Running TEST4: top_test(): \n");
	top_test();
	fprintf(stderr,"OK.\n");

  fprintf(stderr,"Running TEST5: equal_test(): \n");
	equal_test();
	fprintf(stderr,"OK.\n");

	fprintf(stderr,"SUCCESS: Implementation passed all tests. Normal exit.\n");

	return 0;
}
