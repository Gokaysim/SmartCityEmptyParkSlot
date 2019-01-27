#include "helpers.h"
#include "base.h"
#include <stdlib.h>


unsigned int convert(char *st) {
  char *x;
  for (x = st ; *x ; x++) {
    if (!isdigit(*x))
      return 0L;
  }
  return (strtoul(st, 0L, 10));
}

int split (char *str, char c, char ***arr)
{
    int count = 1;
    int token_len = 1;
    int i = 0;
    char *p = str;
    char *t;

    while (*p != '\0')
    {
        if (*p == c)
            count++;
        p++;
    }

    *arr = (char**) malloc(sizeof(char*) * count);
    if (*arr == NULL)
        exit(1);

    p = str;
    while (*p != '\0')
    {
        if (*p == c)
        {
            (*arr)[i] = (char*) malloc( sizeof(char) * token_len );
            if ((*arr)[i] == NULL)
                exit(1);

            token_len = 0;
            i++;
        }
        p++;
        token_len++;
    }
    (*arr)[i] = (char*) malloc( sizeof(char) * token_len );
    if ((*arr)[i] == NULL)
        exit(1);

    i = 0;
    p = str;
    t = ((*arr)[i]);
    while (*p != '\0')
    {
        if (*p != c && *p != '\0')
        {
            *t = *p;
            t++;
        }
        else
        {
            *t = '\0';
            i++;
            t = ((*arr)[i]);
        }
        p++;
    }

    return count;
}

int splitToArrays(char *str,int **arr,unsigned int ** arr2)
{
  char ** tempCharArray= NULL;

  int count = split(str,',',&tempCharArray);
  *arr = (int*) malloc(sizeof(int) * count/2);
  *arr2 = (unsigned int *) malloc(sizeof(unsigned int) * count/2);
  if (*arr == NULL)
      exit(1);
  int i;
  for(i =0 ;i<count;i++)
  {
    if(i%2==0)
    {
        (*arr)[i/2] = atoi(tempCharArray[i]);
    }
    else{
        (*arr2)[i/2] = convert(tempCharArray[i]);
    }
  }
  free(tempCharArray);

  return count/2;
}

int strCmp(char string1[], char string2[])
{
    int i = 0;
    int flag = 0;
    while (flag == 0)
    {
        if (string1[i] > string2[i])
        {
            flag = 1;
        }
        else if (string1[i] < string2[i])
        {
            flag = -1;
        }

        if (string1[i] == '\0')
        {
            break;
        }

        i++;
    }
    return flag;
}

int getIntDigit(int i)
{
  int digit = 1;

  while(1)
  {
    i=i/10;
    if(i==0)
    {
      break;
    }
    digit++;
  }
  return digit;
}

int getULDigit(unsigned long i)
{
  int digit = 1;

  while(1)
  {
    i=i/10;
    if(i==0)
    {
      break;
    }
    digit++;
  }
  return digit;
}

char * getString(int nodeId,unsigned int timeStamp)
{
  int nodeIdDigit = getIntDigit(nodeId);
  int timeStampDigit = getIntDigit(timeStamp);
  int totalLenght = nodeIdDigit + timeStampDigit+1;
  char * array = malloc(sizeof(char)*totalLenght);
  array[nodeIdDigit]=',';
  array[totalLenght]='\0';

  while(timeStampDigit>0)
  {
    timeStampDigit --;
    int digit = timeStamp%10;
    timeStamp = timeStamp/10;
    char c = digit+'0';
    array[nodeIdDigit+1+timeStampDigit] = c;
  }
  while(nodeIdDigit>0)
  {
    nodeIdDigit --;

    int digit = nodeId%10;
    nodeId = nodeId/10;
    array[nodeIdDigit] = digit+'0';
  }
  return array;
}

