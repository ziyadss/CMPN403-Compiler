%{
    #include <stdio.h>

    #include "../src/symbol_table_interface.c"
    #include "../src/ast.c"
    #include "../src/main.c"

    int yylex();
    int yyerror(const char *s) { fprintf_s(stderr, "Error: %s\n", s); return 1; }
    int yywrap() { return 1; }
    extern int yylineno;
%}

%union
{
    int intValue;
    double floatValue;
    char charValue;
    char *stringValue;
    _Bool boolValue;
    struct AST_Node *nodePointer;
    /* enum OPERATION operation; */
    int operation;
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
%token <boolValue>FALSE <boolValue>TRUE <intValue>INT_LITERAL <floatValue>FLOAT_LITERAL <charValue>CHAR_LITERAL <stringValue>STRING_LITERAL

    /* Identifier. */
%token <stringValue>IDENTIFIER

%type <nodePointer>expression assign_expression ternary_expression or_expression and_expression bit_or_expression xor_expression bit_and_expression equal_expression compare_expression shift_expression add_expression mul_expression prefix_expression postfix_expression base_expression optional_expression

%type <nodePointer>literal

%type <operation>unary_op assignment_op

%type <nodePointer>initializer initializer_list function top_level_statement declaration
%type <nodePointer>function_start function_start_par function_start_no_par
%type <nodePointer> block_statement block_item_list statement 
// selection_statement iteration_statement jump_statement try_statement

%%

    /* A program consists of a number of top level statements. */
program                 : program top_level_statement                   { program_append($2); }
                        | top_level_statement                           { program_append($1); }
                        ;

    /* Top level statements only declare or define functions and other language constructs (variables, enums, etc.). */
top_level_statement     : declaration SEMICOLON
                        | function
                        ;

    /* A declaration consists of a type, and optionally initializers. */
declaration             : type_modifier_list initializer_list       { $$ = $2; }
                        | type_modifier_list                        { $$ = NULL; }
                        ;

    /* Initializiers can be compounded using commas. */
initializer_list        : initializer_list COMMA initializer        { $$ = operation_node(COMMA_OP, $1, $3); }
                        | initializer
                        ;

    /* An initializer is an identifier optionally assigned an assignment expression. */
initializer             : IDENTIFIER ASSIGN assign_expression       { insert($1); $$ = operation_node(ASSIGN_OP, identifier_node($1), $3); }
                        | IDENTIFIER                                { insert($1); $$ = identifier_node($1); }
                        ;

    /* A function consists of type modifiers, an identifier, and optionally a paramater list and/or a body. */

function_start          : type_modifier_list IDENTIFIER { insert($2); } LPAREN      {$$ = identifier_node($2);}
                        ;

function_start_par      : function_start { scope_down(); } parameter_list RPAREN
                        ;

function_start_no_par   : function_start RPAREN
                        ;

function                : function_start_par block_statement                        { scope_up(); $$ = operation_node(FUNC_DECL, $1, $2);}
                        | function_start_no_par { scope_down(); } block_statement   { scope_up(); $$ = operation_node(FUNC_DECL, $1, $3);}
                        | function_start_par RPAREN SEMICOLON                       { scope_up(); $$ = operation_node(FUNC_DECL, $1, NULL);}
                        | function_start_no_par SEMICOLON                           { $$ = operation_node(FUNC_DECL, $1, NULL);}
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
expression              : expression COMMA assign_expression            { $$ = operation_node(COMMA_OP, $1, $3); }
                        | assign_expression
                        ;

    /* An assignment expression is either an assignment expression or decays to a ternary expression. */
assign_expression       : IDENTIFIER assignment_op assign_expression    { $$ = operation_node($2, identifier_node($1), $3); }
                        | ternary_expression
                        ;

    /* An ternary expression is either a ternary expression or decays to a logical or expression. */
ternary_expression      : or_expression QUESTION expression COLON ternary_expression    {printf_s("Unreachable\n");}
                        | or_expression
                        ;

    /* A logical or expression is either a logical or expression or decays to a logical and expression. */
or_expression           : or_expression OR and_expression               { $$ = operation_node(OR_OP, $1, $3); }
                        | and_expression
                        ;

    /* A logical and expression is either a logical and expression or decays to a bitwise or expression. */
and_expression          : and_expression AND bit_or_expression          { $$ = operation_node(AND_OP, $1, $3); }
                        | bit_or_expression
                        ;

    /* A bitwise or expression is either a bitwise or expression or decays to a bitwise xor expression. */
bit_or_expression       : bit_or_expression BIT_OR xor_expression       { $$ = operation_node(BIT_OR_OP, $1, $3); }
                        | xor_expression
                        ;

    /* An xor expression is either an xor expression or decays to a bitwise and expression. */
xor_expression          : xor_expression XOR bit_and_expression         { $$ = operation_node(XOR_OP, $1, $3); }
                        | bit_and_expression
                        ;

    /* A bitwise and expression is either a bitwise and expression or decays to an equality expression. */
bit_and_expression      : bit_and_expression BIT_AND equal_expression   { $$ = operation_node(BIT_AND_OP, $1, $3); }
                        | equal_expression
                        ;

    /* An equality expression is either an equality expression or decays to a comparison expression. */
equal_expression        : equal_expression EQ compare_expression        { $$ = operation_node(EQ_OP, $1, $3); }
                        | equal_expression NE compare_expression        { $$ = operation_node(NE_OP, $1, $3); }
                        | compare_expression
                        ;

    /* A comparison expression is either a comparison expression or decays to a shift expression. */
compare_expression      : compare_expression LT shift_expression        { $$ = operation_node(LT_OP, $1, $3); }
                        | compare_expression GT shift_expression        { $$ = operation_node(GT_OP, $1, $3); }
                        | compare_expression LE shift_expression        { $$ = operation_node(LE_OP, $1, $3); }
                        | compare_expression GE shift_expression        { $$ = operation_node(GE_OP, $1, $3); }
                        | shift_expression
                        ;

    /* A shift expression is either a shift expression or decays to an addition expression. */
shift_expression        : shift_expression SHL add_expression           { $$ = operation_node(SHL_OP, $1, $3); }
                        | shift_expression SHR add_expression           { $$ = operation_node(SHR_OP, $1, $3); }
                        | add_expression
                        ;

    /* An addition expression is either an addition expression or decays to a multiplication expression. */
add_expression          : add_expression ADD mul_expression             { $$ = operation_node(ADD_OP, $1, $3); }
                        | add_expression SUB mul_expression             { $$ = operation_node(SUB_OP, $1, $3); }
                        | mul_expression
                        ;

    /* A multiplication expression is either a multiplication expression or decays to a prefix expression. */
mul_expression          : mul_expression MUL prefix_expression          { $$ = operation_node(MUL_OP, $1, $3); }
                        | mul_expression DIV prefix_expression          { $$ = operation_node(DIV_OP, $1, $3); }
                        | mul_expression MOD prefix_expression          { $$ = operation_node(MOD_OP, $1, $3); }
                        | prefix_expression
                        ;

    /* A prefix expression is either a prefix expression or decays to a postfix expression. */
prefix_expression       : unary_op prefix_expression                    { $$ = operation_node($1, $2, NULL); }
                        | postfix_expression
                        ;

    /* A postfix expression is either a postfix expression (including a function call) or decays to a base expression. */
postfix_expression      : postfix_expression INC                        { $$ = operation_node(INC_OP, $1, NULL); }
                        | postfix_expression DEC                        { $$ = operation_node(DEC_OP, $1, NULL); }
                        | IDENTIFIER LPAREN optional_expression RPAREN  { $$ = call_node(identifier_node($1), $3); }
                        | base_expression
                        ;

    /* A base expression is either an identifier, a literal, or a parenthesized optional expression. */
base_expression         : IDENTIFIER                                    { $$ = identifier_node($1); }
                        | literal
                        | LPAREN optional_expression RPAREN             { $$ = $2; }
                        ;

    /* An optional expression is either an expression or nothing. */
optional_expression     : expression
                        | { $$ = NULL; }
                        ;

    /* STATEMENTS */

    /* A statement is one of a block, selection, iteration, jump, a semicolon optionally preceded by an expression, a try or a declaration followed by a semicolon. */
statement               : { scope_down();} block_statement  { scope_up(); $$ = $2; }
                        | selection_statement               {$$ = NULL;}
                        | iteration_statement               {$$ = NULL;}
                        | jump_statement                    {$$ = NULL;}
                        | try_statement                     {$$ = NULL;}
                        | optional_expression SEMICOLON
                        | declaration SEMICOLON
                        ;

    /* A block statement is a brace-enclosed list of optional block items. */
block_statement         : LBRACE block_item_list RBRACE         { $$ = $2; }
                        | LBRACE RBRACE                         { $$ = NULL; }
                        ;

    /* Block items can be compounded, and are statements. */
block_item_list         : block_item_list statement             { $$ = add_statement($1, $2); }
                        | statement                             { $$ = block_node($1); }
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

try_start               : TRY { scope_down();} block_statement { scope_up(); }
                        ;

try_statement           : try_start catch_block_list
                        | try_start catch_block_list FINALLY { scope_down();} block_statement { scope_up(); }
                        ;

    /* A catch block list is a sequence of CATCHes and statements, the last which can be a catch-all. */
catch_block_list        : non_final_catch_block CATCH { scope_down();} block_statement { scope_up(); }
                        | non_final_catch_block
                        | CATCH { scope_down();} block_statement { scope_up(); }
                        ;

non_final_catch_block   : non_final_catch_block CATCH LPAREN type_modifier_list IDENTIFIER RPAREN { scope_down();} block_statement { scope_up(); }
                        | CATCH LPAREN type_modifier_list IDENTIFIER RPAREN { scope_down();} block_statement { scope_up(); }
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
unary_op                : INC                       {$$ = INC_OP;}
                        | DEC                       {$$ = DEC_OP;}
                        | ADD                       {$$ = ADD_OP;}
                        | SUB                       {$$ = SUB_OP;}
                        | NOT                       {$$ = NOT_OP;}
                        | BIT_NOT                   {$$ = BIT_NOT_OP;}
                        ;

    /* Assignment perators. */
assignment_op           : ADD_ASSIGN                {$$ = ADD_ASSIGN_OP;}
                        | AND_ASSIGN                {$$ = AND_ASSIGN_OP;}
                        | ASSIGN                    {$$ = ASSIGN_OP;}
                        | DIV_ASSIGN                {$$ = DIV_ASSIGN_OP;}
                        | MOD_ASSIGN                {$$ = MOD_ASSIGN_OP;}
                        | MUL_ASSIGN                {$$ = MUL_ASSIGN_OP;}
                        | OR_ASSIGN                 {$$ = OR_ASSIGN_OP;}
                        | SHL_ASSIGN                {$$ = SHL_ASSIGN_OP;}
                        | SHR_ASSIGN                {$$ = SHR_ASSIGN_OP;}
                        | SUB_ASSIGN                {$$ = SUB_ASSIGN_OP;}
                        | XOR_ASSIGN                {$$ = XOR_ASSIGN_OP;}
                        ;

    /* Literals. */
literal                 : FALSE                             {$$ = bool_node(0);}
                        | TRUE                              {$$ = bool_node(1);}
                        | INT_LITERAL                       {$$ = int_node($1);}
                        | FLOAT_LITERAL                     {$$ = float_node($1);}
                        | CHAR_LITERAL                      {$$ = char_node($1);}
                        | STRING_LITERAL                    {$$ = string_node($1);}
                        ;

%%
