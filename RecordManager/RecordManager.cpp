#include "RecordManager.h"

string delete_values[32];
vector<string> select_values;
MBuffer mybuffer;
int delete_num;
int select_num;

std::vector<int> select_offsets;

char* Convertvalue(Tuple &tuple)
{
    string value = "";
    char *p = new char[tuple.length()+2];
    memset(p, 0, tuple.length()+2);
    char *v = p;

    int ivalue;

    float fvalue;
    for(int i=0;i<tuple.attr_count;i++)
    {
        if(tuple.attrs[i].attr_type == CHAR)
        {
            strcpy(v,tuple.attr_values[i].data());
            v += tuple.attr_values[i].size();
        }
        if(tuple.attrs[i].attr_type == INT)
        {
            ivalue = atoi(tuple.attr_values[i].c_str());

            memcpy(v, &ivalue, 4);
            v += 4;
        }
        if(tuple.attrs[i].attr_type == FLOAT)
        {
            fvalue = (float) atof(tuple.attr_values[i].c_str());
            memcpy(v,&fvalue,4);
            v += 4;
        }
    }

    return p;
}

int Insert(Tuple &tuple)
{
    int i,j;
    int offtest;
    char *values;
    int dp;
    int ip;
    char *t;
    int compi;
    float compf;
    bool flag;
    string comps;
    char *target = new char[1];
    int comi;
    float comf;
    values = Convertvalue(tuple);
    
    values[tuple.length()]='1';
    values[tuple.length()+1]=0;
    string file_name = tuple.table_name + "_table.rec";
    int offset = Block_num(file_name)-1;
    if(offset<0)
    {
        Block* newblock = mybuffer.GetBlock(file_name,0,1);
        memcpy(newblock->record,values,(tuple.length()+1));

        delete[] values;
        delete target;
        return 0;
    }
    else
    {
        Block* Target;
        for(i=0;i<tuple.attr_count;i++)
        {
            if(tuple.attrs[i].attr_key_type==UNIQUE)
            {
                ip=0;
                for(j=0;j<i;j++)
                {
                    ip+=tuple.attrs[j].attr_len;
                }
                for(offtest = 0;offtest<=offset;offtest++)
                {
                    Target = mybuffer.GetBlock(file_name,offtest,0);
                    for(dp=0;dp<BlockSize;dp+=tuple.length()+1)
                    {
                        memcpy(target,Target->record+dp+tuple.length(),1);
                        if(*target=='1')
                        {
                            if(tuple.attrs[i].attr_type==CHAR)
                            {
                                t = new char[tuple.attrs[i].attr_len+1];
                                memcpy(t,Target->record+dp+ip,tuple.attrs[i].attr_len);
                                t[tuple.attrs[i].attr_len]='\0';
                                comps=t;
                                if(comps==tuple.attr_values[i])
                                {
                                    flag = true;
                                    break;
                                }
                                delete t;
                            }
                            if(tuple.attrs[i].attr_type==INT)
                            {
                                memcpy(&compi,Target->record+dp+ip,4);
                                comi=atoi(tuple.attr_values[i].c_str());
                                if(compi==comi)
                                {
                                    flag = true;
                                    break;
                                }
                            }
                            if(tuple.attrs[i].attr_type==FLOAT)
                            {
                                memcpy(&compf,Target->record+dp+ip,4);
                                comf=atof(tuple.attr_values[i].c_str());
                                if(compf==comf)
                                {
                                    flag = true;
                                    break;
                                }
                            }
                        }
                    }
                    if(flag == true)
                        break;
                }
                if(flag == true)
                    break;
            }
        }
        if(flag != true)
        {
            Target = mybuffer.GetBlock(file_name,offset,0);
            for(i=tuple.length();i<BlockSize;i+=tuple.length()+1)
            {
                if(Target->record[i] == 0)
                    break;
            }
            if(i<BlockSize)
            {
                memcpy(Target->record+i-tuple.length(),values,tuple.length()+1);
                Target->accessed = true;
                Target->written = true;
                delete[] values;
                delete target;
                return offset;
            }
            else
            {
                Block* newblock = mybuffer.GetBlock(file_name,offset+1,1);
                memcpy(newblock->record,values,tuple.length()+1);
                delete[] values;
                delete target;
                return offset+1;
            }
        }
    }

    delete[] values;
    delete target;
    return -1;
}

