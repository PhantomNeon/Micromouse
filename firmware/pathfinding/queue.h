#include <stdbool.h>
#include "cell.h"

#define MAX_SIZE 1024

typedef struct
{
    Cell item[MAX_SIZE];
    int front;
    int rear;
} Queue;

void initializeQueue(Queue *q);
bool isEmpty(Queue *q);
bool isFull(Queue *q);
bool contains(Queue *q, Cell cell);
void enqueue(Queue *q, Cell cell);
void dequeue(Queue *q);
Cell peek(Queue *q);
void printQueue(Queue *q);