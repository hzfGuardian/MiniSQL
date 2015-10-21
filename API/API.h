#pragma once

#include "../MiniSQL.h"
#include "../RecordManager/Record.h"
#include "../IndexManager/IndexManager.h"
#include "../CatalogManager/CatalogManager.h"

using namespace std;

//	创建表时的内部调用
bool API_Create_Table(Table& table)
{
    //if create table succeed
    if (Create_table(table)) {
        cout << RIGHT_INFO << endl;
    }
    else {
        cout << ERROR_DROP_TABLE_PHRASE << "'" << table.table_name << "'" << endl;
    }
}

//	删除表时的内部调用
bool API_Drop_Table(string table_name)
{
    //if drop table succeed
    if (Drop_table(table_name)) {
        cout << RIGHT_INFO << endl;
    }
    else {
        cout << ERROR_DROP_TABLE_PHRASE << "'" << table_name << "'" << endl;
    }
}

//	创建索引时的内部调用
bool API_Create_Index(Index index)
{
    if (Create_index(index)) {
        //real create index(call IndexManager)
        
    }
    else {
        cout << ERROR_CREATE_INDEX_PHRASE << endl;
    }
}

//	删除索引时的内部调用
bool API_Drop_Index(string index_name)
{
    //if drop table succeed
    if (Drop_index(index_name)) {
        cout << RIGHT_INFO << endl;
    }
    else {
        cout << ERROR_INDEX_NOT_EXIST << endl;
    }
}

//	读取表信息
void Read_Table_Info()
{
    
}

//	读取索引信息
void Read_Index(string index_name)
{
    
}

//	插入纪录时的内部调用
bool API_Insert()
{
    
}

//	选择纪录时的内部调用
bool API_Select()
{
    
}

//	删除纪录时的内部调用
bool API_Delete()
{
    
}






