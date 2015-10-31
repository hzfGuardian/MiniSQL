/*Index Manager
Index Manager负责B+树索引的实现，实现B+树的创建和删除（由索引的定义与删除引起）、等值查找、插入键值、删除键值等操作，并对外提供相应的接口。
B+树中节点大小应与缓冲区的块大小相同，B+树的叉数由节点大小与索引键大小计算得到。*/
#ifndef _BPLUS_H_
#define _BPLUS_H_

#include "../MiniSQL.h"
#include "../BufferManager/BufferManager.h"
using namespace std;


class node{
protected:
	int type;
	int N;
	string* value;
public:
	node(int q);
	virtual ~node(); 
	virtual int gettype();
	virtual int Haveson();
	virtual string getvalue(int i);
	virtual void setvalue(string k,int i);
	virtual int getadree(int i); //修改 
	virtual void setadree(int place,int i);
	virtual int getOffset(int i);
	virtual void setOffset(int Off,int i);
	virtual void clear();
	virtual void setNext(int place);
	virtual int getNext();
	virtual int whoareyou();
};


class normalnode :public node{
private:
	int* P; //子节点位置 
public:
	normalnode(int q);
	virtual ~normalnode();
	virtual int Haveson();
	virtual int getadree(int i); //修改 
	virtual void setadree(int place,int i);//修改 
	void clear();
	virtual int whoareyou();
};


class leafnode :public node{
protected:
	int* Offset; //偏移量
private:
	int Next;
public:
	leafnode(int q);
	virtual ~leafnode();
	virtual int Haveson();
	virtual int getOffset(int i);
	virtual void setOffset(int Off,int i);
	virtual void clear();
	virtual void setNext(int place);
	virtual int getNext();
	virtual int whoareyou();
};

class tempnode :public leafnode{
private:
	string* value2;
public:
	tempnode(int q);
	tempnode(int q,node *p);//修改 
	virtual ~tempnode();
	virtual int Haveson();
	virtual string getvalue(int i);
	virtual void setvalue(string k,int i);
	virtual int getOffset(int i);
	virtual void setOffset(int Off,int i);
};

class neicun{
public:
	int type;
	int N;
	string* value;
	int* p;
	neicun(int q);
	neicun(int q,node *p);
	~neicun();
};

node* getfromfile(char* re);//0 int, 1 float, 2 char*
char* getfromnode(node* p);
node* getnode(int off,string filename);

int insertleaf(string k,int off,int place,string filename);

int insert(string k,int off,int place,string filename);

int find1(int root,string s,string filename);

int findfirst(int root,string filename);

vector<int> findsmallthan(int root,string value,string filename,int equal);

vector<int> findbigthan(int root,string value,string filename,int equal);

vector<int> findnoeq(int root,string value,string filename);

int deletenode(string k,int place,string filename);

int deletereall(string k,int place,int root,string filename);


#endif

