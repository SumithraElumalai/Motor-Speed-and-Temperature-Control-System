/*
 * queue.h
 *
 * As provided by Daniel W. Lewis in "Fundamentals of Embedded Software with
 * the ARM Cortex-M3" Second Edition, Pearson Higher Education, Inc. 2013
 *
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint32_t nq;      // insertion index
    uint32_t dq;      // removal index
    uint32_t count;   // #items in queue
    uint32_t size;    // buffer capacity
    uint8_t bfr[ 1 ]; // buffer array
} QUEUE;

QUEUE* QUEUE_Create( uint32_t bfrsize );

bool QUEUE_Enqueue( QUEUE *q, uint8_t data );
bool QUEUE_Dequeue( QUEUE *q, uint8_t *ptr2data );

#endif /* QUEUE_H_ */
