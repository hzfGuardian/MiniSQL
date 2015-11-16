
#include "SQLGrammarTree.h"
#include "../MiniSQL.h"
#include "../API/API.h"

using namespace std;

NodeManager node_manager;

extern void yyrestart(FILE* fp);
extern FILE* yyin;


long start_t, end_t;
double running_time;

//以下为语法树外部函数

SQLGrammarTree* MallocNewNode()
{
    SQLGrammarTree* pNew = new SQLGrammarTree;
    node_manager.push_back(pNew);
    if (!pNew)
    {
        return pNew;
    }
    memset(pNew, 0, sizeof(*pNew));
    return pNew;
}

void ProcessTree(SQLGrammarTree* pNode)
{
    //start_t = clock();
    //printf("%ld\n", start_t);
    SQLGrammarTree* current_node = pNode->lpSub;
    switch (current_node->type) {

        case CREATE:                      
            //move to next node, "table" or "index"
            current_node = current_node->lpNext;
            switch (current_node->type) {
                //create table
                case TABLE:
                    ProcessCreateTable(current_node);
                    break;
                
                case INDEX: //CREATE INDEX NAME ON NAME '(' NAME ')'
                    ProcessCreateIndex(current_node);
                    break;
                    
                default:
                    break;
            }
            break;
        
        case DROP:
            //move to next node, "table" or "index"
            current_node = current_node->lpNext;
            
            switch (current_node->type) {
                case TABLE:                    
                    ProcessDropTable(current_node);
                    break;
                    
                case INDEX:
                    ProcessDropIndex(current_node);
                    break;
                    
                default:
                    break;
            }
            break;
        
        case INSERT: //INSERT INTO NAME VALUES '(' attr_value_list ')'           
            ProcessInsert(current_node);
            break;
        
        case DELETE:
            ProcessDelete(current_node);
            break;
        
        case SELECT:
            ProcessSelect(current_node);
            break;
        
        case EXECFILE:
            ProcessExecfile(current_node);
            break;

        default:
            break;
    }
    //end_t = clock();
    //printf("%ld\n", end_t);
    //printf("(done in %.2f sec)\n", calculate_time(start_t, end_t));
}

SQLGrammarTree* FatherAddSon(SQLGrammarTree* pFather, SQLGrammarTree* pSon)
{
    SQLGrammarTree* pFS = pFather->lpSub;
    if (!pFS)//no son
    {
        pFather->lpSub = pSon;
        return pFather;
    }
    else
    {
        while(pFS->lpNext) //add to neighbor
        {
            pFS = pFS->lpNext;
        }
        
        pFS->lpNext = pSon;
        return pFather;
    }
}

//add num_of_sons sons to a new father node and return father
SQLGrammarTree* NewFatherAddSon(int iType, int num_of_sons, ...)
{
    SQLGrammarTree* pFather = MallocNewNode();
    if (!pFather)
    {
        return pFather;
    }
    
    pFather->type = iType;
    
    va_list ap;
    va_start(ap, num_of_sons);
    
    while(num_of_sons--)
        FatherAddSon(pFather, va_arg(ap, SQLGrammarTree*));
    
    va_end(ap);
    
    return pFather;
}


void FreeTree(SQLGrammarTree* pNode)
{
    if (pNode->lpNext)  FreeTree(pNode->lpNext);
    if (pNode->lpSub)   FreeTree(pNode->lpSub);
    //real free
    delete pNode;
    pNode = NULL;
}


//clear
void nm_clear()
{
    //printf("Free: ");
    for (int i = 0; i < node_manager.size(); ++i)
    {
        //printf("%s ", node_manager[i]->text);
        delete node_manager[i];
        node_manager[i] = NULL;
    }
    //printf("\n");
    node_manager.clear();
}



//////////////////////////////////////////////////

