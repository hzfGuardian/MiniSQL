

#include "API.h"

using namespace std;

//	创建表时的内部调用
void API_Create_Table(Table& table)
{
    //if create table succeed

    if (Create_table(table)) {
        cout << RIGHT_INFO << endl;
    }
    else {
        //cout << ERROR_DROP_TABLE_PHRASE << "'" << table.table_name << "'" << endl;
    }
    
}

//	删除表时的内部调用
void API_Drop_Table(string& table_name)
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
void API_Create_Index(Index& index)
{

    if (Create_index(index)) {
        //real create index(call IndexManager)
        
    }
    else {
        cout << ERROR_CREATE_INDEX_PHRASE << endl;
    }

}

//	删除索引时的内部调用
void API_Drop_Index(string& index_name)
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
void Read_Table_Info(Table& table)
{
    string tableInfo = table.table_name + "_table.info";   
    ifstream fin;   
    fin.open(tableInfo.c_str(), ios::in);   
    table.attr_count = 0;   
    //读表的信息   
    char attr_temp[256];   
    string attr;   
    while(!fin.eof())   
    {   
        fin.getline(attr_temp, 256);   
        attr = attr_temp;   
        if(attr == "" && fin.eof())   
            break;   
           
        table.attrs[table.attr_count].attr_name = Get_Word(attr, 1);   
        parse_to_int(Get_Word(attr, 2), table.attrs[table.attr_count].attr_type);   
        parse_to_int(Get_Word(attr, 3), table.attrs[table.attr_count].attr_key_type);   
        parse_to_int(Get_Word(attr, 4), table.attrs[table.attr_count].attr_len);   
        parse_to_int(Get_Word(attr, 5), table.attrs[table.attr_count].attr_num);   
           
        table.attr_count++;   
    }   
    fin.close(); 
}

//	读取索引信息
void Read_Index(string& index_name)
{
    
}

//	插入纪录时的内部调用
bool API_Insert()
{
    return false;
}

//	选择纪录时的内部调用
bool API_Select()
{
    return false;
}

//	删除纪录时的内部调用
bool API_Delete()
{
    return false;
}






