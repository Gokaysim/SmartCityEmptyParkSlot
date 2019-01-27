#include <stdio.h>
#include "../dataLayer.h"

#include "../helpers.h"
#include <stdlib.h>

void case1();
//To run
//gcc dataLayerTest.c ../dataLayer.c ../data.c ../helpers.c
int main ()
{
  case1();
}

void case1(){
  DataLayer_initStorage();

  DataLayer_addOrReplaceRecords("Empty:1,100,2,200,3,300");
  DataLayer_addOrReplaceRecords("Nonempty:2,200,3,300");

  if(DataLayer_isDataNeedToBeSend())
  {    
    char * t = DataLayer_getDataToSend();

    if(strCmp(t,"Empty:1,100;Nonempty:2,200,3,300")!=0)
    {
      printf("Error at case1\n");
    }
  }

  DataLayer_destroyStorage();
}
