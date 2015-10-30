all:MiniSQL clean

MiniSQL:Interpreter/SQLGrammarTree.cpp Interpreter/Lex/Analysis.cpp Interpreter/Lex/lex.cpp API/API.cpp CatalogManager/CatalogManager.cpp MiniSQL.cpp
	g++ -o MiniSQL Interpreter/SQLGrammarTree.cpp Interpreter/Lex/*.cpp API/API.cpp CatalogManager/CatalogManager.cpp RecordManager/RecordManager.cpp MiniSQL.cpp -m32 
clean:
	find ./ -name "*.o" -depth -exec rm {} \;

