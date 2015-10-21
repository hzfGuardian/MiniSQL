#pragma once

//SQL语法树
struct SQLGrammarTree {
	int type;
	char text[48];
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






