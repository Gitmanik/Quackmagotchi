/*
 * queue.c
 *
 *  Created on: Mar 6, 2022
 *      Author: Gitmanik
 */

//https://www.tutorialspoint.com/data_structures_algorithms/queue_program_in_c.htm

#include <stdio.h>
#include <stdbool.h>

#include "queue.h"

void QUEUE_Init(queue_t* queue)
{
	queue->rear = -1;
	queue->front = 0;
	queue->itemCount = 0;
}

int QUEUE_peek(queue_t* queue) {
   return queue->q[queue->front];
}

bool QUEUE_isEmpty(queue_t* queue) {
   return queue->itemCount == 0;
}

bool QUEUE_isFull(queue_t* queue) {
   return queue->itemCount == QUEUE_MAX_ITEMS;
}

int QUEUE_size(queue_t* queue) {
   return queue->itemCount;
}

void QUEUE_insert(queue_t* queue, uint8_t data) {

   if(!QUEUE_isFull(queue)) {

      if(queue->rear == QUEUE_MAX_ITEMS-1) {
         queue->rear = -1;
      }

      queue->q[++queue->rear] = data;
      queue->itemCount++;
   }
}

int QUEUE_get(queue_t* queue) {
   uint8_t data = queue->q[queue->front++];

   if(queue->front == QUEUE_MAX_ITEMS) {
      queue->front = 0;
   }

   queue->itemCount--;
   return data;
}
