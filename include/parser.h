#ifndef PARSER_H
#define PARSER_H
#include "global.h"

void consome( TAtomo atomo );
void program(); 
void block(); 
void variable_declaration_part();
void variable_declaration();
void type();
void statement_part(); 
void statement();
void assignment_statement();
void read_statement();
void write_statement();
void if_statement();
void while_statement();
void expression(); 
void simple_expression();
void term(); 
void factor();
void relational_operator(); 
void adding_operator(); 
void multiplying_operator();
void analyse();

const char* imprimir_atomo(TAtomo atomo);

#endif