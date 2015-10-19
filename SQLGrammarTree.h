#ifndef __SQL_H
#define __SQL_H

//SQL语法树

#include <iostream>
#include <cstring>
using namespace std;

struct SQLGrammarTree;

void ProcessTree(struct SQLGrammarTree* pNode);
SQLGrammarTree* MallocNewNode();
SQLGrammarTree* FatherAddSon(SQLGrammarTree* pFather, SQLGrammarTree* pSon);
SQLGrammarTree* NewFatherAddSon(int iType, SQLGrammarTree* pSon1);
SQLGrammarTree* NewFatherAddSon(int iType, SQLGrammarTree* pSon1, SQLGrammarTree* pSon2);
SQLGrammarTree* NewFatherAddSon(int iType, SQLGrammarTree* pSon1, SQLGrammarTree* pSon2, SQLGrammarTree* pSon3);
void FreeTree(SQLGrammarTree* pNode);


struct SQLGrammarTree{
	int type;
	double dVal;
	char text[48];
	SQLGrammarTree *lpNext; // 指向同一层的节点
	SQLGrammarTree *lpSub;  // 指向子节点
    
    friend SQLGrammarTree* MallocNewNode(){
        SQLGrammarTree* pNew = new SQLGrammarTree;
        if (!pNew)
        {
            return pNew;
        }
        memset(pNew, 0, sizeof(*pNew));
        return pNew;
    }
    
    friend void ProcessTree(SQLGrammarTree* pNode){
        cout << "Info: done.\n";
    }
    
    friend SQLGrammarTree* FatherAddSon(SQLGrammarTree* pFather, SQLGrammarTree* pSon){
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
    
    friend SQLGrammarTree* NewFatherAddSon(int iType, SQLGrammarTree* pSon1){
        SQLGrammarTree* pFather = MallocNewNode();
        if (!pFather)
        {
            return pFather;
        }
        
        pFather->type = iType;
        
        FatherAddSon(pFather, pSon1);
        return pFather;
    }
    
    friend SQLGrammarTree* NewFatherAddSon(int iType, SQLGrammarTree* pSon1, SQLGrammarTree* pSon2){
        SQLGrammarTree* pFather = MallocNewNode();
        if (!pFather)
        {
            return pFather;
        }
        
        pFather->type = iType;
        
        FatherAddSon(pFather, pSon1);
        FatherAddSon(pFather, pSon2);
        
        return pFather;
    }
    
    friend SQLGrammarTree* NewFatherAddSon(int iType, SQLGrammarTree* pSon1, SQLGrammarTree* pSon2, SQLGrammarTree* pSon3){
        SQLGrammarTree* pFather = MallocNewNode();
        if (!pFather)
        {
            return pFather;
        }
        
        pFather->type = iType;
        
        FatherAddSon(pFather, pSon1);
        FatherAddSon(pFather, pSon2);
        FatherAddSon(pFather, pSon3);
        
        return pFather;
    }
    
    friend void FreeTree(SQLGrammarTree* pNode){
        if (pNode->lpNext)  FreeTree(pNode->lpNext);
        if (pNode->lpSub)   FreeTree(pNode->lpSub);
        //real free
        delete pNode;
        pNode = NULL;
    }
};



#endif
