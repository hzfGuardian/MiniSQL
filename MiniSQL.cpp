#include "BufferManager/BufferManager.h" 
#include "Interpreter/Interpreter.h"
#include "API/API.h"
using namespace std;

extern MBuffer m1;
extern MBuffer mybuffer;

int main(int argc, const char* argv[])
{
	m1.Init();
	mybuffer.Init();
    Interpreter inter;
    
    inter.exec();

    return 0;
}

/*
 
/Users/huangzf/Library/Developer/Xcode/DerivedData/Minisql-faimapqkrbixgyelcnpuklbhhfxf/Build/Products/Debug

*/