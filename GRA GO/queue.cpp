/*
 * Program  : Queue data structure using array
 * Language : C
 */

#include<stdio.h>

 //size of the queue
#define size 5

int arr[size];

/*
 * intialize front and rear as 0
 * which indicates that the queue is empty initially.
 */
int front = 0;
int rear = 0;

/*
 * It will check whether the queue is empty or not
 * return 1, if the queue is empty
 * return -1, otherwise
 */
int isQueueEmpty()
{
    if (front == rear)
        return 1;
    return -1;
}

//removes the current beginning element from the queue.
void dequeue()
{
    if (isQueueEmpty() == 1)
        printf("Queue is Empty.\n");
    else
    {
        printf("Dequeued element = %d\n", arr[front]);
        front++;
    }
}

/*
 * It will check whether the queue if full or not
 * return 1, if the queue is full
 * return -1, otherwise
 */
int isQueueFull()
{
    if (rear == size)
        return 1;
    return -1;
}

//adds element at the end of the queue
void enqueue(int val)
{
    if (isQueueFull() == 1)
        printf("Queue is Full\n");
    else
    {
        arr[rear] = val;
        rear++;
    }
}


