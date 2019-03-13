/*
CS3103 Assginment 2
Completed by:
Quintus Kilbourn (54871935)
Rohit Shyla Kumar (54581876)
File Name - MyQueue.h
Description - Declaration of standard FIFO queue class and functions
*/

#define QLEN 50
class MyQueue {
private:
	int front, size;			//stores the index of the front of the queue and the size of the queue respectively 
	int tokens[QLEN];			//stores the actual elements of the queue sequentially 

public:
	MyQueue();
	bool isEmpty();		//check if the queue is empty
	bool isFull();		//check if the queue is full
	bool EnQueue(int token);	//Adds a token to the queue
	int DeQueue();				//Removes the token at the front of the queue
};
