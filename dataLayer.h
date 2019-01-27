void DataLayer_initStorage();

void DataLayer_addOrReplaceRecords(char * input);

void DataLayer_addOwnRecord(int isEmpty,int node_id,unsigned int timeStamp);

int DataLayer_isDataNeedToBeSend();

char * DataLayer_getDataToSend();

void DataLayer_destroyStorage();
