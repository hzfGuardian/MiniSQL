//#include "../SQLGrammarTree.h"
#include <stdio.h>
#include <stdarg.h>

void simple_va_fun(int i, int j, ...)
{
    va_list arg_ptr;
    int k=2, kk=1;
    
    va_start(arg_ptr, j);
    
    k = va_arg(arg_ptr, int);
    kk = va_arg(arg_ptr, int);
    
    va_end(arg_ptr);
    
    printf("%d %d %d %d\n", i, j, k, kk);
}

int main()
{
    //SQLGrammarTree* s_tree = MallocNewNode();
    simple_va_fun(100, 200, 300, 400);
	return 0;
}
