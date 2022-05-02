build:
	mkdir -p build bin
	lex -o build/lex.yy.c src/lexer.l
	yacc -o build/y.tab.c -d src/parser.y

debug:
	mkdir -p build bin
	lex -o build/lex.yy.c src/lexer.l
	yacc -o build/y.tab.c -d src/parser.y &> counterexamples

clean:
	rm -r build bin

compile:
	gcc -Wall -Wextra -o bin/compiler.exe build/lex.yy.c build/y.tab.c
	echo "Compiling..."
	./bin/compiler.exe tests/source.c
	echo "Compiled!"