#ifndef _BufferManager_H_
#define _BufferManager_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <list>
#define BlockSize 4096
using namespace std;

struct Block
{
	string tablename;
	char* record;
	bool written;
	bool accessed;
	int offset; 
};

class MBuffer{
	public:
		Block* Buffer[64];
	public:
		MBuffer(){
			int i;
			for(i=0;i<64;i++)
				Buffer[i]=NULL;
				
		};
		bool Init();
		Block* GBlock();
		Block* GetBlock(string tablename, int offset, int flag);
	 	bool Sche1(string tablename, int offset);
	 	bool Sche2(string tablename, int offset);
	 	bool Drop(string tablename);
	 	bool Exchange(string tablename, int offset, Block* Repalced);
    
        void clearBuffer();
    
	 	~MBuffer();
};
int Block_num (string file_name);
#endif 
