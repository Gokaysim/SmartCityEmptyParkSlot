#include "dataLayer.h"
#include "base.h"
#include "data.h"
#include "rtimer.h"
#include "sys/node-id.h"

void DataLayer_initStorage()
{
  initData();
  // PRINTF("Digit : %d",)
  unsigned long ul =  123;
}

void DataLayer_addOwnRecord(int isEmpty){
  unsigned long timeStamp = clock_seconds();
  addToHead(timeStamp,node_id,isEmpty);
}

void DataLayer_addOrReplaceRecords(const char * input)
{
  char **arr = NULL;

  int c = split(input, ';', &arr);

  int i;
  for (i = 0; i < c; i++)
  {
      char ** fieldSplit = NULL;
      int fieldSplitCount = split(arr[i], ':', &fieldSplit);

      if(fieldSplitCount!=2)
      {
        PRINTF("Error: wrong input format!!!");
      }

      if(strCmp(fieldSplit[0],"Empty")==0)
      {
        int * nodeIdArray = NULL;
        unsigned long * nodeTimeStamp = NULL;

        int nodeIdCount = splitToArrays(fieldSplit[1], &nodeIdArray,&nodeTimeStamp);
        int j = 0;
        for(j=0;j<nodeIdCount;j++)
        {
          addToTail(nodeTimeStamp[j],nodeIdArray[j],1);
        }

        free(nodeIdArray);
        free(nodeTimeStamp);
      }
      else if(strCmp(fieldSplit[0],"Nonempty")==0)
      {
        int * nodeIdArray = NULL;
        unsigned long * nodeTimeStamp = NULL;

        int nodeIdCount = splitToArrays(fieldSplit[1], &nodeIdArray,&nodeTimeStamp);
        int j = 0;
        for(j=0;j<nodeIdCount;j++)
        {
          addToTail(nodeTimeStamp[j],nodeIdArray[j],0);
        }

        free(nodeIdArray);
        free(nodeTimeStamp);
      }

      free(fieldSplit);
  }
  free(arr);
}

int DataLayer_isDataNeedToBeSend()
{
  int count = getCount();

  if(count>5)
  {
    return 0;
  }
  return 1;
}

char * DataLayer_getDataToSend()
{
  char empty[2000]="Empty:";
  char notEmpty[2000]="Nonempty:";
  //
  int count = getCount();
  int reduceCounter = (int)(count /3);
  if(reduceCounter<5)
  {
    reduceCounter=5;
  }
  struct Node * head  = getNFirstElement(reduceCounter);

  struct Node * current = head;
  reduceCounter--;
  int firstEmpty=1,firstNonempty=1;
  while(reduceCounter>0)
  {
    int nodeId = current->nodeId;
    unsigned long timeStamp = getTimestamp(nodeId);

    if(current->isEmpty==0)
    {
      if(firstEmpty==0)
      {
        strcat(empty,",");
      }
      strcat(empty,getString(nodeId,timeStamp));

      firstEmpty=0;
    }
    else{
      if(firstNonempty==0)
      {
        strcat(notEmpty,",");
      }
      strcat(notEmpty,getString(nodeId,timeStamp));

      firstNonempty=0;
    }


    if(current->next == NULL)
    {
      break;
    }
    else{
      current = current->next;
    }
    reduceCounter--;
  }

  if(firstEmpty==0&&firstNonempty==0)
  {
    return strcat(strcat(empty,";"),notEmpty);
  }
  else if(firstEmpty ==0)
  {
    return empty;
  }
  else if (firstNonempty==0)
  {
    return notEmpty;
  }
  else{
    return"";
  }

}
