build:
	mkdir -p build bin
	lex -o build/lex.yy.c src/lexer.l
	yacc -o build/y.tab.c -d src/parser.y
	gcc -Wall -Wextra -o bin/compiler.exe build/lex.yy.c build/y.tab.c

debug:
	mkdir -p build bin
	lex -o build/lex.yy.c src/lexer.l
	yacc -o build/y.tab.c -d src/parser.y &> counterexamples
	gcc -Wall -Wextra -o bin/compiler.exe build/lex.yy.c build/y.tab.c

clean:
	rm -r build bin
	rm counterexamples

compile:
	echo "Compiling..."
	./bin/compiler.exe tests/source.c
	echo "Compiled!"