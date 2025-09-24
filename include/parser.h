#ifndef PARSER_H
#define PARSER_H
#include "global.h"

/* FUNÇÕES DO ANALISADOR SINTÁTICO */

void consume( TAtom atom );
// <program> ::= program <identifier> ‘;‘ <block> ‘.’
void program(); 
//<block> ::= <variable_declaration_part> <statement_part>
void block();  
 //<variable_declaration_part> ::= [ var <variable_declaration> ‘;’  { <variable_declaration> ‘;’ } ] 
void variable_declaration_part();
//<variable_declaration> ::= identifier { ‘,’ identifier } ‘:’ <type>
void variable_declaration(); 
//<type> ::= char | integer | boolean
void type(); 
//<statement_part> ::= begin <statement> { ‘;’ <statement> } end
void statement_part(); 
// <statement> ::=   <assignment_statement>  |  <read_statement> | <write_statement> | <if_statement> | <while_statement> | <statement_part>
void statement();
//<assignment_statement> ::= <variable> ‘:=’ <expression>
void assignment_statement();
//<read_statement> ::= read ‘(’ <variable> { ‘,’ <variable> } ‘)’
void read_statement();
//<write_statement> ::= write ‘(’ <variable> { ‘,’ <variable> } ‘)’
void write_statement();
//<if_statement> ::= if <expression> then <statement> [ else <statement> ]
void if_statement();
//<while_statement> ::= while <expression> do <statement>
void while_statement();
//<expression> ::= <simple_expression> [ <relational_operator> <simple expression> ] 
void expression(); 
//<simple_expression> ::= <term> { <adding_operator> <term> }
void simple_expression();
//<term> ::= <factor> { <multiplying_operator> <factor> } 
void term(); 
//<factor> ::= identifier | constint | constchar | ‘(’ <expression> ‘)’ | not <factor> | true | false
void factor();
//<relational_operator> ::= ‘<>’ | ‘<’ | ‘<=’ | ‘>=’ | ‘>’ | ‘=’ | or | and
void relational_operator(); 
//<adding operator> ::=  ‘+’ | ‘-’
void adding_operator(); 
//<multiplying_operator> ::= ‘*’ | div
void multiplying_operator();
//Função de entrada no analisadro sintático
void syntactic_analysis();

//Função para printar o átomo esperado
const char* print_expected_atom(TAtom atom);
//Função para printar o átomo de saída
const char* print_atom(TAtom atom);

#endif