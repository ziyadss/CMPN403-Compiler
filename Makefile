build:
	mkdir -p build bin
	lex -o build/lex.yy.c src/lexer.l
	yacc -o build/y.tab.c -d src/parser.y
	gcc -Wall -Wextra -o bin/compiler.exe build/lex.yy.c build/y.tab.c

debug:
	mkdir -p build bin
	lex -o build/lex.yy.c src/lexer.l
	yacc -o build/y.tab.c -d src/parser.y -vt 
	gcc -Wall -Wextra -o bin/compiler.exe build/lex.yy.c build/y.tab.c

counterex:
	mkdir -p build bin
	lex -o build/lex.yy.c src/lexer.l
	yacc -o build/y.tab.c -d src/parser.y -Wcounterexamples &> counterexamples
	gcc -Wall -Wextra -o bin/compiler.exe build/lex.yy.c build/y.tab.c


clean:
	rm -r build bin
	rm output_log counterexamples

compile:
	echo "Compiling..."
	./bin/compiler.exe tests/source.c 
	echo "Compiled!"

compile_debug:
	echo "debugging..."
	./bin/compiler.exe tests/source.c &> output_log
	echo "done!"