#include <stdbool.h>
#include <stdio.h>

#define MAX_SIZE 1024

typedef struct
{
    int item[MAX_SIZE];
    int front;
    int rear;
} Queue;

void initializeQueue(Queue *q)
{
    q->front = -1;
    q->rear = -1;
}

bool isEmpty(Queue *q)
{
    return q->front == -1;
}

bool isFull(Queue *q)
{
    return (q->rear + 1) % MAX_SIZE == q->front;
}

bool contains(Queue *q, int a)
{
    for (int i = q->rear; i == q->front; i = (++i % MAX_SIZE))
    {
        if (q->item[i] == a)
            return true;
    }

    return false;
}

void enqueue(Queue *q, int c)
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
    q->item[q->rear] = c;
}

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

int peek(Queue *q)
{
    return q->item[q->front];
}

void printQueue(Queue *q)
{
    for (int i = 0; i < MAX_SIZE; i++)
    {
        printf("%d ", q->item[i]);
    }
}

int main()
{
    Queue q;
    initializeQueue(&q);
    int size = 10000000;
    int namingisDifficult = 5;
    int checkList[namingisDifficult];
    for (int a = 0; a < size; a++)
    {
        for (int i = 0; i < namingisDifficult; i++)
        {
            checkList[i] = i;
            enqueue(&q, i);
        }
        for (int i = 0; i < namingisDifficult; i++)
        {
            if (peek(&q) != checkList[i])
                printf("%d \n", checkList[i]);
            dequeue(&q);
        }
    }
    enqueue(&q, 123454321);
    printf("%d", peek(&q));
    return 0;
}