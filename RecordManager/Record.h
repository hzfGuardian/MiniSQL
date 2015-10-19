#pragma once

/*
 Record Manager 负责管理记录表中数据的数据文件。主要功能为实现数据文件的创建与删除(由表的定义与删除引起)、记录的插入、删除与查找操作,并对外提供相应的接口。其中记录的查找操作要求能够支持不带条件的查找和带一个条件的查找(包 括等值查找、不等值查找和区间查找)。
 数据文件由一个或多个数据块组成,块大小应与缓冲区块大小相同。一个块中包含一条至多条记录,为简单起见,只要求支 持定长记录的存储,且不要求支持记录的跨块存储。
 */

#include "../MiniSQL.h"

class Record {
    
public:
    
    //查找记录功能
    bool Select(Table table, Condition_list conditions)
    {
        
    }
    
    //	实现插入纪录的功能
    bool Insert(Table table, string attr_value[])
    {
        
    }
    
    //	实现删除纪录的功能
    bool Delete(Table table, Condition_list conditions)
    {
        
    }
    
    //删除表文件
    bool Drop_table(string table)
    {
        
    }
    
    //创建表文件
    bool Create_table(string table)
    {
        
    }
    
private:
    
};


