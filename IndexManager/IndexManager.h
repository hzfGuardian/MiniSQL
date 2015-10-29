#pragma once

#include "../MiniSQL.h"
using namespace std;

//automatically create index on primary key
void Create_index(string index_name, int attr_type);

//CREATE INDEX NAME ON NAME '(' NAME ')'
void Create_index(vector<> values, string index_name, int attr_type);

//
void Insert_index(string index_name, string value, int offset);

//
void Delete_index(string index_name, string value);

//int Find_index(string index_name, string value);

//
vector<int> Find_indices(string index_name, string op, string value);

