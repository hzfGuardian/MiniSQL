#ifndef _CatalogManager_h_
#define _CatalogManager_h_

#include<stdio.h>
#include<fstream>
#include<iostream>
using namespace std;

struct Attr{
	string attr_name;
	int attr_type;
	int attr_key_type;
	int attr_len;
	int attr_id;
};

struct Table{
	string name;
	Attr attrs[32];
	int attr_num;
};

struct Index{
	string index_name;
	string table_name;
	string attr_name;	
};

bool Create_table(Table& table);

bool Create_index(Index& index);

bool Drop_table(string table_name);

bool Drop_index(string index_name);

bool Judge_attr_primary_unique(string table_name, string attr_name);

/**********************************************/

bool Judge_table_exist(string table_name);

bool Judge_index_exist(string index_name);

bool Judge_attr_exist(string table_name, string attr_name);
#endif

