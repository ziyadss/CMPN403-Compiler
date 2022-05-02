rm -r build bin
mkdir build bin
lex -o build/lex.yy.c src/lexer.l
yacc -o build/y.tab.c -d src/parser.y
gcc -Wall -Wextra -o bin/compiler.exe build/lex.yy.c build/y.tab.c
echo "Starting..."
./bin/compiler.exe source.c
echo "Done!"