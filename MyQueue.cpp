/*
CS3103 Assginment 2
Completed by: 
Quintus Kilbourn (54871935)
Rohit Shyla Kumar (54581876)
File Name - MyQueue.cpp
Description - Implementation of standard FIFO queue functions
*/

#include "MyQueue.h"

//Constructor performs initialization when a new queue is instantiated 
MyQueue::MyQueue() {
	front = 0;				
	size = 0;				
}

//We use inline functions to optimize small processes
inline bool MyQueue::isEmpty() {
	return (size == 0);		//if the queue has size 0, it is considered to be empty
}

inline bool MyQueue::isFull() {
	return (size == QLEN);	//if the queue has the maximum permitted elements, it is considered full
}

bool MyQueue::EnQueue(int token) {
	//Cannot add a token to a full queue
	if (!isFull()) {
		tokens[(front + size) % QLEN] = token;		//  %QLEN allows us to use empty spaces at the front of the queue making this a circular queue
		size++;
		return 1;									//Operation was successful 
	}
	else {
		return 0;									//Operation failed
	}
}

int MyQueue::DeQueue() {
	int retVal;
	//Cannot remove a token from an empty queue
	if (!isEmpty()) {
		retVal = tokens[front];
		front = (front + 1) % QLEN;					//So the front wraps around the queue and may never go out of bounds of the array
		size--;
		return retVal;								//Return the sequence number of the token at the fron of the queue
	}
	else {
		return -1;									//Operation failed. We return -1 instead of 0 as a token may have sequence number 0 and other functions could misinterpret this result
	}
}