void ProcessCreateTable(SQLGrammarTree* current_node)
{
    Table tbl;
    //primary key
    string primary_key_name = "";

    //initialize table
    tbl.attr_count = 0;
    
    //move to next node, "name"
    current_node = current_node->lpNext;

    //assign the name value
    tbl.table_name = string(current_node->text);
    
    //move to next node, "attribute_info"
    current_node = current_node->lpNext;
    
    //move to its son, "attribute_list"
    current_node = current_node->lpSub;

    //get next node, "primary key name"
    if (current_node->lpNext != NULL)
        primary_key_name = string(current_node->lpNext->text);
    
    //move to its son, first "attr"
    current_node = current_node->lpSub;

    //deal with each attribute
    while (current_node != NULL) {
        
        //deal with current attr
        SQLGrammarTree* temp_in_attr = current_node->lpSub; //NAME
        
        //assign name value
        tbl.attrs[tbl.attr_count].attr_name = string(temp_in_attr->text);
        
        //move to its next, "data_type"
        temp_in_attr = temp_in_attr->lpNext;
        
        //assign data_type value
        tbl.attrs[tbl.attr_count].attr_type = temp_in_attr->lpSub->type;
        
        //assign attr_key_type value
        if (temp_in_attr->lpNext == NULL) {
            tbl.attrs[tbl.attr_count].attr_key_type = -1;
        }
        else {
            tbl.attrs[tbl.attr_count].attr_key_type = temp_in_attr->lpNext->type;
        }
        
        //assign attribute len value
        if (temp_in_attr->lpSub->lpNext != NULL) //char
        {
            tbl.attrs[tbl.attr_count].attr_len = atoi(temp_in_attr->lpSub->lpNext->text);
            
            //if char(-1), return error
            if (tbl.attrs[tbl.attr_count].attr_len <= 0) {
                printf("error: SQL syntax wrong near '%d))'", tbl.attrs[tbl.attr_count].attr_len);
                return;
            }
            
        }
        else //not char
            tbl.attrs[tbl.attr_count].attr_len = 4;
        
        //assign attribute id value
        tbl.attrs[tbl.attr_count].attr_id = tbl.attr_count;
        
        //count next attribute
        ++tbl.attr_count;
        
        //move to next attribute
        current_node = current_node->lpNext;
        
    }

    //check whether primary key legal
    int primary_id = -1;
    if(primary_key_name != "") {
        bool exist = false;
        for (int i = 0; i < tbl.attr_count; ++i)
        {
            if(tbl.attrs[i].attr_name == primary_key_name) {
                primary_id = i;
                exist = true;
            }
        }
        if (exist == false)
        {
            printf("error: illegal primary key '%s'\n", primary_key_name.c_str());
            return;
        }
        else
        {
            //note primary key
            tbl.attrs[primary_id].attr_key_type = PRIMARY;
        }
    }
    
    //call API to create table
    API_Create_Table(tbl);
}

void ProcessCreateIndex(SQLGrammarTree* current_node)
{
    Index idx;
    //move to next node, "index_name"
    current_node = current_node->lpNext;
    
    //assign index_name value
    idx.index_name = string(current_node->text);
    
    //move to next node, "table_name"
    current_node = current_node->lpNext;
    
    //assign table_name value
    idx.table_name = string(current_node->text);
    
    //move to next node, "attr_name"
    current_node = current_node->lpNext;
    
    //assign attr_name value
    idx.attr_name = string(current_node->text);
    
    //check idx legal
    Table tbl = Read_Table_Info(idx.table_name);
    int i;
    for (i = 0; i < tbl.attr_count; i++) {
        if (tbl.attrs[i].attr_name == idx.attr_name) {
            break;
        }
    }
    //if attribute has not existed in table
    if (i == tbl.attr_count) {
        printf("error: Key column '%s' doesn't exist in table\n", idx.attr_name.c_str());
        return;
    }
    
    //call API to create index
    API_Create_Index(idx);
}

void ProcessDropTable(SQLGrammarTree* current_node)
{
    //move to next node, "table_name"
    current_node = current_node->lpNext;
    
    //call API to drop table
    API_Drop_Table(string(current_node->text));
}

