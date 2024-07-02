#ifndef QUEUE_H
#define QUEUE_H
#include <stdio.h>
#include <stdbool.h>

/*
* queue.h - this code is the header file for queue.c.
*
* Course: 5DV088 Systemnära programmering.
*
* Author: Gustaf Söderlund (et14gsd@cs.umu.se)
* Version information: 2020/11/10 V2.0
*
*/
 typedef struct node {
    char *value;
    struct node *next;
}node;

 typedef struct queue {
    struct node *firstnode; // head
    struct node *lastnode; // tail
    int size;      //size

}queue;



/*
* queue_empty() - Create empty queue and initialize the head and tail.
*
*
* @ q - the queue.
*
*/
void queue_empty( queue *q);

/*
* queue_is_empty()- Check if the queue is empty or not.
*
* @ q - the queue.
*
* returns: bool.
*/
bool queue_is_empty(queue *q);


/*
* queue_add() - add an element in the end of the que.
*
*
* @ q - the queue.
*/
void queue_add( queue *q, char *value);

/*
* queue_remove() - remove the first element in the que.
*
* @ q - the queue.
*/
void queue_remove(queue *q);

/*
* queue_first() - return the first element in the que.
*
* @ q - the queue.
*/
char* queue_first(queue *q);


/*
 queue_size() return the size of queue; 
*/
int queue_size(queue *q);


#endif // !defined QUEUE_H
