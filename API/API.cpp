
#include "API.h"
#include "../RecordManager/RecordManager.h"
#include "../CatalogManager/CatalogManager.h"

using namespace std;

//	创建表时的内部调用
void API_Create_Table(Table& table)
{
    //if create table succeed
    if (Create_table(table)) {
        //get primary key
        int id = table.getPrimaryKeyId();
        //automatically create index
        string index_name = table.table_name + "_" + table.attrs[id].attr_name;
        
        //Create_index(index_name, table.attrs[id].attr_type);

        printf("Query OK, 0 rows affected.\n");
    }
    else {
        printf("error: Table '%s' already exists\n", table.table_name.c_str());
    }   
}

//	删除表时的内部调用
void API_Drop_Table(string table_name)
{
    //if drop table succeed
    if (Drop_table(table_name)) {
        printf("Query OK, 0 rows affected.\n");
    }
    else {
        printf("error: Unknown table '%s'\n", table_name.c_str());
    }

}

//	创建索引时的内部调用
void API_Create_Index(Index& index)
{

    if (Create_index(index)) {
        //real create index(call IndexManager)
        
    }
    else {

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
    //先补全所有CHAR类型的尾字节
    for (int i = 0; i < tuple.attr_count; ++i)
    {
        //调整char型数据，不足的末尾补全空格，超出的截断
        if (tuple.attrs[i].attr_type == CHAR)
        {          
            int default_len = tuple.attrs[i].attr_len;
            int input_len = tuple.attr_values[i].length();

            //长度不足，补全空格
            if (input_len < default_len)
            {
                for (int j = 0; j < default_len - input_len; ++j)
                    tuple.attr_values[j] += " ";
            }
            //长度超出，截断
            else if(input_len > default_len)
            {
                tuple.attr_values[j] = tuple.attr_values[j].substr(0, default_len);
            }
        }
    }

    //预检查，处理含有索引项
    for (int i = 0; i < tuple.attr_count; ++i)
    {
        //检查每个属性，primary的去B+树查看
        if (tuple.attrs[i].attr_key_type == PRIMARY)
        {
            //获取primary key的索引名
            string index_name = Find_index_name(tuple.table_name, tuple.attrs[i].attr_name);
            //搜索有无重名
            vector<int> v = Find_indices(index_name, "=", tuple.attr_values[i]);
            //若重名
            if (!v.empty())
            {
                printf("error: Duplicate entry '%s' for key 'PRIMARY'\n", tuple.attr_values[i]);
                return;
            }
        }
        else if (tuple.attrs[i].attr_key_type == UNIQUE)
        {
            //unqiue属性如果存在索引，去B+树查看，然后设置为not unique(EMPTY)
            if (Find_index_name(tuple.table_name, tuple.attrs[i].attr_name) != "")
            {
                //去B+树查

                tuple.attrs[i].attr_key_type == EMPTY;
            }
        }
    }
    //insert成功
    if (Insert(tuple)) {
        printf("Query OK, 1 row affected.\n");
    }
    else {
        printf("error: \n");
    }
}

//	选择纪录时的内部调用
void API_Select(string table_name, Condition_list clist)
{
    //create iterator
    Condition_list::iterator it;

    //divide all conditions into two parts: have index or not have index
    Condition_list have_index_list, no_index_list;

    //traversal condition list
    for (it = clist.begin(); it != clist.end(); ++it)
    {
        //check whether this attr has index
        if (Find_index_name(table_name, it->attr_name) == "")
            no_index_list.push_back(*it);
        else
            have_index_list.push_back(*it);
    }

    //check have_index_list
    vector<int> v_d, v_a, v_b;
    vector<int>::iterator my_Iter;
    
    string index_name;
    
    //we always do this: vd <- va intersects vb, va <- vd
    it = have_index_list.begin();
    v_d = Find_indices(index_name, it->op_type, it->cmp_value);

    for (; it != have_index_list.end(); ++it) {      
        index_name = Find_index_name(table_name, it->attr_name);       
        v_b = Find_indices(index_name, it->op_type, it->cmp_value);
        v_a = v_d;
        //intersection
        v_d.resize(v_a.size()+v_b.size());
        my_Iter = set_intersection(v_a.begin(), v_a.end(), v_b.begin(), v_b.end(), v_d.begin());
        v_d.resize(my_Iter - v_d.begin());
    }

    //final have index result stores in vector<int> v_d
    Select(table_name, v_d, no_index_list);
}

//	删除纪录时的内部调用
void API_Delete(string table_name, Condition_list clist)
{
    //create iterator
    Condition_list::iterator it;

    //divide all conditions into two parts: have index or not have index
    Condition_list have_index_list, no_index_list;

    //traversal condition list
    for (it = clist.begin(); it != clist.end(); ++it)
    {
        //check whether this attr has index
        if (Find_index_name(table_name, it->attr_name) == "")
            no_index_list.push_back(*it);
        else
            have_index_list.push_back(*it);
    }

    //check have_index_list
    vector<int> v_d, v_a, v_b;
    vector<int>::iterator my_Iter;
    
    string index_name;
    
    //we always do this: vd <- va intersects vb, va <- vd
    it = have_index_list.begin();
    v_d = Find_indices(index_name, it->op_type, it->cmp_value);

    for (; it != have_index_list.end(); ++it) {      
        index_name = Find_index_name(table_name, it->attr_name);       
        v_b = Find_indices(index_name, it->op_type, it->cmp_value);
        v_a = v_d;
        //intersection
        v_d.resize(v_a.size()+v_b.size());
        my_Iter = set_intersection(v_a.begin(), v_a.end(), v_b.begin(), v_b.end(), v_d.begin());
        v_d.resize(my_Iter - v_d.begin());
    }

    //final have index result stores in vector<int> v_d
    Delete(table_name, v_d, no_index_list);
}





