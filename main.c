#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <Windows.h>
#include <signal.h>


// --------------------------------------------------------C program for array implementation of queue




struct Queue {
/**
* A FIFO data structure that will be used with semaphore.
* Source : https://www.geeksforgeeks.org/queue-set-1introduction-and-array-implementation/
*/

    int front, rear, size;
    unsigned capacity;
    int* array;
};

/**
* Methods for FIFO data structure
*/
struct Queue* createQueue(unsigned capacity)
{
/**
* Constructor method.
*/

    struct Queue* queue = (struct Queue*)malloc(
        sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;

    // This is important, see the enqueue
    queue->rear = capacity - 1;
    queue->array = (int*)malloc(
        queue->capacity * sizeof(int));
    return queue;
}

// Queue is full when size becomes
// equal to the capacity
int isFull(struct Queue* queue)
{
/**
* Check if the data structure is full.
*/
    return (queue->size == queue->capacity);
}

// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{
/**
* Check if the data structure is empty.
*/
    return (queue->size == 0);
}

// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct Queue* queue, int item)
{
/**
* Add new value end of the line.
*/
    if (isFull(queue))
        return;
    queue->rear = (queue->rear + 1)
                  % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    //printf("%d enqueued to queue\n", item);
}

// Function to remove an item from queue.
// It changes front and size
int dequeue(struct Queue* queue)
{
/**
* Remove value from first of the line.
*/
    if (isEmpty(queue))
        return INT_MIN;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1)
                   % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

// Function to get front of queue
int front(struct Queue* queue)
{
/**
* Peek the value first in line.
*/
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}

// Function to get rear of queue
int rear(struct Queue* queue)
{
/**
* Peek the value last in line.
*/
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->rear];
}

// --------------------------------------------------------------------------------Semaphore structure-----------------------------

// A structure to represent a semaphore
struct semaphore
{
    volatile int count;
    struct Queue* ReadyQueue  ;
    struct Queue* BlockQueue   ;

};

void semWait(struct semaphore s)
{
    int blockedprocess;
    int newprocess;
    s.count=s.count--;

    if(s.count <0)
    {
        blockedprocess = dequeue(s.ReadyQueue);
        //newprocess       = dequeue(s.ReadyQueue);

        enqueue(s.BlockQueue,blockedprocess); // current process goes to blocked list

        printf(" placing process: %d in blocked list\n ", blockedprocess);
        //printf(" now running process %d \n ", newprocess);
    }


}

void semSignal(struct semaphore s)
{
    int removedprocess;
    int removecurrent;
    s.count=s.count++;
    if(s.count <=0)
    {

        removecurrent  = dequeue(s.ReadyQueue); // p3

        removedprocess = dequeue(s.BlockQueue); // Removing process from the block list
        enqueue(s.ReadyQueue, removedprocess ); // adding process to the ready list
        //enqueue(s.ReadyQueue, removecurrent); // adding p3 to the ready list



        printf(" remove the process: %d \n ", removedprocess );
        printf(" process: %d on ready list \n ", removedprocess);
    }


}


void process1 (struct semaphore s)
{

     printf("Philosopher A and C are eating\n");
     printf("Philosopher B, D and E are Thinking\n");
     Sleep(3000);

     semWait(s);


}

void process2 (struct semaphore s)
{


     printf("Philosopher B and D are eating\n");
     printf("Philosopher A, C and E are Thinking\n");
     Sleep(3000);

     semWait(s);


}

void process3 (struct semaphore s)
{


     printf("Philosopher C and E are eating\n");
     printf("Philosopher A, B and D are Thinking\n");

     Sleep(3000);

     semSignal(s);



}


// ----------------------Global declaration

int globalsem=0;



//-----------------------------------------------------------------------------------Driver----------------------------------------------------
int main()
{



// initialize the semaphore parameters and process list
// we have 5 philosophers: A, B, C, D, E
// we will pair them in way that 2 non-neighbor philosopher will eat while 3 others will think.
// each pair could be considered as a single process.
//  p1: A C
// p2: B D
// p3: E C

struct semaphore s;

    s.ReadyQueue = createQueue(5) ;
    s.BlockQueue  = createQueue(5) ;
    s.count = 1;  // initialize semaphore

    enqueue(s.ReadyQueue,1);
    enqueue(s.ReadyQueue,2);
    enqueue(s.ReadyQueue,3);




    while(1)
    {

        // check the ready list and run process using FIFO

        int process = front(s.ReadyQueue); // check who is first in line



        switch(process)
        {
        case 1:
            process1(s);
            printf("----\n");

        case 2:
            process2(s);
            printf("----\n");

        case 3:
            process3(s);
            printf("----\n");



        }



    }











   // semWait(s);

   // printf("dequeue from ReadyQueue: %d \n", dequeue(s.ReadyQueue));
   // printf("dequeue from BlockQueue: %d \n", dequeue(s.ReadyQueue));
















    return 0;
}
