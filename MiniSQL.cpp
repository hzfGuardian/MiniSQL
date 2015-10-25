#include "Interpreter/Interpreter.h"
using namespace std;


int main(int argc, const char* argv[])
{
    Interpreter inter;

    //real deal
    yyparse();

    return 0;
}