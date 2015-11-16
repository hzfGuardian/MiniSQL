
#include "API.h"

using namespace std;

extern string delete_values[32]; 
extern vector<string> select_values;

extern int delete_num;
extern int select_num;

extern vector<int> select_offsets;



/**************************************************************************************************/

//	创建表时的内部调用
void API_Create_Table(Table& table)
{
    //if create table succeed
    if (Create_table(table)) {
        
        //get primary key
        int id = table.getPrimaryKeyId();
        
        //automatically create index
        if (id != -1) {
            
            string index_name = table.table_name + "_" + table.attrs[id].attr_name;
            
            Index idx;
            idx.table_name = table.table_name;
            idx.attr_name = table.attrs[id].attr_name;
            idx.index_name = index_name;
            
            //如果索引名重复
            while (idx.index_name == Find_index_name(table.table_name, table.attrs[id].attr_name)) {
                //在前面加下划线
                idx.index_name = "_" + idx.index_name;
            }
            
            //catalog create index
            Create_index(idx);
            
            //index manager create index
            Create_index(idx.index_name + "_index.rec", table.attrs[id].attr_type - INT);
        }
        
        
        printf("Query OK, 0 rows affected\n");
    }
    else {
        printf("error: Table '%s' already exists\n", table.table_name.c_str());
    }   
}

//	删除表时的内部调用
void API_Drop_Table(string table_name)
{
    //寻找这个table上建立的所有索引
    Table tbl = Read_Table_Info(table_name);
    if (tbl.attr_count == 0) {
        printf("error: Unknown table '%s'\n", table_name.c_str());
        return;
    }
    
    for (int i = 0; i < tbl.attr_count; i++) {
        string index_name = Find_index_name(table_name, tbl.attrs[i].attr_name);
        //若该属性上存在索引
        if (index_name != "") {
            Drop_index(index_name);
        }
    }
    
    //if drop table succeed
    if (Drop_table(table_name)) {
        printf("Query OK, 0 rows affected.\n");
    }
    else {
        printf("error: Table '%s' does not exists", table_name.c_str());
    }
}







/**************************************************************************************************/

//	创建索引时的内部调用
void API_Create_Index(Index& index)
{
    if (Create_index(index)) {
        //real create index(call IndexManager)

        //first select
        vector<int> offsetlist; //null
        Condition_list list;    //null
        
        //select * from table_name;
        Select_tuple(offsetlist, Read_Table_Info(index.table_name), list);
        
        //获取表信息
        Table tbl = Read_Table_Info(index.table_name);
        
        //获取建立索引属性的id
        int id = tbl.searchAttrId(index.attr_name);
        
        vector<Value_offset> values;
        
        //vector<string> select_values, '\t'
        //extern vector<int> select_offsets
        
        //对每一行，获取索引对应属性值+offset
        for (int i = 0; i < select_values.size(); i++) {
            
            Value_offset vo;
            vo.s = split(select_values[i], "\t", id);
            vo.off = select_offsets[i];
            
            values.push_back(vo);
        }
        Create_index(values, index.index_name + "_index.rec", tbl.attrs[id].attr_type - INT);
        
        printf("Query OK, 0 rows affected\n");
    }
    
}

//	删除索引时的内部调用
void API_Drop_Index(string index_name)
{
    //if drop table succeed
    //catalog drop index
    if (Drop_index(index_name)) {
        printf("Query OK. 0 rows affected\n");
    }
    else {
        printf("error: Index '%s' not exists\n", index_name.c_str());
    }
}







/**************************************************************************************************/

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
            vector<int> v = Find_indices(index_name+"_index.rec", "=", tuple.attr_values[i]);
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
                vector<int> v = Find_indices(index_name+"_index.rec", "=", tuple.attr_values[i]);
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
                Insert_index(index_name+"_index.rec", tuple.attr_values[i], offset);
            }
        }        
        
        printf("Query OK, 1 row affected\n");
    }
    else {
        printf("error: Duplicate unique attribute exists\n");
    }
}








/**************************************************************************************************/