void ProcessDropIndex(SQLGrammarTree* current_node)
{
    //move to next node, "index_name"
    current_node = current_node->lpNext;
    
    //call API to drop index
    API_Drop_Index(string(current_node->text));
}


void ProcessInsert(SQLGrammarTree* current_node)
{
    Tuple tuple;

    //move to next node, "table name"
    current_node = current_node->lpNext;
    
    //check existence of table
    if(Judge_table_exist(string(current_node->text)) == false) //no exist
    {
        printf("error: table '%s' does not exists\n", current_node->text);
        return;
    }
    //get table name
    tuple.table_name = string(current_node->text);

    //move to next node, "attr_value_list"
    current_node = current_node->lpNext;

    //read table information
    Table tbl = Read_Table_Info(tuple.table_name);
    
    //get each value
    current_node = current_node->lpSub;

    //assign value for tuple
    tuple.attr_count = tbl.attr_count;
    memcpy(tuple.attrs, tbl.attrs, tbl.attr_count * sizeof(Attribute));

    vector<int> v;
    int i = 0;
    while(current_node != NULL) 
    {
        //get attribute list type
        v.push_back(current_node->type);
        
        //get attribute
        tuple.attr_values[i++] = string(current_node->text);

        //move to next node
        current_node = current_node->lpNext;
    }

    //check the attribute list size
    if (v.size() != tbl.attr_count)
    {
        printf("error: the number of attribute list is not equal as defined.\n");
        return;
    }
    
    //check each value type
    for (int i = 0; i < v.size(); ++i)
    {
        //if input null but it is a primary key
        if (v[i] == EMPTY && tbl.attrs[i].attr_key_type == PRIMARY)
        {
            printf("error: cannot set null value on primary key '%s'\n", tbl.attrs[i].attr_name.c_str());
            return;
        }

        //check type matching
        if (v[i] == STRING && tbl.attrs[i].attr_type != CHAR)
        {
            printf("error: type does not match on attribute: %s\n", tbl.attrs[i].attr_name.c_str());
            return;
        }
        if(v[i] == INTNUM && tbl.attrs[i].attr_type == CHAR)
        {
            printf("error: type does not match on attribute: %s\n", tbl.attrs[i].attr_name.c_str());
            return;
        }
        if(v[i] == FLOATNUM && tbl.attrs[i].attr_type == CHAR)
        {
            printf("error: type does not match on attribute: %s\n", tbl.attrs[i].attr_name.c_str());
            return;
        }
    }
    
    //real API Insert
    API_Insert(tuple);
}

void ProcessDelete(SQLGrammarTree* current_node) //delete from table_name where a1 > value1 and a2 < value2
{
    //move to table_name
    current_node = current_node->lpNext;

    string table_name = string(current_node->text);

    //check existence of table
    if(Judge_table_exist(string(current_node->text)) == false) //no exist
    {
        printf("error: Table '%s' doesn't exist\n", current_node->text);
        return;
    }

    Condition_list clist;

    //delete without condition: delete from table_name;
    if (current_node->lpNext == NULL) 
    {
        API_Delete(table_name, clist);
        return;
    }

    //delete with conditions
    Table tbl = Read_Table_Info(string(current_node->text));

    //get first condition
    current_node = current_node->lpNext->lpSub;

    //get each condition, NAME COMPARISON real_value
    while(current_node != NULL) 
    {
        //move to son, NAME 
        SQLGrammarTree* temp_in_cond = current_node->lpSub;
        //scan the attribute list, check whether the name is in it
        int i, num;
        for (i = 0; i < tbl.attr_count; ++i)
            if (string(temp_in_cond->text) == tbl.attrs[i].attr_name)
            {
                num = i; //note the id of attribute
                break;
            }
            
        //if not found
        if (i == tbl.attr_count) 
        {
            printf("error: Unknown column '%s' in 'where clause'\n", temp_in_cond->text);
            return;
        }
        //if attribute exists
        
        //note type of attribute
        int type = tbl.attrs[num].attr_type;
        string name = string(temp_in_cond->text);
        
        //move to next, COMPARSION
        temp_in_cond = temp_in_cond->lpNext;
        
        //note COMPARSION
        string op = string(temp_in_cond->text);

        //move to next, VALUE
        temp_in_cond = temp_in_cond->lpNext;

        if (type == CHAR && temp_in_cond->type != STRING)
        {
            printf("error: type does not match on '%s' and '%s'\n", name.c_str(), temp_in_cond->text);
            return;
        }
        if (type == INT && temp_in_cond->type == STRING)
        {
            printf("error: type does not match on '%s' and '%s'\n", name.c_str(), temp_in_cond->text);
            return;
        }
        if (type == FLOAT && temp_in_cond->type == STRING)
        {
            printf("error: type does not match on '%s' and '%s'\n", name.c_str(), temp_in_cond->text);
            return;
        }

        //construct a condition
        Condition cond;
        cond.attr_name = name;
        cond.op_type = op;
        cond.cmp_value = string(temp_in_cond->text);

        clist.push_back(cond);

        current_node = current_node->lpNext;
    }

    API_Delete(table_name, clist);
}

