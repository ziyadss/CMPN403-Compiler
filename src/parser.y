%{
    #include <stdio.h>
    int yylex();
    int yyerror(const char *s) { fprintf(stderr, "Error: %s\n", s); return 1; }
    int yywrap() { return 1; }
    extern FILE *yyin;
%}

%union
{
    int intValue;
    double floatValue;
    char charValue;
    char *stringValue;
}

    /* Keywords. */
%token CONST
%token BOOL CHAR DOUBLE FLOAT INT LONG SHORT SIGNED STRING UNSIGNED VOID
%token BREAK CASE CONTINUE DEFAULT DO ELSE ENUM FOR IF RETURN SWITCH WHILE
%token TRY CATCH FINALLY THROW

    /* To resolve dangling elses. */
%nonassoc IF
%nonassoc ELSE

    /* Operators. */
%token COLON COMMA LBRACE LPAREN QUESTION RBRACE RPAREN SEMICOLON
%token ADD_ASSIGN AND_ASSIGN ASSIGN DIV_ASSIGN MOD_ASSIGN MUL_ASSIGN OR_ASSIGN SHL_ASSIGN SHR_ASSIGN SUB_ASSIGN XOR_ASSIGN
%token ADD AND BIT_AND BIT_NOT BIT_OR DEC DIV EQ GE GT INC LE LT MOD MUL NE NOT OR SHL SHR SUB XOR

    /* Boolean, numeric, character, and string literals. */
%token FALSE TRUE <intValue>INT_LITERAL <floatValue>FLOAT_LITERAL <charValue>CHAR_LITERAL <stringValue>STRING_LITERAL

    /* Identifier. */
%token IDENTIFIER

%%

    /* A program consists of a number of top level statements. */
program                 : program top_level_statement
                        | top_level_statement
                        ;

    /* Top level statements only declare or define functions and other language constructs (variables, enums, etc.). */
top_level_statement     : declaration SEMICOLON
                        | function
                        ;

    /* A declaration consists of a type, and optionally initializers. */
declaration             : type_modifier_list initializer_list
                        | type_modifier_list
                        ;

    /* Initializiers can be compounded using commas. */
initializer_list        : initializer_list COMMA initializer
                        | initializer
                        ;

    /* An initializer is an identifier optionally assigned an assignment expression. */
initializer             : IDENTIFIER ASSIGN assign_expression
                        | IDENTIFIER
                        ;

    /* A function consists of type modifiers, an identifier, and optionally a paramater list and/or a body. */
function                : type_modifier_list IDENTIFIER LPAREN parameter_list RPAREN block_statement
                        | type_modifier_list IDENTIFIER LPAREN RPAREN block_statement
                        | type_modifier_list IDENTIFIER LPAREN parameter_list RPAREN SEMICOLON
                        | type_modifier_list IDENTIFIER LPAREN RPAREN SEMICOLON
                        ;

    /* A parameter list is a comma-separated list of parameters. */
parameter_list          : parameter_list COMMA parameter
                        | parameter
                        ;

    /* A parameter is a type, and an optional initializer. */
parameter               : type_modifier_list initializer
                        | type_modifier_list
                        ;

    /* EXPRESSIONS */

    /* Expressions ordered with precedence in mind, following from lecture's guide to enfore precedence with grammar rules.
        Reference: https://en.cppreference.com/w/c/language/operator_precedence

        We move up from the bottom up, never going back down until the last step to avoid reduce conflicts.
        Left-to-right expressions are left-recursive, and right-to-left expressions are right-recursive.
    */

    /* Any expression is a comma-separated list of assignment expressions. */
expression              : expression COMMA assign_expression
                        | assign_expression
                        ;

    /* An assignment expression is either an assignment expression or decays to a ternary expression. */
assign_expression       : IDENTIFIER assignment_op assign_expression
                        | ternary_expression
                        ;

    /* An ternary expression is either a ternary expression or decays to a logical or expression. */
ternary_expression      : or_expression QUESTION expression COLON ternary_expression
                        | or_expression
                        ;

    /* A logical or expression is either a logical or expression or decays to a logical and expression. */
or_expression           : or_expression OR and_expression
                        | and_expression
                        ;

    /* A logical and expression is either a logical and expression or decays to a bitwise or expression. */
and_expression          : and_expression AND bit_or_expression
                        | bit_or_expression
                        ;

    /* A bitwise or expression is either a bitwise or expression or decays to a bitwise xor expression. */
bit_or_expression       : bit_or_expression BIT_OR xor_expression
                        | xor_expression
                        ;

    /* An xor expression is either an xor expression or decays to a bitwise and expression. */
xor_expression          : xor_expression XOR bit_and_expression
                        | bit_and_expression
                        ;

    /* A bitwise and expression is either a bitwise and expression or decays to an equality expression. */
bit_and_expression      : bit_and_expression BIT_AND equal_expression
                        | equal_expression
                        ;

    /* An equality expression is either an equality expression or decays to a comparison expression. */
equal_expression        : equal_expression EQ compare_expression
                        | equal_expression NE compare_expression
                        | compare_expression
                        ;

    /* A comparison expression is either a comparison expression or decays to a shift expression. */
compare_expression      : compare_expression LT shift_expression
                        | compare_expression GT shift_expression
                        | compare_expression LE shift_expression
                        | compare_expression GE shift_expression
                        | shift_expression
                        ;

    /* A shift expression is either a shift expression or decays to an addition expression. */
shift_expression        : shift_expression SHL add_expression
                        | shift_expression SHR add_expression
                        | add_expression
                        ;

    /* An addition expression is either an addition expression or decays to a multiplication expression. */
