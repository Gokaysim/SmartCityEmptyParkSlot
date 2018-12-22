


struct Node{
  int nodeId;
  int isEmpty;
  struct Node*  next;
  struct Node* previous;
};

struct DataStructure{
  struct Node * head;
  struct Node * tail;
  int count;
};

void initData();

void addToTail(unsigned long timeStamps,int nodeId,int isEmpty);
void addToHead(unsigned long timeStamps,int nodeId,int isEmpty);
int getCount();
struct Node * getNFirstElement(int N);
unsigned long getTimestamp(int nodeId);
