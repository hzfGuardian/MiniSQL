
#include "SQLGrammarTree.h"
#include "../MiniSQL.h"
#include "Lex/Analysis.hpp"
#include "../API/API.h"

using namespace std;

Table tbl;
string primary_key_name;
Index idx;

NodeManager node_manager;


extern void yyrestart(FILE* fp);
extern FILE* yyin;

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
    SQLGrammarTree* current_node = pNode->lpSub;
    switch (current_node->type) {

        case CREATE:
            
            //move to next node, "table" or "index"
            current_node = current_node->lpNext;
            
            switch (current_node->type) {
                
                //create table
                case TABLE:
                
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
                                cout << ERROR_CHAR_INDEX_LESS_ZERO << endl;
                                return;
                            }
                            
                        }
                        else //not char
                            tbl.attrs[tbl.attr_count].attr_len = 1;
                        
                        //assign attribute id value
                        tbl.attrs[tbl.attr_count].attr_id = tbl.attr_count;
                        
                        //count next attribute
                        ++tbl.attr_count;
                        
                        //move to next attribute
                        current_node = current_node->lpNext;
                        
                    }
                    
                    //call API to create table
                    API_Create_Table(tbl);
                    
                    break;
                
                case INDEX: //CREATE INDEX NAME ON NAME '(' NAME ')'
                    
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
                    
                    //call API to create index
                    API_Create_Index(idx);
                    
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
                    
                    //move to next node, "table_name"
                    current_node = current_node->lpNext;
                    
                    //call API to drop table
                    API_Drop_Table(string(current_node->text));
                    
                    break;
                    
                case INDEX:
                    
                    //move to next node, "index_name"
                    current_node = current_node->lpNext;
                    
                    //call API to drop index
                    API_Drop_Index(string(current_node->text));

                    break;
                    
                default:
                    break;
            }
            break;
        
        case INSERT: //INSERT INTO NAME VALUES '(' attr_value_list ')'
            
            //move to next node, "name"
            current_node = current_node->lpNext;
            
            //
            
            break;
        
        case DELETE:
            
            break;
        
        case SELECT:
            
            break;
        
        case EXECFILE:
            
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
                    printf("error: no such file exists: %s\n", current_node->text);
                    return;
                }
            }

            break;
        default:
            break;
    }
    
    cout << "Info: done." << endl;
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




