#pragma once

#include "../MiniSQL.h"
//#include "Lex/lex.cpp"
#include "Lex/Analysis.hpp"

using namespace std;


class Interpreter {
public:
    Interpreter() :sql_instr(""), current_line(""), prompt_state(true)
    {
        //welcome user interface
        cout << WELCOME << endl;
        cout << PROMPT;
    }
    
    ~Interpreter()
    {
        cout << endl << BYE_BYE << endl;
    }
    
    void userInput()
    {
        //read a line once
        while (cin.getline(current_line, MAX_ONE_LINE)) {
            
            //transfer char* to string
            sql_instr += string(current_line);
            
            //a line ends with ';'
            if (hasTerminated(current_line)) {
                
                //redirect yyin to string sql_instr
                stringParse();
                
                //clean the string
                sql_instr = "";
                
                //change the state
                prompt_state = true;
            }
            
            else {
                prompt_state = false;
            }
            
            //print the prompt on the head of a line
            if (prompt_state)
                cout << PROMPT;
            else
                cout << ENTER_SIGN;
        }
        
    }
    
    bool hasTerminated(char *s)
    {
        for (int i=strlen(s)-1; i >= 0; i--) {
            if (s[i]=='\t' || s[i]==' ')
                continue;
            else if (s[i]==';') {
                return true;
            }
            else {
                return false;
            }
        }
        //if arrives here, it is an empty string
        return false;
    }
    
    void stringParse()
    {
        //test code
        char scan_str[1000];

        strcpy(scan_str, sql_instr.c_str());

        scan_str[strlen(scan_str)+1] = '\0';
        /*
        yy_scan_buffer(scan_str, strlen(scan_str)+2);
        
        yyparse();

        yy_delete_buffer(YY_CURRENT_BUFFER);
        */
        
        //used for check
        cout << "Deal success" << endl;
        cout << "Check: " << sql_instr << endl;
    }
    
private:
    //SQL expression string, end with ';'
    string sql_instr;
    
    //user input on current line
    char current_line[MAX_ONE_LINE];
    
    //When state is true, output "minisql> ". Else output " ->"
    bool prompt_state;
    
};


















