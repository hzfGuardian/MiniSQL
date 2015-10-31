#ifndef _RecordManager_H_
#define _RecordManager_H_

#include "../BufferManager/BufferManager.h"
#include "../MiniSQL.h"


int Insert(Tuple &tuple);

char* Convertvalue(Tuple &tuple); 


bool Delete_tuple(vector<int> offsetlist, Table table, Condition_list list);


bool Select_tuple(vector<int> offsetlist, Table table, Condition_list list);


#endif
