#pragma once

#include <vector>

//SQL语法树
struct SQLGrammarTree {
	int type;
	char text[256];
	SQLGrammarTree *lpNext; // 指向同一层的节点
	SQLGrammarTree *lpSub;  // 指向子节点
};

//以下为语法树外部函数

SQLGrammarTree* MallocNewNode();

void ProcessTree(SQLGrammarTree* pNode);

SQLGrammarTree* FatherAddSon(SQLGrammarTree* pFather, SQLGrammarTree* pSon);

//add num_of_sons sons to a new father node and return father
SQLGrammarTree* NewFatherAddSon(int iType, int num_of_sons, ...);

//free the tree
void FreeTree(SQLGrammarTree* pNode);


//节点管理器
typedef std::vector< SQLGrammarTree* > NodeManager;


//clear
void nm_clear();


//以下为不同语句处理的函数

void ProcessCreateTable(SQLGrammarTree* );

void ProcessCreateIndex(SQLGrammarTree* );

void ProcessDropTable(SQLGrammarTree* );

void ProcessDropIndex(SQLGrammarTree* );

void ProcessInsert(SQLGrammarTree* );

void ProcessDelete(SQLGrammarTree* );

void ProcessSelect(SQLGrammarTree* );

void ProcessExecfile(SQLGrammarTree* );






