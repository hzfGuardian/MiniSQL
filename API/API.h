#pragma once

#include "../RecordManager/Record.h"

class API: public Record, public Index, public Catalog,
        public SQLGrammarTree
{
public:
    //	函数原型声明
    
    //	API主程序
    static void exec()
    {
        
    }
    
    //	创建表时的内部调用
    static bool API_Create_Table()
    {
        
    }
    
    //	删除表时的内部调用
    static bool API_Drop_Table()
    {
        
    }
    
    //	创建索引时的内部调用
    static bool API_Create_Index()
    {
        
    }
    
    //	删除索引时的内部调用
    static bool API_Drop_Index()
    {
        
    }
    
    //	读取表信息
    static void Read_Table_Info()
    {
        
    }
    
    //	读取索引信息
    static void Read_Index(string index_name)
    {
        
    }
    
    //	插入纪录时的内部调用
    static bool API_Insert()
    {
        
    }
    
    //	选择纪录时的内部调用
    static bool API_Select()
    {
        
    }
    
    //	删除纪录时的内部调用
    static bool API_Delete()
    {
        
    }
    
    
private:
    
    //操作类型，包括上述所有操作
    int opType;
    
};




