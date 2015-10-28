
#include "API.h"
#include "../CatalogManager/CatalogManager.h"

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
void API_Drop_Table(string table_name)
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
void API_Drop_Index(string index_name)
{
    //if drop table succeed

    if (Drop_index(index_name)) {
        cout << RIGHT_INFO << endl;
    }
    else {
        cout << ERROR_INDEX_NOT_EXIST << endl;
    }
}


//	插入纪录时的内部调用
void API_Insert(Tuple& tuple)
{
    printf("Call insert.\n");
}

//	选择纪录时的内部调用
void API_Select(string table_name, Condition_list clist)
{
    //get all indices on this table
    vector<string> index_list;// = Read_Index_Info(table_name);
    Condition_list::iterator it;

    //divide all conditions into two parts: have index or not have index
    Condition_list have_index_list, no_index_list;
    for (it = clist.begin(); it != clist.end(); ++it)
    {
        //check whether this attr has index
        for (int i = 0; i < index_list.size(); ++i)
        {
            if (index_list[i] == it->attr_name)
                have_index_list.push_back(*it);
            else
                no_index_list.push_back(*it);
        }
    }

    //map: first->key, second->count
    map<int, int> hash_join_map;
    map<int, int>::iterator my_Itr;
    for (it = have_index_list.begin(); it != have_index_list.end(); ++it)
    {
        //get offsets
        vector<int> offsets;// = Find_indices(index_name, it->op_type, it->cmp_value);
        //hash join
        for (int i = 0; i < offsets.size(); ++i)
        {
            //if not found
            if (hash_join_map.find(offsets[i]) == hash_join_map.end())
                hash_join_map[offsets[i]] = 1;
            else
                ++hash_join_map[offsets[i]];
        }
    }

    for (my_Itr = hash_join_map.begin(); my_Itr != hash_join_map.end(); ++my_Itr) {
        if (my_Itr->second > 1)
        {
            
        }
    }
}

//	删除纪录时的内部调用
void API_Delete(string table_name, Condition_list clist)
{
    
}