//	选择纪录时的内部调用
void API_Select(string table_name, Condition_list clist)
{
    
    //create iterator
    Condition_list::iterator it;

    //divide all conditions into two parts: have index or not have index
    Condition_list have_index_list, no_index_list;

    Table tbl = Read_Table_Info(table_name);
    
    //记得补全char结尾的空格
    //先补全所有Condition_list里面的CHAR类型attr_name的尾字节 tbl
    for (it = clist.begin(); it != clist.end(); ++it)
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
    
    //条件列表没有索引
    if (have_index_list.empty())
    {
        //Select_tuple(vector<int> offsetlist, Table table, Condition_list list)
        select_values.clear();
        select_num = 0;
        Select_tuple(v_d, tbl, clist);
        if (select_num == 0) {
            printf("Empty set\n");
            return;
        }
        else {
            API_Draw_result(tbl);
            printf("%d rows in set\n", select_num);
        }
        return;
    }

    //we always do this: vd <- va intersects vb, va <- vd
    it = have_index_list.begin();
    index_name = Find_index_name(table_name, it->attr_name);
    v_d = Find_indices(index_name+"_index.rec", it->op_type, it->cmp_value);
    
    //取完交集后一定要排序才能正确去重
    sort(v_d.begin(), v_d.end());
    v_d.erase( unique(v_d.begin(), v_d.end() ), v_d.end() );
    
    for (++it; it != have_index_list.end(); ++it) {
        index_name = Find_index_name(table_name, it->attr_name);       
        v_b = Find_indices(index_name+"_index.rec", it->op_type, it->cmp_value);
        v_a = v_d;
        //intersection
        sort(v_a.begin(), v_a.end());
        sort(v_b.begin(), v_b.end());

        v_d.resize(v_a.size()+v_b.size());
        my_Iter = set_intersection(v_a.begin(), v_a.end(), v_b.begin(), v_b.end(), v_d.begin());
        v_d.resize(my_Iter - v_d.begin());
        //去除空格
        sort(v_d.begin(), v_d.end());
        v_d.erase( unique(v_d.begin(), v_d.end() ), v_d.end() );
    }
    
    //作完交集后若v_d为空，直接返回
    if (v_d.empty()) {
        printf("Empty set\n");
        return;
    }
    
    //final have index result stores in vector<int> v_d
    //Select_tuple(vector<int> offsetlist, Table table, Condition_list list)
    select_values.clear();
    select_num = 0;
    Select_tuple(v_d, tbl, clist);
    
    if (select_num == 0) {
        printf("Empty set\n");
        return;
    }
    else {
        API_Draw_result(tbl);
        printf("%d rows in set\n", select_num);
    }
}









/**************************************************************************************************/


//	删除纪录时的内部调用
void API_Delete(string table_name, Condition_list clist)
{
    
    //create iterator
    Condition_list::iterator it;
    
    //divide all conditions into two parts: have index or not have index
    Condition_list have_index_list, no_index_list;
    
    
    Table tbl = Read_Table_Info(table_name);
    
    //先补全所有Condition_list里面的CHAR类型attr_name的尾字节 tbl
    for (it = clist.begin(); it != clist.end(); ++it)
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
    
    //条件列表没有索引, 同时处理了条件列表为空的情况, 因为此时no_index_list也为空, 筛选所有记录
    if (have_index_list.empty())
    {
        //extern string delete_values[32];
        
        //清空delete_values
        for (int i = 0; i < 32; i++) {
            delete_values[i] = "";
        }
        delete_num = 0;
        
        //调用RecordManager删除记录
        Delete_tuple(v_d, tbl, clist);
        
        //反馈删除的行数
        printf("Query OK, %d rows affected\n", delete_num);
        
        //若未删除任何记录，直接返回，不用处理index
        if (delete_num == 0) {
            return;
        }
        
        //前往删除相应索引值
        goto Final_Delete;
        
    }
    
    //we always do this: vd <- va intersects vb, va <- vd
    it = have_index_list.begin();
    index_name = Find_index_name(table_name, it->attr_name);
    v_d = Find_indices(index_name+"_index.rec", it->op_type, it->cmp_value);
    
    //取完交集后一定要排序才能正确去重
    sort(v_d.begin(), v_d.end());
    v_d.erase( unique(v_d.begin(), v_d.end() ), v_d.end() );
    
    for (; it != have_index_list.end(); ++it) {
        index_name = Find_index_name(table_name, it->attr_name);
        v_b = Find_indices(index_name+"_index.rec", it->op_type, it->cmp_value);
        v_a = v_d;
        //intersection
        sort(v_a.begin(), v_a.end());
        sort(v_b.begin(), v_b.end());
        
        v_d.resize(v_a.size()+v_b.size());
        my_Iter = set_intersection(v_a.begin(), v_a.end(), v_b.begin(), v_b.end(), v_d.begin());
        v_d.resize(my_Iter - v_d.begin());
        
        //去除重复
        //取完交集后一定要排序才能正确去重
        sort(v_d.begin(), v_d.end());
        v_d.erase( unique(v_d.begin(), v_d.end() ), v_d.end() );
    }
    
    //作完交集后若v_d为空，说明没有符合条件的记录，直接返回
    if (v_d.empty()) {
        printf("Empty set\n");
        return;
    }
    
    //final have index result stores in vector<int> v_d
    
    //先清空delete_values
    for (int i = 0; i < 32; i++) {
        delete_values[i] = "";
    }
    delete_num = 0;
    
    //调用RecordManager删除记录操作
    Delete_tuple(v_d, tbl, clist);
    
    //反馈删除行数
    printf("Query OK, %d rows affected \n", delete_num);
    
    //若未删除任何记录，直接返回，不用处理index
    if (delete_num == 0) {
        printf("Empty set\n");
        return;
    }

Final_Delete:
    //遍历该表的attribute，找value，找index_name，delete
    for (int i = 0; i < tbl.attr_count; i++) {
        
        //找条件列表中含索引项的属性的value和索引名
        
        //index_name
        string index_name = Find_index_name(table_name, tbl.attrs[i].attr_name);

        //先确定属性有无索引, 无索引则直接进入下一循环
        if (index_name == "") {
            continue;
        }
        
        //取出delete_values的第i列，即为要B+树删除的value
        vector<string> vs = split(delete_values[i], "\t");
        
        //将该列的value逐一删除
        for (int j = 0; j < vs.size(); j++) {
            Delete_index(index_name+"_index.rec", vs[j]);
        }
        
    }
    
}





