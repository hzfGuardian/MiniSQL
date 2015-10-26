all:MiniSQL clean

MiniSQL: 
	g++ -o MiniSQL Interpreter/SQLGrammarTree.cpp Interpreter/Lex/*.cpp API/API.cpp CatalogManager/CatalogManager.cpp MiniSQL.cpp 
clean:
	find ./ -name "*.o" -depth -exec rm {} \;

