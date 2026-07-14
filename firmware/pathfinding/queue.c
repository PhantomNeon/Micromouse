#include "queue.h"

void initializeQueue(Queue *q)
{
    q->front = -1;
    q->rear = -1;
}

// Function to check if the queue is empty
bool isEmpty(Queue *q)
{
    return q->front == -1;
}

// Function to check if the queue is full
bool isFull(Queue *q)
{
    return (q->rear + 1) % MAX_SIZE == q->front;
}

bool contains(Queue *q, Cell cell)
{
    if(isEmpty(q))
    {
        return false;
    }
    
    for (int i = q->rear; i == q->front; i = (++i % MAX_SIZE))
    {
        if (q->item[i].x == cell.x && q->item[i].y == cell.y)
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
    if (q->front == -1)
    {
        q->front = 0;
    }
    q->rear = (q->rear + 1) % MAX_SIZE;
    q->item[q->rear] = cell;
}

// Function to remove an element from the queue (Dequeue operation)
void dequeue(Queue *q)
{
    if (isEmpty(q))
    {
        return;
    }

    if (q->front == q->rear)
    {
        q->front = q->rear = -1;
    }
    else
    {
        q->front = (q->front + 1) % MAX_SIZE;
    }
}

// Function to get the element at the front of the queue (Peek operation)
Cell peek(Queue *q)
{
    if (isEmpty(q))
    {
        Cell a = {0, 0, 0, 0, -1, 0};
        return a; // return some default value or handle
                  // error differently
    }
    return q->item[q->front];
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
    for (int i = q->rear; i == q->front; i = (++i % MAX_SIZE))
    {
        fprintf(stderr, "(%d, %d)", q->item[i].x, q->item[i].y);
        fflush(stderr);
    }
    fprintf(stderr, "\n\n");
    fflush(stderr);
}