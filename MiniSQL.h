#pragma once

#include <list>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdarg>

using namespace std;

//allow user input 500 characters once in a line
#define MAX_ONE_LINE 256

//prompt
#define WELCOME     "Welcome to the minisql system. Hope you have a good day."
#define PROMPT      "\nminisql> "
#define ENTER_SIGN  "      -> "
#define BYE_BYE     "Good Bye. Fuck you next time."




//出错信息宏定义
//	以下是程序中用到的出错信息定义
#define ERROR_WRONG_CMD_INPUT 			"Wrong command input !"
#define ERROR_EMPTY_CMD_INPUT 			"Empty input !"
#define ERROR_NULL_OP_TYPE 					"No such operate type !"
#define ERROR_CANNOT_OPEN_FILE			"Cannot open the shell file !"
#define ERROR_NOTFULL_CMD						"Not full command in file !"
#define ERROR_NO_FILENAME_INPUT			"No file name input !"
#define ERROR_TABLE_EXIST						"Table already exists !"
#define ERROR_TABLE_NOT_EXIST				"Table not exists, you cannot drop it !"
#define ERROR_NULL_CREATE_TYPE			"Can only create table or index !"
#define ERROR_EMPTY_CREATE_TYPE			"Empty create type input !"
#define ERROR_CREATE_TABLE_PHRASE		"Create table phrase error !"
#define ERROR_EMPTY_DROP_TYPE				"Empty drop type input !"
#define ERROR_NULL_DROP_TYPE				"Can only create table or index !"
#define ERROR_DROP_TABLE_PHRASE			"Error: Unknown table "
#define ERROR_CREATE_INDEX_PHRASE		"Create index phrase error !"
#define ERROR_INDEX_EXIST						"Index already exists !"
#define ERROR_INDEX_NO_TABLE				"Table not exists, create it first !"
#define ERROR_INDEX_NO_ATTR					"Attr no exists in table, cannot create the index !"
#define ERROR_DROP_INDEX_PHRASE			"Drop index phrase error !"
#define ERROR_INDEX_NOT_EXIST				"Index not exists, you cannot drop it !"
#define ERROR_INSERT_PHRASE					"Insert phrase error !"
#define ERROR_INSERT_NO_TABLE				"Table not exists, cannot insert a record !"
#define ERROR_SELECT_PHRASE					"Select phrase error !"
#define ERROR_SELECT_NO_TABLE				"Table not exists, cannot select records !"
#define ERROR_DELETE_PHRASE					"Delete phrase error !"
#define ERROR_DELETE_NO_TABLE				"Table not exists, cannot delete records !"

#define ERROR_CHAR_INDEX_LESS_ZERO      "Error: length of attribute must be positive. "

#define RIGHT_INFO                      "Query OK."

//	用于描述表中一个属性在表中的具体信息
struct Attribute
{
    string attr_name;
    int attr_type;	//属性的数据类型，分别为CHAR, FLOAT, INT
    int attr_key_type;//属性的主键类型，分别为PRIMARY, UNIQUE, NULL
    int attr_len; 	//属性所存放的数据的长度，如果不是CHAR，则长度标记为1
    int attr_id;    //属性在表中是第几个
};

//	用于描述表的信息
struct Table
{
    string table_name;  //表名
    int attr_count;				//表中属性的总个数
    Attribute attrs[32];	//表的所有属性列表, 最多32个属性
};

//	用于描述判断条件的信息
struct Condition
{
    string attr_name;	//条件所对应的属性名
    int op_type;		//条件所用到的比较模式，分别为SMALL,EQUEL,BIG
    string cmp_value;	//条件所需要进行比较的值
};
typedef list<Condition> Condition_list;

//	用于描述索引信息
struct Index
{
    string index_name;
    string table_name;
    string attr_name;	//	索引所对应的属性
};





