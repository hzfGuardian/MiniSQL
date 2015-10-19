#pragma once

#include <iostream>
#include <list>
#include <string>
#include <fstream>

using namespace std;

//	以下是程序中所用到的结构体定义

//	用于描述表中一个属性在表中的具体信息
struct Attribute
{
    string attr_name;
    int attr_type;				//属性的数据类型，分别为ATTR_TYPE_CHAR, ATTR_TYPE_FLOAT, ATTR_TYPE_INT
    int attr_key_type;		//属性的主键类型，分别为ATTR_TYPE_PRIMARY, ATTR_TYPE_UNIQUE, ATTR_TYPE_NULL
    int attr_len; 				//属性所存放的数据的长度，如果不是ATTR_TYPE_CHAR，则长度标记为1
    //unsigned int attr_start;		//属性在一条记录中的起始位置（从1开始计数）
    int attr_id;				//属性在表中是第几个
};

//	用于描述表的信息
struct Table
{
    string table_name;  //表名
    int attr_count;				//表中属性的总个数
    Attribute attrs[32];	//表的所有属性列表
};

//	用于描述判断条件的信息
typedef list<Condition> Condition_list;
struct Condition
{
    string attr_name;			//条件所对应的属性名
    int op_type;					//条件所用到的比较模式，分别为SMALL,EQUEL,BIG
    string cmp_value;			//条件所需要进行比较的值
    //Condition* next;	//指向下一个条件
};

//	用于描述索引信息
struct Index
{
    string index_name;
    string table_name;
    string attr_name;					//	索引所对应的属性
};
/********************************************************************************/




