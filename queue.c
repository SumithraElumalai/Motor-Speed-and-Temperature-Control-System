/*
 * queue.c
 *
 * As provided by Daniel W. Lewis in "Fundamentals of Embedded Software with
 * the ARM Cortex-M3" Second Edition, Pearson Higher Education, Inc. 2013
 *
 * Disabling and enabling of interrupts is performed outside of the functions
 * and removed from code in this file.
 */

#include "global.h"
#include "queue.h"

#include <stdlib.h>

QUEUE* QUEUE_Create( uint32_t bfrsize )
{
    uint32_t bytes;
    QUEUE *q;

    bytes = ( sizeof( QUEUE ) - 1 ) + bfrsize;
    q = ( QUEUE *)malloc( bytes );

    if( q != NULL )
    {
        q->nq = q->dq = q->count = 0;
        q->size = bfrsize;
    }

    return q;
}

bool QUEUE_Enqueue( QUEUE *q, uint8_t data )
{
    bool full;

    full = q->count == q->size;
    if( !full )
    {
        q->bfr[ q->nq ] = data;
        if( ++q->nq == q->size )
        {
            q->nq = 0;
        }
        q->count++;
    }

    return !full;
}

bool QUEUE_Dequeue( QUEUE *q, uint8_t *ptr2data )
{
    bool empty;

    empty = q->count == 0;
    if( !empty )
    {
        *ptr2data = q->bfr[ q->dq ];
        if( ++q->dq == q->size )
        {
            q->dq = 0;
        }
        q->count--;
    }

    return !empty;
}
