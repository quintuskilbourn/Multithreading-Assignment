#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

//Quintus Kilbourn 54871935
//Rohit Shyla Kumar 54581876


//error handling
//comments
//pthread_exit (try reduce number of global var by pthreadjoin)
//refactor code
//upload

using namespace std;

#define QLEN 50
#define SSLEEP 2

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;


int servTok = 0;
int queueLen = 0;
int totFetch = 0;

class MyQueue{
  private:
    int front,size;
    int tokens[QLEN];

  public:
    MyQueue();
    bool isEmpty();
    bool isFull();
    bool EnQueue(int token);
    int DeQueue();
};

MyQueue::MyQueue(){
  front = 0;
  size = 0;
}

bool MyQueue::isEmpty(){
  return (size==0);
}

bool MyQueue::isFull(){
  return (size==QLEN);
}

int MyQueue::DeQueue(){
  int retVal;
  if(!isEmpty()){
    retVal = tokens[front];
    front = (front+1)%QLEN;
    size--;
    return retVal;
  }else{
    return -1;
  }
}

bool MyQueue::EnQueue(int token){
  if(!isFull()){
    tokens[(front+size)%QLEN] = token;
    size++;
    return 1;
  }else{
    return 0;
  }
}

void *serve(void *args){

  MyQueue** temp = (MyQueue**)args;
  MyQueue* queue = temp[1];
  int** boohoo = (int**)args;
  int maxC = *(boohoo[2]);
  static int fetched = 0;
    srand(time(NULL));
    while(servTok<maxC){
      int ran = rand()%20+1;
      if(pthread_mutex_lock(&mtx)){
        cout<<"Mutex Lock Error"<<endl;
        exit(-1);
      }
      if(ran >= queueLen){
        fetched = queueLen;
        while(!(queue->isEmpty())){
          queue->DeQueue();
          totFetch++;
          queueLen--;
          servTok++;
          if(servTok>=maxC){
            break;
          }
        }
      }else{
        fetched = ran;
        for(int i = 0; i<ran; i++){
          if(queue->DeQueue()==-1){
            cout<<"DeQueue not successfull"<<endl;
            exit(-1);
          }
          totFetch++;
          queueLen--;
          servTok++;
          if(servTok>=maxC){
            break;
          }
        }
      }
      cout<<"\t\t\t\t\t"<<queueLen<<"\t\t"<<fetched<<"\t\t"<<fetched<<endl;
      if(pthread_mutex_unlock(&mtx)){
        cout<<"Mutex Unlock Error"<<endl;
        exit(-1);
      }
      usleep(2000000);
    }
    return (void *) 1;
}

void *flow(void *args){

  MyQueue** temp = (MyQueue**)args;
  static int count = 0;
  MyQueue* queue = temp[1];
  int** boohoo = (int**)args;
  int maxC = *(boohoo[2]);
  int flowInt = *(boohoo[0]);
  srand(time(NULL));
  while(servTok<maxC){
    int ran = rand()%10+1;
    int placed = 0;
    //mutex
    if(pthread_mutex_lock(&mtx)){
      cout<<"Mutex Lock Error"<<endl;
      exit(-1);
    }
    for(int i = 0; i<ran;i++){
      if(!(queue->isFull())){

        queue->EnQueue(count);
        count++;
        queueLen++;
        placed++;
      }else{
        servTok++;
        if(servTok>=maxC){
          break;
        }
      }
    }
    cout<<placed<<"\t\t"<<count-1<<"\t\t\t"<<queueLen<<endl;
    if(pthread_mutex_unlock(&mtx)){
      cout<<"Mutex Unlock Error"<<endl;
      exit(-1);
    }
    usleep(flowInt*1000000);
  }
  return (void *) 1;
}

int main(int argc, char* argv[]){
  int maxC = strtol(argv[1],NULL,10);
  int flowInt = strtol(argv[2],NULL,10);
  pthread_t flowId, servId;
  MyQueue* queue = new MyQueue();
    // cout<<"in main1 "<< queue->isEmpty()<<endl;
    // cout<<queue->EnQueue(2)<<endl;
    //   cout<<"in main2 "<< queue->isEmpty()<<endl;
    // cout<<queue->DeQueue()<<endl;
    // cout<<"in main3 "<< queue->isEmpty()<<endl;

  void* args[] = {&flowInt, queue, &maxC};


  cout<<"The Max Token is "<< maxC << " and the interval time for the flow is "<< flowInt<<endl;
  cout<<"Flow\t\t\t\t\tMyQueue\t\tServer"<<endl;
  cout<<"Token Added\tLatest Sequence Num\tCurrent Length\tToken Fetched\tTotal Token Fetched"<<endl;


  if(pthread_create(&flowId,NULL,flow,args)){
    cout<<"Failed to create flowr thread"<<endl;
    exit(-1);                                         //check on exit code x2
  }

  if(pthread_create(&servId,NULL,serve,args)){
    cout<<"Failed to create flowr thread"<<endl;
    exit(-1);
  }

  pthread_join(flowId,NULL);
  pthread_join(servId,NULL); //add some shit

cout<<"The total number of token that have been generated by the flow is " << servTok + queueLen << endl;
cout << "The total number of token that have been feteched by the server is " << totFetch << endl;
cout << "The total number of token that have been dropped by the queue is " << maxC-totFetch<< endl;

//

  return 0;
}
