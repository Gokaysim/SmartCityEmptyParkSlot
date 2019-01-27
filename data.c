#include "data.h"
#include "base.h"
#include <stdlib.h>
#define MaxHashSize 100

void setTimestamp(struct DataStructure * dataStructure,int nodeId,unsigned int timeStamp);

struct DataStructure* initDataStructure()
{
  struct DataStructure * dataStructure = (struct DataStructure *) malloc(sizeof(struct DataStructure));
  dataStructure->count=0;
  dataStructure->head=NULL;
  dataStructure->tail=NULL;
  dataStructure->hashTable =(int *) malloc(sizeof(int)*MaxHashSize);

  int i = 0;
  for(i = 0;i<MaxHashSize;i++)
  {
    dataStructure->hashTable[i] = 0;
  }
  return dataStructure;
}

void addToTail(struct DataStructure * dataStructure,unsigned int timeStamp,int nodeId,int isEmpty)
{
  unsigned  int lastTimeStamp = getTimestamp(dataStructure,nodeId);
  if(lastTimeStamp>timeStamp)
  {
    return;
  }
  setTimestamp(dataStructure,nodeId,timeStamp);

  if(dataStructure->head==NULL)
  {
    dataStructure->head = (struct DataNode*) malloc(sizeof(struct DataNode));
    dataStructure->tail= dataStructure->head;

    dataStructure->head->nodeId = nodeId;
    dataStructure->head->isEmpty = isEmpty;
    dataStructure->head->next = NULL;
    dataStructure->head->previous = NULL;
    dataStructure->count = 1;

  }
  else{

    struct DataNode* current = dataStructure->head;
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
        current->next = (struct DataNode*)malloc(sizeof(struct DataNode));
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
void addToHead(struct DataStructure * dataStructure,unsigned int timeStamp,int nodeId,int isEmpty)
{
  dataStructure->count--;
  struct DataNode * previousHead = dataStructure->head;
  dataStructure->head = (struct DataNode*) malloc(sizeof(struct DataNode));
  dataStructure->head->nodeId = nodeId;
  dataStructure->head->isEmpty = isEmpty;
  dataStructure->head->next = previousHead;
  dataStructure->head->previous = NULL;

  setTimestamp(dataStructure,nodeId,timeStamp);
  if(previousHead == NULL)
  {
    dataStructure->tail= dataStructure->head;
  }
}

int getCount(struct DataStructure * dataStructure)
{
  return dataStructure->count;
}

struct DataNode* getNFirstElement(struct DataStructure * dataStructure,int n)
{
  int reduceCounter;
  if(dataStructure->count<n)
  {
    reduceCounter = dataStructure->count;
  }
  else{
    reduceCounter = n;
  }

  struct DataNode * head = dataStructure->head;
  struct DataNode * current = dataStructure->head;
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

unsigned int getTimestamp(struct DataStructure * dataStructure,int nodeId)
{
  return dataStructure->hashTable[nodeId%MaxHashSize];
}

void setTimestamp(struct DataStructure * dataStructure,int nodeId,unsigned int timeStamp)
{
  dataStructure->hashTable[nodeId%MaxHashSize]=timeStamp;
}