bool Delete_tuple(vector<int> offsetlist, Table table, Condition_list list)
{
    Block * Target;
    int compi;
    delete_num = 0;
    float compf;
    string comps;
    char *target = new char[1];
    int comi;
    int comf;
    int i,j;
    int vi=0;
    int ip;
    string value;
    int dp;
    int offset=0;
    Condition_list::iterator li;
    int Blocknum = 0;
    char* t;
    string file_name = table.table_name + "_table.rec";
    for(int i=0;i<32;i++)
    {
        delete_values[i]="";
    }
    if(offsetlist.empty()==1)
    {
        Blocknum = Block_num(file_name);
    }
    if(Blocknum != 0 || offsetlist.empty() != 1)
    {
        while(1)
        {
            if(offsetlist.empty()!=1)
            {
                offset = offsetlist[vi];
            }
            if(offset>=Block_num(file_name))
                break;
            Target = mybuffer.GetBlock(file_name,offset,0);
            for(dp=0;dp<BlockSize;dp+=(table.length()+1))
            {
                memcpy(target,Target->record+dp+table.length(),1);
                if(*target == '1')
                {
                    if(list.empty()!=1)
                    {
                        for(li=list.begin();li!=list.end();li++)
                        {
                            
                            for(i=0;i<table.attr_count;i++)
                            {
                                if(table.attrs[i].attr_name == li->attr_name)
                                    break;
                            }
                            ip = 0;
                            for(j=0;j<i;j++)
                            {
                                ip+=table.attrs[j].attr_len;
                            }
                            if(table.attrs[i].attr_type == CHAR)
                            {
                                t = new char[table.attrs[i].attr_len+1];
                                memcpy(t,Target->record+dp+ip,table.attrs[i].attr_len);
                                t[table.attrs[i].attr_len]='\0';
                                comps = t;
                                delete t;
                                if(li->op_type == "<>")
                                {
                                    if(li->cmp_value == comps)
                                        break;
                                }
                                else if(li->op_type == "=")
                                {
                                    if(li->cmp_value != comps)
                                        break;
                                }
                                else if(li->op_type == ">")
                                {
                                    if(li->cmp_value >= comps)
                                        break;
                                }
                                else if(li->op_type == ">=")
                                {
                                    if(li->cmp_value > comps)
                                        break;
                                }
                                else if(li->op_type == "<")
                                {
                                    if(li->cmp_value <= comps)
                                        break;
                                }
                                else if(li->op_type == "<=")
                                {
                                    if(li->cmp_value < comps)
                                        break;
                                }
                            }
                            else if(table.attrs[i].attr_type == INT)
                            {
                                memcpy(&compi, Target->record+dp+ip, 4);
                                comi = atoi(li->cmp_value.c_str());
                                if(li->op_type == "<>")
                                {
                                    if(comi == compi)
                                        break;
                                }
                                else if(li->op_type == "=")
                                {
                                    if(comi != compi)
                                        break;
                                }
                                else if(li->op_type == ">")
                                {
                                    if(comi>= compi)
                                        break;
                                }
                                else if(li->op_type == ">=")
                                {
                                    if(comi > compi)
                                        break;
                                }
                                else if(li->op_type == "<")
                                {
                                    if(comi <= compi)
                                        break;
                                }
                                else if(li->op_type == "<=")
                                {
                                    if(comi < compi)
                                        break;
                                }
                            }
                            else if(table.attrs[i].attr_type==FLOAT)
                            {
                                memcpy(&compf,Target->record+dp+ip,4);
                                comf = atof(li->cmp_value.c_str());
                                if(li->op_type == "<>")
                                {
                                    if(comf == compf)
                                        break;
                                }
                                else if(li->op_type == "=")
                                {
                                    if(comf != compf)
                                        break;
                                }
                                else if(li->op_type == ">")
                                {
                                    if(comf >= compf)
                                        break;
                                }
                                else if(li->op_type == ">=")
                                {
                                    if(comf > compf)
                                        break;
                                }
                                else if(li->op_type == "<")
                                {
                                    if(comf <= compf)
                                        break;
                                }
                                else if(li->op_type == "<=")
                                {
                                    if(comf < compf)
                                        break;
                                }
                            }
                        }
                    }
                    if(li == list.end()||list.empty()==1)
                    {
                        delete_num++;
                        *target = '0';
                        memcpy(Target->record+dp+table.length(),target,1);
                        for(int i=0;i<table.attr_count;i++)
                        {
                            if(table.attrs[i].attr_key_type!=EMPTY)
                            {
                                ip = 0;
                                for(j=0;j<i;j++)
                                {
                                    ip+=table.attrs[j].attr_len;
                                }
                                value = "";
                                if(table.attrs[i].attr_type==CHAR)
                                {
                                    t = new char[table.attrs[i].attr_len+1];
                                    memcpy(t,Target->record+dp+ip,table.attrs[i].attr_len);
                                    t[table.attrs[i].attr_len]='\0';
                                    value = t;
                                    delete_values[i]+=value+'\t';
                                    delete t;
                                }
                                if(table.attrs[i].attr_type==INT)
                                {
                                    t = new char[100];
                                    memcpy(&compi,Target->record+dp+ip,4);
                                    sprintf(t,"%d",compi);
                                    value = t;
                                    delete_values[i]+=value+'\t';
                                    delete t;
                                }
                                if(table.attrs[i].attr_type==FLOAT)
                                {
                                    t = new char[100];
                                    memcpy(&compf,Target->record+dp+ip,4);
                                    sprintf(t,"%f",compf);
                                    value = t;
                                    delete_values[i]+=value+'\t';
                                    delete t;
                                }
                            }
                        }						
                    }
                }				
            }
            if(offsetlist.empty()==1)
            {
                offset++;
                if(offset==Blocknum)
                    break;
            }
            if(offsetlist.empty()!=1)
            { 
                vi++; 
                if(vi == offsetlist.size())
                    break;
            } 
        }		
    }
    delete target;
    return true;
}

