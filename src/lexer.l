%{
    #include "y.tab.h"
%}

%option nounput noinput
%option yylineno

D   [0-9]
H   [0-9a-fA-F]

%%

    /* Comments and whitespace ignored. */
    /* TODO: Fix comments ending with '**\/' not ending. */
\/\*((\*[^\/])|[^*])*\*\/           ;
\/\/.*                              ;
[ \t\r\n]+                          ;

    /* Keywords. */
"const"                             return CONST;

"bool"                              return BOOL;
"char"                              return CHAR;
"double"                            return DOUBLE;
"float"                             return FLOAT;
"int"                               return INT;
"long"                              return LONG;
"short"                             return SHORT;
"signed"                            return SIGNED;
"string"                            return STRING;
"unsigned"                          return UNSIGNED;
"void"                              return VOID;

"break"                             return BREAK;
"case"                              return CASE;
"continue"                          return CONTINUE;
"default"                           return DEFAULT;
"do"                                return DO;
"else"                              return ELSE;
"enum"                              return ENUM;
"for"                               return FOR;
"if"                                return IF;
"return"                            return RETURN;
"switch"                            return SWITCH;
"while"                             return WHILE;

"try"                               return TRY;
"catch"                             return CATCH;
"finally"                           return FINALLY;
"throw"                             return THROW;

    /* Operators. */
":"                                 return COLON;
","                                 return COMMA;
"{"                                 return LBRACE;
"("                                 return LPAREN;
"?"                                 return QUESTION;
"}"                                 return RBRACE;
")"                                 return RPAREN;
";"                                 return SEMICOLON;

"+="                                return ADD_ASSIGN;
"&="                                return AND_ASSIGN;
"="                                 return ASSIGN;
"/="                                return DIV_ASSIGN;
"%="                                return MOD_ASSIGN;
"*="                                return MUL_ASSIGN;
"|="                                return OR_ASSIGN;
"<<="                               return SHL_ASSIGN;
">>="                               return SHR_ASSIGN;
"-="                                return SUB_ASSIGN;
"^="                                return XOR_ASSIGN;

"+"                                 return ADD;
"&&"                                return AND;
"&"                                 return BIT_AND;
"~"                                 return BIT_NOT;
"|"                                 return BIT_OR;
"--"                                return DEC;
"/"                                 return DIV;
"=="                                return EQ;
">="                                return GE;
">"                                 return GT;
"++"                                return INC;
"<="                                return LE;
"<"                                 return LT;
"%"                                 return MOD;
"*"                                 return MUL;
"!="                                return NE;
"!"                                 return NOT;
"||"                                return OR;
"<<"                                return SHL;
">>"                                return SHR;
"-"                                 return SUB;
"^"                                 return XOR;

    /* Boolean, numeric, character, and string literals. */
"false"                             { yylval.boolValue = 0;                             return FALSE; }
"true"                              { yylval.boolValue = 1;                             return TRUE;  }

0[bB][01]+                          { yylval.intValue = strtol(yytext + 2, NULL, 2);    return INT_LITERAL; }
0[0-7]*                             { yylval.intValue = strtol(yytext, NULL, 8);        return INT_LITERAL; }
[1-9]{D}*                           { yylval.intValue = strtol(yytext, NULL, 10);       return INT_LITERAL; }
0[xX]{H}+                           { yylval.intValue = strtol(yytext, NULL, 16);       return INT_LITERAL; }

{D}+[eE][+-]?{D}+                   { yylval.floatValue = strtod(yytext, NULL);         return FLOAT_LITERAL; }
{D}+\.([eE][+-]?{D}+)?              { yylval.floatValue = strtod(yytext, NULL);         return FLOAT_LITERAL; }
{D}*\.{D}+([eE][+-]?{D}+)?          { yylval.floatValue = strtod(yytext, NULL);         return FLOAT_LITERAL; }
0[xX]{H}+\.?[pP][+-]?{H}+           { yylval.floatValue = strtod(yytext, NULL);         return FLOAT_LITERAL; }
0[xX]{H}*\.{H}+[pP][+-]?{H}+        { yylval.floatValue = strtod(yytext, NULL);         return FLOAT_LITERAL; }

\'[^']\'                            { yylval.charValue = yytext[1];                     return CHAR_LITERAL; }
\"[^"]*\"                           { 
                                        yylval.stringValue = malloc(yyleng);    // Freed after use in parser.
                                        strncpy(yylval.stringValue, yytext + 1, yyleng - 2);
                                        yylval.stringValue[yyleng - 2] = '\0';
                                                                                        return STRING_LITERAL;
                                        // Do we need to do this, or is yytext validity time sufficient?
                                        // If we don't need, just do this.
                                        // yytext[yyleng - 2] = '\0';
                                        // yylval.stringValue =  yytext + 1;

                                        // Possible solution: Insert into symbol table, at end, loop over it and free strings.
                                    }
    /* Identifier. */
[a-zA-Z_][a-zA-Z0-9_]*              { 
                                        yylval.stringValue = malloc(yyleng + 1);    // Freed after use in parser.
                                        strncpy(yylval.stringValue, yytext, yyleng);
                                        yylval.stringValue[yyleng] = '\0';
                                                                                        return IDENTIFIER;
                                    }

.                                   printf("Unmatched: '%s'\n", yytext);

%%
