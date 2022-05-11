build:
	mkdir -p build bin
	lex -o build/lex.yy.c src/lexer.l
	yacc -o build/y.tab.c -d src/parser.y
	gcc -Wall -Wextra -o bin/compiler.exe build/lex.yy.c build/y.tab.c src/main.c
	# -static-libgcc -static-libstdc++ -ggdb

clean:
	rm -r build bin