/**************************************************************************************************/
//字符串分割函数
vector<string> split(string str, string pattern)
{
    string::size_type pos;
    vector<string> result;
    str+=pattern;//扩展字符串以方便操作
    int size = str.size();
    
    for(int i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            std::string s=str.substr(i,pos-i);
            result.push_back(s);
            i = pos+pattern.size() - 1;
        }
    }
    return result;
}

string split(string str, string pattern, int id)
{
    vector<string> result = split(str, pattern);
    return result[id];
}

//extern vector<string> select_values;
//extern int select_num;

void API_Draw_result(Table& tbl)
{
    //确定列宽, 比该列字符串多两个字符
    int col_width[32];
    
    //initialize为2
    for (int i = 0; i < 32; i++) {
        col_width[i] = 0;
    }
    
    //扫描每一行每一列，调整列宽
    for (vector<string>::iterator it = select_values.begin(); it != select_values.end(); it++) {
        
        //每一行取出attr_count个字符串, 计算长度
        vector<string> vs = split(*it, "\t");
        for (int i = 0; i < tbl.attr_count; i++) {
            //调整列宽
            if (vs[i].length() > col_width[i]) {
                col_width[i] = vs[i].length();
            }
        }
    }
    //对属性名长度调整列宽
    for (int i = 0; i < tbl.attr_count; i++) {
        //调整列宽
        if (tbl.attrs[i].attr_name.length() > col_width[i]) {
            col_width[i] = tbl.attrs[i].attr_name.length();
        }
    }
    
    //最长字符串长度+2作为列宽
    for (int i = 0; i < tbl.attr_count; i++) {
        col_width[i] += 2;
    }
    
    //每行先画"+-----+-------+-------+", 再画"| id  | value | price |"
    
    //先确定分割线的字符串div_line
    string div_line = "+";
    for (int i = 0; i < tbl.attr_count; i++) {
        //
        for (int j = 0; j < col_width[i]; j++) {
            div_line += "-";
        }
        div_line += "+";
    }
    
    //先画分割线，属性名，分割线
    printf("%s\n", div_line.c_str());
    
    for (int i = 0; i < tbl.attr_count; i++) {
        
        //先画"| "
        printf("| ");
        
        //输出属性名
        cout << tbl.attrs[i].attr_name;
        
        //输出足够的填补空格
        for (int j = tbl.attrs[i].attr_name.length() + 1; j < col_width[i]; j++) {
            printf(" ");
        }

    }
    printf("|\n");

    printf("%s\n", div_line.c_str());
    
    //按行，画属性值
    for (vector<string>::iterator it = select_values.begin(); it != select_values.end(); it++) {
        
        //分割字符串
        vector<string> vs = split(*it, "\t");
        
        for (int i = 0; i < tbl.attr_count; i++) {
            
            //先画"| "
            printf("| ");
            
            //输出属性名
            printf("%s", vs[i].c_str());
            
            //输出足够的填补空格
            for (int j = vs[i].length() + 1; j < col_width[i]; j++) {
                printf(" ");
            }

        }
        printf("|\n");
    }
    
    printf("%s\n", div_line.c_str());
}

//测时函数
double calculate_time(long start, long end)
{
    return (double)(end - start)/CLK_TCK;
}


