#include <stdbool.h>
#include <stdio.h>
#include "cell.c"

#define MAX_SIZE 1024

typedef struct
{
    Cell item[MAX_SIZE];
    int front;
    int rear;
} Queue;

void initializeQueue(Queue *q)
{
    q->front = -1;
    q->rear = 0;
}

// Function to check if the queue is empty
bool isEmpty(Queue *q)
{
    return (q->front == q->rear - 1);
}

// Function to check if the queue is full
bool isFull(Queue *q)
{
    return (q->rear == MAX_SIZE);
}

bool contains(Queue *q, Cell cell)
{
    for(int i = 0; i < q->front + 1; i++)
    {
        if(q->item[i].x == cell.x && q->item[i].y == cell.y)
            return true;
    }

    return false;
}

void enqueue(Queue *q, Cell cell)
{
    if (isFull(q))
    {
        return;
    }
    q->item[q->rear] = cell;
    q->rear++;
}

// Function to remove an element from the queue (Dequeue operation)
void dequeue(Queue *q)
{
    if (isEmpty(q))
    {
        return;
    }
    q->front++;
}

// Function to get the element at the front of the queue (Peek operation)
Cell peek(Queue *q)
{
    if (isEmpty(q))
    {
        Cell a;
        return a; // return some default value or handle
                   // error differently
    }
    return q->item[q->front + 1];
}

// Function to print the current queue
void printQueue(Queue *q)
{
    if (isEmpty(q))
    {
        fprintf(stderr, "Queue is empty\n");
        fflush(stderr);
        return;
    }

    fprintf(stderr, "Current Queue: ");
    fflush(stderr);
    for (int i = q->front + 1; i < q->rear; i++)
    {
        fprintf(stderr, "(%d, %d)", q->item[i].x, q->item[i].y);
        fflush(stderr);
    }
    fprintf(stderr, "\n\n");
    fflush(stderr);
}