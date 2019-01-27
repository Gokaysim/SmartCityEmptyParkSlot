struct DataNode{
  int nodeId;
  int isEmpty;
  struct DataNode*  next;
  struct DataNode* previous;
};

struct DataStructure{
  struct DataNode * head;
  struct DataNode * tail;
  int count;
  unsigned int * hashTable;
};

struct DataStructure* initDataStructure();

void addToTail(struct DataStructure * dataStructure, unsigned int timeStamp,int nodeId,int isEmpty);
void addToHead(struct DataStructure * dataStructure, unsigned int timeStamp,int nodeId,int isEmpty);
int getCount(struct DataStructure * dataStructure);
struct DataNode * getNFirstElement(struct DataStructure * dataStructure,int N);

unsigned int getTimestamp(struct DataStructure * dataStructure,int nodeId);
void setTimestamp(struct DataStructure * dataStructure,int nodeId,unsigned int timeStamp);
