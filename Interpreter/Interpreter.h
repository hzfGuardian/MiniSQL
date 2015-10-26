#pragma once

#include "../MiniSQL.h"
#include "Lex/Analysis.hpp"

using namespace std;


class Interpreter {
    
public:
    Interpreter()
    {
        //welcome user interface
        cout << WELCOME << endl;
        cout << PROMPT;
    }
    
    ~Interpreter()
    {
        cout << endl << BYE_BYE << endl;
    }
    
    void exec()
    {
        yyparse();
    }
     
};


