add_expression          : add_expression ADD mul_expression
                        | add_expression SUB mul_expression
                        | mul_expression
                        ;

    /* A multiplication expression is either a multiplication expression or decays to a prefix expression. */
mul_expression          : mul_expression MUL prefix_expression
                        | mul_expression DIV prefix_expression
                        | mul_expression MOD prefix_expression
                        | prefix_expression
                        ;

    /* A prefix expression is either a prefix expression or decays to a postfix expression. */
prefix_expression       : unary_op prefix_expression
                        | postfix_expression
                        ;

    /* A postfix expression is either a postfix expression (including a function call) or decays to a base expression. */
postfix_expression      : postfix_expression INC
                        | postfix_expression DEC
                        | postfix_expression LPAREN optional_expression RPAREN
                        | base_expression
                        ;

    /* A base expression is either an identifier, a literal, or a parenthesized optional expression. */
base_expression         : IDENTIFIER
                        | literal
                        | LPAREN optional_expression RPAREN
                        ;

    /* An optional expression is either an expression or nothing. */
optional_expression     : expression
                        |
                        ;

    /* STATEMENTS */

    /* A statement is one of a block, selection, iteration, jump, a semicolon optionally preceded by an expression, a try or a declaration followed by a semicolon. */
statement               : block_statement
                        | selection_statement
                        | iteration_statement
                        | jump_statement
                        | optional_expression SEMICOLON
                        | try_statement
                        | declaration SEMICOLON
                        ;

    /* A block statement is a brace-enclosed list of optional block items. */
block_statement         : LBRACE block_item_list RBRACE
                        | LBRACE RBRACE
                        ;

    /* Block items can be compounded, and are statements. */
block_item_list         : block_item_list statement
                        | statement
                        ;

    /* Selection statements are IFs and SWITCHes. */
selection_statement     : IF LPAREN expression RPAREN statement %prec IF
                        | IF LPAREN expression RPAREN statement ELSE statement
                        | SWITCH LPAREN expression RPAREN LBRACE switch_case_list RBRACE
                        ;

    /* A switch case list is a sequence of switch cases. */
switch_case_list        : switch_case_list switch_case
                        | switch_case
                        ;

    /* The two case types of a switch case. */
switch_case             : CASE ternary_expression COLON block_item_list
                        | DEFAULT COLON block_item_list
                        ;

    /* Iteration statements are WHILEs and DO WHILES and FORs */
iteration_statement     : WHILE LPAREN expression RPAREN statement
                        | DO statement WHILE LPAREN expression RPAREN SEMICOLON
                        | FOR LPAREN optional_expression SEMICOLON optional_expression SEMICOLON optional_expression RPAREN statement
                        | FOR LPAREN declaration SEMICOLON optional_expression SEMICOLON optional_expression RPAREN statement
                        ;

    /* Jump statements are ones which affect control flow. */
jump_statement          : CONTINUE SEMICOLON
                        | BREAK SEMICOLON
                        | RETURN optional_expression SEMICOLON
                        | THROW optional_expression SEMICOLON
                        ;

    /* A try statement is a try block followed by one or more catch blocks, and an optional finally block. */
try_statement           : TRY block_statement catch_block_list
                        | TRY block_statement catch_block_list FINALLY block_statement
                        ;

    /* A catch block list is a sequence of CATCHes and statements, the last which can be a catch-all. */
catch_block_list        : non_final_catch_block CATCH block_statement
                        | non_final_catch_block
                        | CATCH block_statement
                        ;

non_final_catch_block   : non_final_catch_block CATCH LPAREN type_modifier_list IDENTIFIER RPAREN block_statement
                        | CATCH LPAREN type_modifier_list IDENTIFIER RPAREN block_statement
                        ;

    /* MISCELLANEOUS */

    /* A sequence of type modifiers. Need semantic checks.*/
type_modifier_list      : type_modifier_list type_modifier
                        | type_modifier_list CONST
                        | type_modifier
                        | CONST
                        ;

    /* A type modifier is a data type. */
type_modifier           : BOOL
                        | CHAR
                        | DOUBLE
                        | FLOAT
                        | INT
                        | LONG
                        | SHORT
                        | SIGNED
                        | STRING
                        | UNSIGNED
                        | VOID
                        | enum_type
                        ;

    /* An enum type is ENUM followed by an identifier, or an (optionally anonymous) enum declaration. */
enum_type               : ENUM IDENTIFIER LBRACE initializer_list RBRACE
                        | ENUM LBRACE initializer_list RBRACE
                        | ENUM IDENTIFIER
                        ;

    /* Unary operators. */
unary_op                : INC
                        | DEC
                        | ADD
                        | SUB
                        | NOT
                        | BIT_NOT
                        ;

    /* Assignment perators. */
assignment_op           : ADD_ASSIGN
                        | AND_ASSIGN
                        | ASSIGN
                        | DIV_ASSIGN
                        | MOD_ASSIGN
                        | MUL_ASSIGN
                        | OR_ASSIGN
                        | SHL_ASSIGN
                        | SHR_ASSIGN
                        | SUB_ASSIGN
                        | XOR_ASSIGN
                        ;

    /* Literals. */
literal                 : FALSE
                        | TRUE
                        | INT_LITERAL
                        | FLOAT_LITERAL
                        | CHAR_LITERAL
                        | STRING_LITERAL                        { free(yylval.stringValue); }
                        ;

%%

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL)
    {
        fprintf(stderr, "Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    yyparse();

    fclose(yyin);

    return 0;
}
