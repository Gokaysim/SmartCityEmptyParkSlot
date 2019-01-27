#include "dataLayer.h"
#include "base.h"
#include "data.h"
#include "helpers.h"
#include <string.h>
#include <stdlib.h>


static struct DataStructure * dataStructure;
void DataLayer_initStorage()
{
  dataStructure = initDataStructure();  
}

void DataLayer_addOwnRecord(int isEmpty,int node_id,unsigned int timeStamp){  
  addToHead(dataStructure,timeStamp,node_id,isEmpty);
}

void DataLayer_addOrReplaceRecords(char * input)
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
        unsigned  * nodeTimeStamp = NULL;

        int nodeIdCount = splitToArrays(fieldSplit[1], &nodeIdArray,&nodeTimeStamp);
        int j = 0;
        for(j=0;j<nodeIdCount;j++)
        {
          addToTail(dataStructure,nodeTimeStamp[j],nodeIdArray[j],1);
        }

        free(nodeIdArray);
        free(nodeTimeStamp);
      }
      else if(strCmp(fieldSplit[0],"Nonempty")==0)
      {
        int * nodeIdArray = NULL;
        int * nodeTimeStamp = NULL;

        int nodeIdCount = splitToArrays(fieldSplit[1], &nodeIdArray,&nodeTimeStamp);
        int j = 0;
        for(j=0;j<nodeIdCount;j++)
        {
          addToTail(dataStructure,nodeTimeStamp[j],nodeIdArray[j],0);
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
  int count = getCount(dataStructure);

  if(count>5)
  {
    return 0;
  }
  return 1;
}

char * DataLayer_getDataToSend()
{
  char *empty = (char *)malloc(sizeof(char)*2000);
  strcat(empty,"Empty:");
  char *notEmpty = (char *)malloc(sizeof(char)*2000);
  strcat(notEmpty,"Nonempty:");
  //
  int count = getCount(dataStructure);
  int reduceCounter = (int)(count /3);
  if(reduceCounter<5)
  {
    reduceCounter=5;
  }
  struct DataNode * head  = getNFirstElement(dataStructure,reduceCounter);

  struct DataNode * current = head;
  reduceCounter--;
  int firstEmpty=1,firstNonempty=1;
  while(reduceCounter>0)
  {
    int nodeId = current->nodeId;
    int timeStamp = getTimestamp(dataStructure,nodeId);

    if(current->isEmpty==1)
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
    strcat(strcat(empty,";"),notEmpty);    
    free(notEmpty);
    return empty;
  }
  else if(firstEmpty ==0)
  {

    free(notEmpty);
    return empty;
  }
  else if (firstNonempty==0)
  {
    free(empty);
    return notEmpty;
  }
  else{
    return"";
  }

}

void DataLayer_destroyStorage()
{
  free(dataStructure);
}
