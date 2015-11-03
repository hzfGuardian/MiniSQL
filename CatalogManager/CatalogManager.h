#ifndef _CatalogManager_h_
#define _CatalogManager_h_

#include "../MiniSQL.h"
using namespace std;


bool Create_table(Table table);

bool Create_index(Index index);

bool Drop_table(string table_name);

bool Drop_index(string index_name);

bool Judge_attr_primary_unique(string table_name, string attr_name);

/**********************************************/

bool Judge_table_exist(string table_name);

bool Judge_index_exist(string index_name);

bool Judge_attr_exist(string table_name, string attr_name);

/*******************************************/
Table Read_Table_Info(string table_name);

string Find_index_name(string table_name,string attr_name);



#endif

