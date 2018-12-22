

void DataLayer_initStorage();

void DataLayer_addOrReplaceRecords(const char * input);

void DataLayer_addOwnRecord(int isEmpty);

int DataLayer_isDataNeedToBeSend();

char * DataLayer_getDataToSend();
