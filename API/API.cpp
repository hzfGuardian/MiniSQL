
#include "API.h"
#include "../RecordManager/RecordManager.h"
#include "../CatalogManager/CatalogManager.h"
#include "../IndexManager/IndexManager.h"

using namespace std;

extern string delete_values[32]; 
extern vector<string> select_values;

extern int delete_num;
extern int select_num;

//	创建表时的内部调用
void API_Create_Table(Table& table)
{
    //if create table succeed
    if (Create_table(table)) {
        //get primary key
        int id = table.getPrimaryKeyId();
        //automatically create index
        string index_name = table.table_name + "_" + table.attrs[id].attr_name + "_index";

        Index idx;
        idx.table_name = table.table_name;
        idx.attr_name = table.attrs[id].attr_name;
        idx.index_name = index_name;

        //如果索引名重复
        while (!Create_Index_File(idx.index_name)) {
            //在前面加下划线
            idx.index_name = "_" + idx.index_name;          
        }

        //catalog create index
        Create_index(idx);

        //index manager create index
        Create_index(idx.index_name, table.attrs[id].attr_type - INT);

        printf("Query OK, 0 rows affected\n");
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

        //first select
        vector<int> offsetlist; //null
        Condition_list list;    //null

        Select_tuple(offsetlist, Read_Table_Info(index.table_name), list);

        //Create_index(vector<Value_offset> values, string index_name, int attr_type);//------------------------------------------------------------------------///
    }
    
}

//	删除索引时的内部调用
void API_Drop_Index(string index_name)
{
    //if drop table succeed

    //catalog drop index
    if (Drop_index(index_name)) {
        printf("Query OK.\n");
    }
    else {
        printf("error: Index '%s' not exists\n", index_name.c_str());
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
                    tuple.attr_values[i] += " ";
            }
            //长度超出，截断
            else if(input_len > default_len)
            {
                tuple.attr_values[i] = tuple.attr_values[i].substr(0, default_len);
            }
        }
    }

    //预检查，处理含有索引项
    for (int i = 0; i < tuple.attr_count; ++i)
    {
        string index_name;
        //检查每个属性，primary的去B+树查看
        if (tuple.attrs[i].attr_key_type == PRIMARY)
        {
            //获取primary key的索引名
            index_name = Find_index_name(tuple.table_name, tuple.attrs[i].attr_name);
            //搜索有无重名
            vector<int> v = Find_indices(index_name, "=", tuple.attr_values[i]);
            //若重名
            if (!v.empty())
            {
                printf("error: Duplicate entry '%s' for key 'PRIMARY'\n", tuple.attr_values[i].c_str());
                return;
            }
        }
        else if (tuple.attrs[i].attr_key_type == UNIQUE)
        {
            //unqiue属性如果存在索引，去B+树查看，然后设置为not unique(EMPTY)
            index_name = Find_index_name(tuple.table_name, tuple.attrs[i].attr_name);
            if (index_name != "")
            {
                //搜索有无重名
                vector<int> v = Find_indices(index_name, "=", tuple.attr_values[i]);
                //若重名
                if (!v.empty())
                {
                    printf("error: Duplicate entry '%s' for key 'PRIMARY'\n", tuple.attr_values[i].c_str());
                    return;
                }
                tuple.attrs[i].attr_key_type = EMPTY;
            }
        }
    }
    //insert成功
    int offset = Insert(tuple);
    if (offset != -1) {

        //将插入记录中涉及到index的内容插入B+树
        for (int i = 0; i < tuple.attr_count; ++i)
        {
            string index_name = Find_index_name(tuple.table_name, tuple.attrs[i].attr_name);
            if (index_name != "")
            {
                //void Insert_index(string index_name, string value, int offset);
                Insert_index(index_name, tuple.attr_values[i], offset);
            }
        }        

        printf("Query OK, 1 row affected\n");
    }
    else {
        printf("error: Duplicate unique attribute exists\n");
    }
}

//	选择纪录时的内部调用
void API_Select(string table_name, Condition_list clist)
{
    //create iterator
    Condition_list::iterator it;

    //divide all conditions into two parts: have index or not have index
    Condition_list have_index_list, no_index_list;

    Table tbl = Read_Table_Info(table_name);

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
    
    if (have_index_list.empty())
    {
        //Select_tuple(vector<int> offsetlist, Table table, Condition_list list)
        Select_tuple(v_d, tbl, no_index_list);
        return;
    }

    //we always do this: vd <- va intersects vb, va <- vd
    it = have_index_list.begin();
    v_d = Find_indices(index_name, it->op_type, it->cmp_value);

    for (; it != have_index_list.end(); ++it) {      
        index_name = Find_index_name(table_name, it->attr_name);       
        v_b = Find_indices(index_name, it->op_type, it->cmp_value);
        v_a = v_d;
        //intersection
        sort(v_a.begin(), v_a.end());
        sort(v_b.begin(), v_b.end());

        v_d.resize(v_a.size()+v_b.size());
        my_Iter = set_intersection(v_a.begin(), v_a.end(), v_b.begin(), v_b.end(), v_d.begin());
        v_d.resize(my_Iter - v_d.begin());
    }

    //记得补全char结尾的空格----------------------------------------------------------------------------------------------------------------------------------------
    
    for (it = no_index_list.begin(); it != no_index_list.end(); ++it)
    {
        //attr and tablename
        int id = tbl.searchAttrId(it->attr_name);
        //调整char型数据，不足的末尾补全空格，超出的截断
        if (tbl.attrs[id].attr_type == CHAR)
        {          
            int default_len = tbl.attrs[id].attr_len;
            int input_len = it->cmp_value.length();

            //长度不足，补全空格
            if (input_len < default_len)
            {
                for (int j = 0; j < default_len - input_len; ++j)
                    it->cmp_value += " ";
            }
            //长度超出，截断
            else if(input_len > default_len)
            {
                it->cmp_value = it->cmp_value.substr(0, default_len);
            }
        }
    }

    //final have index result stores in vector<int> v_d
    //Select_tuple(vector<int> offsetlist, Table table, Condition_list list)
    Select_tuple(v_d, tbl, no_index_list);
}

//	删除纪录时的内部调用
void API_Delete(string table_name, Condition_list clist)
{
    //create iterator
    Condition_list::iterator it;

    //divide all conditions into two parts: have index or not have index
    Condition_list have_index_list, no_index_list;

    Table tbl = Read_Table_Info(table_name);

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

    //if table has no index    
    if (have_index_list.empty())
    {
        //Delete_tuple(vector<int> offsetlist, Table table, Condition_list list)
        Delete_tuple(v_d, tbl, no_index_list);
        return;
    }
   
    string index_name;
    //we always do this: vd <- va intersects vb, va <- vd
    it = have_index_list.begin();
    v_d = Find_indices(index_name, it->op_type, it->cmp_value);

    for (; it != have_index_list.end(); ++it) {      
        index_name = Find_index_name(table_name, it->attr_name);       
        v_b = Find_indices(index_name, it->op_type, it->cmp_value);
        v_a = v_d;

        //intersection
        sort(v_a.begin(), v_a.end());
        sort(v_b.begin(), v_b.end());

        v_d.resize(v_a.size()+v_b.size());
        my_Iter = set_intersection(v_a.begin(), v_a.end(), v_b.begin(), v_b.end(), v_d.begin());
        v_d.resize(my_Iter - v_d.begin());
    }

    //final have index result stores in vector<int> v_d 
    Delete_tuple(v_d, tbl, no_index_list);

    //have_index_list找value，找index_name，delete
    //Delete_index(string index_name, string value);
}





