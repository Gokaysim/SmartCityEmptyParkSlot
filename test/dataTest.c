#include <stdio.h>
#include "../data.h"
#include <stdlib.h>

//To run
//gcc dataTest.c ../data.c


void test1();

int main()
{
    test1();
}

void test1()
{
    struct DataStructure* dataStructure =initDataStructure();

    addToTail(dataStructure,100,10,1);
    addToTail(dataStructure,200,10,0);
    addToTail(dataStructure,100,11,1);
    addToTail(dataStructure,200,11,0);
    addToTail(dataStructure,100,12,1);
    addToTail(dataStructure,200,12,0);

    int count = getCount(dataStructure);

    if(count!=3)
    {
        printf("Error 1\n");
    }    

    free(dataStructure);
}



