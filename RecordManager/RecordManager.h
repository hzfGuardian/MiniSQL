#ifndef _RecordManager_H_
#define _RecordManager_H_

#include "../BufferManager/BufferManager.h"
#include "../MiniSQL.h"

bool Insert(Tuple& tuple);
char* Convertvalue(Tuple &tuple); 

#endif
