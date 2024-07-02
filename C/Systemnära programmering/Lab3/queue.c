/**
* queue.c - this code is constructed to create the datatype queue.
*
* Course: 5DV088 Systemnära programmering.
*
* Author: Gustaf Söderlund (et14gsd@cs.umu.se)
* Version information: 2020/11/10 V2.0
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "queue.h"

/**
* queue_empty() - Create empty queue and initialize the head and tail.
*
*
* @param q - the queue.
*
*/
void queue_empty( queue *q)
{
  q->firstnode = NULL; //head
  q->lastnode = NULL; //tail
  //q->size = malloc(sizeof(int));
  q->size = 0; 
}

/**
* queue_is_empty()- Check if the queue is empty or not.
*
* @param q - the queue.
*
* @returns: bool.
*/
bool queue_is_empty(queue *q)
{
  if(q->firstnode == NULL)
  {
    return true;
  }
  return false;
}

/**
* queue_add() - add an element in the end of the que.
*
*
* @param q - the queue.
*/
void queue_add(queue *q, char *value)
{
  node *newnode = malloc(sizeof(node));
  if(newnode == NULL)
  {
    exit(EXIT_FAILURE);
  }

  newnode->value = malloc(strlen(value)+1);
  if(newnode->value == NULL)
  {
    exit(EXIT_FAILURE);
  }
  strcpy(newnode->value,value);
  newnode->next = NULL;

  if(q->lastnode != NULL)
  {
    q->lastnode->next = newnode;
    
  }
  q->lastnode = newnode;

  if(q->firstnode == NULL)
  {
    q->firstnode = newnode;
  }
  q->size++;
}


/**
* queue_remove() - remove the first element in the que.
*
*
* @param q - the queue.
*/
void queue_remove(queue *q)
{
    // check if queue is empty already...
    if(queue_is_empty(q))
    {
      printf("already empty. returning.\n");
      return;

    }else
    {

    // create tmp node to hold first value.
    node *tmp = q->firstnode->next;
    free(q->firstnode->value);
    free(q->firstnode);
    // move 2nd node to first node.
    q->firstnode = tmp;

    // if first node is null the rest of nodes also is null.
    if (q->firstnode == NULL)
    {
      q->lastnode = NULL;
    }
    q->size--; 
  }
}

/**
* queue_first() - return the first element in the que.
*
* @param q - the queue.
*/
char* queue_first(queue *q){
    return q->firstnode->value;
}

/**
* queue_size()- This function returns the size of the Queue. 
* @param q the queue
*/

int queue_size(queue *q)
{
  return q->size;
}
