#pragma once

#include "../MiniSQL.h"
//#include "../RecordManager/Record.h"
//#include "../IndexManager/IndexManager.h"
#include "../CatalogManager/CatalogManager.h"

using namespace std;

//	创建表时的内部调用
void API_Create_Table(Table table);

//	删除表时的内部调用
void API_Drop_Table(string table_name);

//	创建索引时的内部调用
void API_Create_Index(Index index);

//	删除索引时的内部调用
void API_Drop_Index(string index_name);

//	读取表信息
void Read_Table_Info();

//	读取索引信息
void Read_Index(string index_name);

//	插入纪录时的内部调用
bool API_Insert();

//	选择纪录时的内部调用
bool API_Select();


//	删除纪录时的内部调用
bool API_Delete();






