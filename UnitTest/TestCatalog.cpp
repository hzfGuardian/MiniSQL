#include "../CatalogManager/CatalogManager.h"
#include "../MiniSQL.h"

Table table;
Index idx;
string s;

int main()
{
    Create_table(table);
    Create_index(idx);
    Drop_table(s);
    Drop_index(s);
	return 0;
}
