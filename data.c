#include "data.h"
#include "base.h"
#define MaxHashSize 100

static unsigned long * hashTable =NULL;
static struct DataStructure * dataStructure;
void initData()
{
  dataStructure = (struct DataStructure *) malloc(sizeof(struct DataStructure));
  dataStructure->count=0;
  dataStructure->head=NULL;
  dataStructure->tail=NULL;

  hashTable =(unsigned long *) malloc(sizeof(unsigned long)*MaxHashSize);

  int i = 0;
  for(i = 0;i<MaxHashSize;i++)
  {
    hashTable[i] = 0;
  }
}

void addToTail(unsigned long timeStamp,int nodeId,int isEmpty)
{
  unsigned long lastTimeStamp = hashTable[nodeId%MaxHashSize];  
  if(lastTimeStamp>timeStamp)
  {
    return;
  }
  setTimestamp(nodeId,timeStamp);

  if(dataStructure->head==NULL)
  {
    dataStructure->head = (struct Node*) malloc(sizeof(struct Node));
    dataStructure->tail= dataStructure->head;

    dataStructure->head->nodeId = nodeId;
    dataStructure->head->isEmpty = isEmpty;
    dataStructure->head->next = NULL;
    dataStructure->head->previous = NULL;
    dataStructure->count = 1;

  }
  else{

    struct Node* current = dataStructure->head;
    while(1)
    {

      if(current->nodeId==nodeId)
      {
        current->isEmpty=isEmpty;
        break;
      }
      else if(current->next==NULL)
      {
        //TODO assignment may deruduce CPU cycle usage
        current->next= (struct Node*)malloc(sizeof(struct Node));
        current->next->previous = current;
        current->next->next = NULL;
        current->next->nodeId = nodeId;
        current->next->isEmpty = isEmpty;

        dataStructure->tail = current->next;
        dataStructure->count++;

        break;
      }

      current = current->next;
    }
  }
}

//TODO no check is needed for time being but it will be considered later
void addToHead(unsigned long timeStamp,int nodeId,int isEmpty)
{
  dataStructure->count--;
  struct Node * previousHead = dataStructure->head;
  dataStructure->head = (struct Node*) malloc(sizeof(struct Node));
  dataStructure->head->nodeId = nodeId;
  dataStructure->head->isEmpty = isEmpty;
  dataStructure->head->next = previousHead;
  dataStructure->head->previous = NULL;

  setTimestamp(nodeId,timeStamp);
  if(previousHead == NULL)
  {
    dataStructure->tail= dataStructure->head;
  }
}

int getCount()
{
  return dataStructure->count;
}
struct Node * getNFirstElement(int n)
{
  int reduceCounter;
  if(dataStructure->count<n)
  {
    reduceCounter = dataStructure->count;
  }
  else{
    reduceCounter = n;
  }

  struct Node * head = dataStructure->head;
  struct Node * current = dataStructure->head;
  reduceCounter--;
  while(reduceCounter>0)
  {
    current = current->next;
    reduceCounter--;
  }

  dataStructure->head = current->next;
  current->next = NULL;

  if(dataStructure->head == NULL)
  {
    dataStructure->tail = dataStructure->head;
  }
  else{
    dataStructure->head->previous=NULL;
  }

  dataStructure->count -=n;

  return head;
}

unsigned long getTimestamp(int nodeId)
{
  return hashTable[nodeId%MaxHashSize];
}
void setTimestamp(int nodeId,unsigned long timeStamp)
{
  hashTable[nodeId%MaxHashSize]=timeStamp;
}