void ProcessSelect(SQLGrammarTree* current_node)
{
    //move to table_name
    current_node = current_node->lpNext;

    string table_name = string(current_node->text);

    //check existence of table
    if(Judge_table_exist(string(current_node->text)) == false) //no exist
    {
        printf("error: Table '%s' doesn't exist\n", current_node->text);
        return;
    }

    Condition_list clist;

    //delete without condition: delete from table_name;
    if (current_node->lpNext == NULL) 
    {
        API_Select(table_name, clist);
        return;
    }

    //delete with conditions
    Table tbl = Read_Table_Info(string(current_node->text));

    //get first condition
    current_node = current_node->lpNext->lpSub;

    //get each condition, NAME COMPARISON real_value
    while(current_node != NULL) 
    {
        //move to son, NAME 
        SQLGrammarTree* temp_in_cond = current_node->lpSub;
        //scan the attribute list, check whether the name is in it
        int i, num;
        for (i = 0; i < tbl.attr_count; ++i)
            if (string(temp_in_cond->text) == tbl.attrs[i].attr_name)
            {
                num = i; //note the id of attribute
                break;
            }
            
        //if not found
        if (i == tbl.attr_count) 
        {
            printf("error: Unknown column '%s' in 'where clause'\n", temp_in_cond->text);
            return;
        }
        //if attribute exists
        
        //note type of attribute
        int type = tbl.attrs[num].attr_type;
        string name = string(temp_in_cond->text);
        
        //move to next, COMPARSION
        temp_in_cond = temp_in_cond->lpNext;
        
        //note COMPARSION
        string op = string(temp_in_cond->text);

        //move to next, VALUE
        temp_in_cond = temp_in_cond->lpNext;

        if (type == CHAR && temp_in_cond->type != STRING)
        {
            printf("error: type does not match on '%s' and '%s'\n", name.c_str(), temp_in_cond->text);
            return;
        }
        if (type == INT && temp_in_cond->type == STRING)
        {
            printf("error: type does not match on '%s' and '%s'\n", name.c_str(), temp_in_cond->text);
            return;
        }
        if (type == FLOAT && temp_in_cond->type == STRING)
        {
            printf("error: type does not match on '%s' and '%s'\n", name.c_str(), temp_in_cond->text);
            return;
        }

        //construct a condition
        Condition cond;
        cond.attr_name = name;
        cond.op_type = op;
        cond.cmp_value = string(temp_in_cond->text);

        clist.push_back(cond);

        current_node = current_node->lpNext;
    }

    API_Select(table_name, clist);
}

void ProcessExecfile(SQLGrammarTree* current_node)
{
    //move to next node, file name
    current_node = current_node->lpNext;

    //get file name
    if(current_node != NULL)
    {
        FILE* fp = fopen(current_node->text, "r");
        if (fp != NULL)
        {   
            yyin = fp;
            yyrestart(yyin);
        }
        else
        {
            printf("error: no such SQL script '%s' exists\n", current_node->text);
            return;
        }
    }
}












