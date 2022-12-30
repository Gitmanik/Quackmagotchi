/*
 * queue.h
 *
 *  Created on: Mar 6, 2022
 *      Author: thegi
 */

#pragma once

#define QUEUE_MAX_ITEMS 16

#include <stdio.h>

typedef struct {

	uint8_t q[QUEUE_MAX_ITEMS];
	int8_t front;
	int8_t rear;
	int8_t itemCount;

} queue_t;

void QUEUE_Init(queue_t* queue);
void QUEUE_insert(queue_t* queue, uint8_t data);
int QUEUE_peek(queue_t* queue);
uint8_t QUEUE_isEmpty(queue_t* queue);
uint8_t QUEUE_isFull(queue_t* queue);
int QUEUE_size(queue_t* queue);
int QUEUE_get(queue_t* queue);
