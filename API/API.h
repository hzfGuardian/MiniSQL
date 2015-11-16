#pragma once

#include "../MiniSQL.h"
#include "../RecordManager/RecordManager.h"
#include "../CatalogManager/CatalogManager.h"
#include "../IndexManager/IndexManager.h"
#include <ctime>

using namespace std;

//	创建表时的内部调用
void API_Create_Table(Table& table);

//	删除表时的内部调用
void API_Drop_Table(string table_name);

//	创建索引时的内部调用
void API_Create_Index(Index& index);

//	删除索引时的内部调用
void API_Drop_Index(string index_name);

//	插入纪录时的内部调用
void API_Insert(Tuple& tuple);

//	选择纪录时的内部调用
void API_Select(string table_name, Condition_list clist);

//	删除纪录时的内部调用
void API_Delete(string table_name, Condition_list clist);

// 字符串分割函数
string split(string str, string pattern, int id);

vector<string> split(string str, string pattern);

//画表函数
void API_Draw_result(Table& tbl);

//测时函数
double calculate_time(long start, long end);
