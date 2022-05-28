.PHONY: build
build:
	mkdir -p build bin
	lex -o build/lex.yy.c src/lexer.l
	yacc -o build/y.tab.c -d src/parser.y
	gcc -fdiagnostics-color=always -Wall -Wextra -g build/lex.yy.c build/y.tab.c -o bin/compiler.exe

clean:
	rm -r build bin