bool Select_tuple(vector<int> offsetlist, Table table, Condition_list list)
{		
    Block * Target;			
    int compi;
    select_num = 0;
    float compf;
    string comps;
    char *target = new char[1];
    int comi;
    int comf;
    int i,j;
    int vi=0;
    int ip;
    string value;
    int dp;			
    int offset=0;	
    Condition_list::iterator li;
    int Blocknum = 0;
    char* t;		
    string tuplevalue;
    string file_name = table.table_name + "_table.rec";
    select_values.clear();
    select_offsets.clear();
    if(offsetlist.empty()==1)
    {
        Blocknum = Block_num(file_name);			
    }
    if(Blocknum != 0 || offsetlist.empty() != 1)
    {
        while(1)
        {
            if(offsetlist.empty()!=1)
            {
                offset = offsetlist[vi];
            }
            if(offset>=Block_num(file_name))
                break;
            Target = mybuffer.GetBlock(file_name,offset,0);
            for(dp=0;dp<BlockSize;dp+=(table.length()+1))
            {
                memcpy(target,Target->record+dp+table.length(),1);
                if(*target == '1')
                {
                    if(list.empty()!=1)
                    {
                        for(li=list.begin();li!=list.end();li++)
                        {
                            for(i=0;i<table.attr_count;i++)
                            {
                                if(table.attrs[i].attr_name == li->attr_name)
                                    break;
                            }
                            ip = 0;
                            for(j=0;j<i;j++)
                            {
                                ip+=table.attrs[j].attr_len;
                            }
                            if(table.attrs[i].attr_type==CHAR)
                            {
                                t = new char[table.attrs[i].attr_len+1];
                                memcpy(t,Target->record+dp+ip,table.attrs[i].attr_len);
                                t[table.attrs[i].attr_len]='\0';
                                comps = t;
                                delete t;
                                if(li->op_type == "<>")
                                {
                                    if(li->cmp_value == comps)
                                        break;
                                }
                                else if(li->op_type == "=")
                                {
                                    if(li->cmp_value != comps)
                                        break;
                                }
                                else if(li->op_type == ">")
                                {
                                    if(li->cmp_value >= comps)
                                        break;
                                }
                                else if(li->op_type == ">=")
                                {
                                    if(li->cmp_value > comps)
                                        break;
                                }
                                else if(li->op_type == "<")
                                {
                                    if(li->cmp_value <= comps)
                                        break;
                                }
                                else if(li->op_type == "<=")
                                {
                                    if(li->cmp_value < comps)
                                        break;
                                }	
                            }
                            else if(table.attrs[i].attr_type==INT)
                            {
                                memcpy(&compi,Target->record+dp+ip,4);
                                comi = atoi(li->cmp_value.c_str());
                                if(li->op_type == "<>")
                                {
                                    if(comi == compi)
                                        break;
                                }
                                else if(li->op_type == "=")
                                {
                                    if(comi != compi)
                                        break;
                                }
                                else if(li->op_type == ">")
                                {
                                    if(comi>= compi)
                                        break;
                                }
                                else if(li->op_type == ">=")
                                {
                                    if(comi > compi)
                                        break;
                                }
                                else if(li->op_type == "<")
                                {
                                    if(comi <= compi)
                                        break;
                                }
                                else if(li->op_type == "<=")
                                {
                                    if(comi < compi)
                                        break;
                                }
                            }
                            else if(table.attrs[i].attr_type==FLOAT)
                            {
                                memcpy(&compf,Target->record+dp+ip,4);
                                comf = atof(li->cmp_value.c_str());
                                if(li->op_type == "<>")
                                {
                                    if(comf == compf)
                                        break;
                                }
                                else if(li->op_type == "=")
                                {
                                    if(comf != compf)
                                        break;
                                }
                                else if(li->op_type == ">")
                                {
                                    if(comf >= compf)
                                        break;
                                }
                                else if(li->op_type == ">=")
                                {
                                    if(comf > compf)
                                        break;
                                }
                                else if(li->op_type == "<")
                                {
                                    if(comf <= compf)
                                        break;
                                }
                                else if(li->op_type == "<=")
                                {
                                    if(comf < compf)
                                        break;
                                }	
                            }
                        }
                    }
                    if(li == list.end()||list.empty())
                    {
                        select_num++;
                        tuplevalue = "";
                        for(int i=0;i<table.attr_count;i++)
                        {
                            ip = 0;
                            for(j=0;j<i;j++)
                            {
                                ip+=table.attrs[j].attr_len;
                            }
                            value = "";
                            if(table.attrs[i].attr_type==CHAR)
                            {
                                t = new char[table.attrs[i].attr_len+1];
                                memcpy(t,Target->record+dp+ip,table.attrs[i].attr_len);
                                t[table.attrs[i].attr_len]='\0';
                                value = t;
                                tuplevalue+=value+'\t';
                                delete t;
                            }
                            if(table.attrs[i].attr_type==INT)
                            {
                                t = new char[100];
                                memcpy(&compi,Target->record+dp+ip,4);									
                                sprintf(t,"%d",compi);
                                value = t;
                                tuplevalue+=value+'\t';
                                delete t;
                            }
                            if(table.attrs[i].attr_type==FLOAT)
                            {
                                t = new char[100];
                                memcpy(&compf,Target->record+dp+ip,4);
                                sprintf(t,"%f",compf);
                                value = t;
                                tuplevalue+=value+'\t';
                                delete t;
                            }
                        }
                        select_values.push_back(tuplevalue);
                        select_offsets.push_back(offset);						
                    }
                }				
            }
            if(offsetlist.empty()==1)
            {
                offset++;
                if(offset==Blocknum)
                    break;
            }
            if(offsetlist.empty()!=1)
            { 
                vi++; 
                if(vi == offsetlist.size())
                    break;
            } 
        }		
    }
    delete target;
    return true;
}

