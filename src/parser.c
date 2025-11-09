#include "lexer.c"
#include "../include/parser.h"
#include "../include/hash.h"

/* ANALISADOR SINTÁTICO */

//Função consome
void consume( TAtom atom ){
    if( lookahead == atom ){

      switch (lookahead)
      {
      case IDENTIFICADOR:
        printf("\n# %2d:identifier : %s", info_atom.line, info_atom.attribute.id);
        break;

      case NUMERO:
        printf("\n# %2d:number : %d", info_atom.line, info_atom.attribute.number);
        break;
      
      default:
        printf("\n# %2d:%s", info_atom.line, print_atom(lookahead)); // Imprime o token ATUAL
        break;
      }

      info_atom = getAtom();
      lookahead = info_atom.atom;
    }
    else{
        printf("\n# %2d:Erro sintatico: esperado [%s] encontrado [%s]\n",info_atom.line, print_expected_atom(atom), print_expected_atom(lookahead));
        exit(1);
    }
}

// <program> ::= program <identifier> ‘;‘ <block> ‘.’
void program(){

  if(lookahead==COMENTARIO){
    consume(COMENTARIO);
  }

  consume(PROGRAM); 
  consume(IDENTIFICADOR); 
  consume(PONTO_VIRGULA); 
  block(); 
  consume(PONTO); 

  if(lookahead==COMENTARIO){
    consume(COMENTARIO);
  }
  
}

//<block> ::= <variable_declaration_part> <statement_part>
void block(){

    variable_declaration_part(); 
    statement_part(); 
}


//<variable_declaration_part> ::= [ var <variable_declaration> ';'  { <variable_declaration> ';' } ] 
void variable_declaration_part(){

  // Consumir comentários antes de var
  while(lookahead == COMENTARIO){
    consume(COMENTARIO);
  }

  if(lookahead==VAR){
    consume(VAR);

    // Consumir comentários após var
    while(lookahead == COMENTARIO){
      consume(COMENTARIO);
    }

    variable_declaration();
    consume(PONTO_VIRGULA);

    while (lookahead==IDENTIFICADOR)
    {
      variable_declaration();
      consume(PONTO_VIRGULA);
    }
  } 

  else if(lookahead == IDENTIFICADOR){
    printf("\n# %2d:Erro lexico: palavra reservada 'var' deve ser em minusculo\n", info_atom.line);
    exit(1);
  }
}

//<variable_declaration> ::= identifier { ‘,’ identifier } ‘:’ <type>
void variable_declaration(){

  char id[16];

  strcpy(id, info_atom.attribute.id);
  consume(IDENTIFICADOR);

  // Verifica se já foi declarado o símbolo
  if(searchSymbol(&tabelaSimbolos, id) != NULL) {
      printf("\n# %2d:Erro semantico: variavel '%s' ja declarada\n", 
              info_atom.line, id);
      exit(1);
  }

  // Insere na tabela
  insertSymbol(&tabelaSimbolos, id, nextAddress++);

  while (lookahead == VIRGULA){
    consume(VIRGULA);
    strcpy(id, info_atom.attribute.id);
    consume(IDENTIFICADOR);

    // Verifica duplicação
    if(searchSymbol(&tabelaSimbolos, id) != NULL) {
        printf("\n# %2d:Erro semantico: variavel '%s' ja declarada\n", 
                info_atom.line, id);
        exit(1);
    }
    
    insertSymbol(&tabelaSimbolos, id, nextAddress++);
  }

  consume(DOIS_PONTOS);
  type();
  
}

//<type> ::= char | integer | boolean
void type(){
  if((lookahead==INTEGER || lookahead==BOOLEAN || lookahead==CHAR)){
    consume(lookahead);
  }
  else
  {
    printf("\n#%2d:Erro sintatico: type esperado\n", info_atom.line);
    exit(1);
  }
  
}

//<statement_part> ::= begin <statement> { ‘;’ <statement> } end
void statement_part(){

    consume(BEGIN);
    statement();
    while(lookahead==PONTO_VIRGULA){
        //consume(lookahead);
        consume(PONTO_VIRGULA);
        statement();
    }
    consume(END);

}

// <statement> ::=   <assignment_statement>  |  <read_statement> | <write_statement> | <if_statement> | <while_statement> | <statement_part>
void statement(){

  if(lookahead==COMENTARIO){
    consume(COMENTARIO);
  }

   switch (lookahead)
   {
   case IDENTIFICADOR:
    assignment_statement();
    break;
    
   case READ:
    read_statement();
    break;

   case WRITE:
    write_statement();
    break;

   case IF:
    if_statement();
    break;
  
   case WHILE:
    while_statement();
    break;

   
   default:
    statement_part();
    break;
   }

     if(lookahead==COMENTARIO){
    consume(COMENTARIO);
  }
}

//<assignment_statement> ::= <variable> ‘:=’ <expression>
void assignment_statement(){

  char id[16];
  strcpy(id, info_atom.attribute.id);

  consume(IDENTIFICADOR);

  // Verifica se a variável foi declarada
  if(searchSymbol(&tabelaSimbolos, id) == NULL) {
      printf("\n# %2d:Erro semantico: variavel '%s' nao declarada\n", 
              info_atom.line, id);
      exit(1);
  }

  consume(ATRIBUICAO);
  expression();

}

//<read_statement> ::= read ‘(’ <variable> { ‘,’ <variable> } ‘)’
void read_statement(){

  char id[16];

  consume(READ);
  consume(ABRE_PAR);

  strcpy(id, info_atom.attribute.id);
  consume(IDENTIFICADOR);

  if(searchSymbol(&tabelaSimbolos, id) == NULL) {
        printf("\n# %2d:Erro semantico: variavel '%s' nao declarada\n", 
               info_atom.line, id);
        exit(1);
  }

  while (lookahead==VIRGULA){
    consume(VIRGULA);
    strcpy(id, info_atom.attribute.id);
    consume(IDENTIFICADOR);

    if(searchSymbol(&tabelaSimbolos, id) == NULL) {
        printf("\n# %2d:Erro semantico: variavel '%s' nao declarada\n", 
               info_atom.line, id);
        exit(1);
    }
  }

  consume(FECHA_PAR);

}

//<write_statement> ::= write ‘(’ <variable> { ‘,’ <variable> } ‘)’
void write_statement(){

  char id[16];

  consume(WRITE);
  consume(ABRE_PAR);

  strcpy(id, info_atom.attribute.id);
  consume(IDENTIFICADOR);

  if(searchSymbol(&tabelaSimbolos, id) == NULL) {
        printf("\n# %2d:Erro semantico: variavel '%s' nao declarada\n", 
               info_atom.line, id);
        exit(1);
  }

  while (lookahead==VIRGULA){
    consume(VIRGULA);
    strcpy(id, info_atom.attribute.id);
    consume(IDENTIFICADOR);

    if(searchSymbol(&tabelaSimbolos, id) == NULL) {
        printf("\n# %2d:Erro semantico: variavel '%s' nao declarada\n", 
               info_atom.line, id);
        exit(1);
    }
  }

  consume(FECHA_PAR);
  
}

//<if_statement> ::= if <expression> then <statement> [ else <statement> ]
void if_statement(){

  consume(IF);
  expression();
  consume(THEN);
  statement();

  if(lookahead==ELSE){
    consume(ELSE);
    statement();
  }

}

//<while_statement> ::= while <expression> do <statement>
void while_statement(){

  consume(WHILE);
  expression();
  consume(DO);
  statement();

}

//<expression> ::= <simple_expression> [ <relational_operator> <simple expression> ] 
void expression(){

    simple_expression();

    if(lookahead==IGUAL || lookahead==DIFERENTE || lookahead==MENOR ||
        lookahead==MENOR_IGUAL || lookahead==MAIOR || lookahead==MAIOR_IGUAL ||
        lookahead==AND || lookahead==OR){
    
        relational_operator();
        simple_expression();
      }
}


//<simple_expression> ::= <term> { <adding_operator> <term> }
void simple_expression(){

   term();
     
  while(lookahead==SOMA || lookahead==SUBTRACAO){
      adding_operator();
      term();
  }
}

//<term> ::= <factor> { <multiplying_operator> <factor> } 
void term(){

  factor();

  while(lookahead==MULTIPLICACAO || lookahead==DIV){
    multiplying_operator();
    factor();
  }
}

//<factor> ::= identifier | constint | constchar | ‘(’ <expression> ‘)’ | not <factor> | true | false
void factor(){

  char id[16];

   if(lookahead==IDENTIFICADOR){
        strcpy(id, info_atom.attribute.id);
        
        // Verifica se foi declarada
        if(searchSymbol(&tabelaSimbolos, id) == NULL) {
            printf("\n# %2d:Erro semantico: variavel '%s' nao declarada\n", 
                   info_atom.line, id);
            exit(1);
        }
        
        consume(lookahead);
    }
    
  if(lookahead==NUMERO || lookahead==CARACTER
        || lookahead==TRUE || lookahead==FALSE){
        consume(lookahead);
    } 
    
  else if(lookahead==ABRE_PAR){

      consume(ABRE_PAR);
      expression();
      consume(FECHA_PAR);

  } 
  
  else if(lookahead==NOT){

      consume(NOT);
      factor();

  } 
}

//<relational_operator> ::= ‘<>’ | ‘<’ | ‘<=’ | ‘>=’ | ‘>’ | ‘=’ | or | and
void relational_operator(){ 
  consume(lookahead); 
}

//<adding operator> ::=  ‘+’ | ‘-’
void adding_operator(){ 
  consume(lookahead); 
}

//<multiplying_operator> ::= ‘*’ | div
void multiplying_operator(){ 
  consume(lookahead); 
}

/* ------------------------
 * FUNÇÃO DE ENTRADA DO PARSER
 * ------------------------ */
void syntactic_analysis(){
  
    program();

    if(lookahead!=EOS){
      printf("\n# %2d:Erro sintático: codigo extra após o fim do program\n", info_atom.line);
      exit(1);
    }
    consume(EOS);

    printf("\n%d lines analisadas, programa sintaticamente correto\n", info_atom.line);
}

//Função para printar o átomo esperado
const char* print_expected_atom(TAtom atom) {

    switch(atom) {
        // Palavras reservadas
        case PROGRAM: return "program";
        case VAR: return "var";
        case BEGIN: return "begin";
        case END: return "end";
        case IF: return "if";
        case THEN: return "then";
        case ELSE: return "else";
        case WHILE: return "while";
        case DO: return "do";
        case READ: return "read";
        case WRITE: return "write";
        case CHAR: return "char";
        case INTEGER: return "integer";
        case BOOLEAN: return "boolean";
        case DIV: return "div";
        case OR: return "or";
        case AND: return "and";
        case NOT: return "not";
        case TRUE: return "true";
        case FALSE: return "false";
        
        // Operadores e pontuação
        case SOMA: return "+";
        case SUBTRACAO: return "-";
        case MULTIPLICACAO: return "*";
        case PONTO_VIRGULA: return ";";
        case DOIS_PONTOS: return ":";
        case ATRIBUICAO: return ":=";
        case ABRE_PAR: return "(";
        case FECHA_PAR: return ")";
        case PONTO: return ".";
        case VIRGULA: return ",";
        case IGUAL: return "=";
        case MAIOR: return ">";
        case MAIOR_IGUAL: return ">=";
        case MENOR: return "<";
        case MENOR_IGUAL: return "<=";
        case DIFERENTE: return "<>";
        
        // Outros átomos
        case IDENTIFICADOR: return "identificador";
        case NUMERO: return "numero";
        case CARACTER: return "caracter";
        case COMENTARIO: return "comentario";
        case EOS: return "fim de arquivo";
        
        default: return "desconhecido";
    }
}

//Função para printar o átomo de saída
const char* print_atom(TAtom atom) {

    switch(atom) {
        // Palavras reservadas
        case PROGRAM: return "program";
        case VAR: return "var";
        case BEGIN: return "begin";
        case END: return "end";
        case IF: return "if";
        case THEN: return "then";
        case ELSE: return "else";
        case WHILE: return "while";
        case DO: return "do";
        case READ: return "read";
        case WRITE: return "write";
        case CHAR: return "char";
        case INTEGER: return "integer";
        case BOOLEAN: return "boolean";
        case DIV: return "div";
        case OR: return "or";
        case AND: return "and";
        case NOT: return "not";
        case TRUE: return "true";
        case FALSE: return "false";
        
        // Operadores e pontuação
        case SOMA: return "soma";
        case SUBTRACAO: return "menos";
        case MULTIPLICACAO: return "multiplicação";
        case PONTO_VIRGULA: return "ponto_virgula";
        case DOIS_PONTOS: return "dois_pontos";
        case ATRIBUICAO: return "atribuição";
        case ABRE_PAR: return "abre_parenteses";
        case FECHA_PAR: return "fecha_parenteses";
        case PONTO: return "ponto";
        case VIRGULA: return "virgula";
        case IGUAL: return "igual";
        case MAIOR: return "maior";
        case MAIOR_IGUAL: return "maior_igual";
        case MENOR: return "menor";
        case MENOR_IGUAL: return "menor_igual";
        case DIFERENTE: return "diferente";
        
        // Outros átomos
        case IDENTIFICADOR: return "identificador";
        case NUMERO: return "numero";
        case CARACTER: return "caracter";
        case COMENTARIO: return "comentario";
        case EOS: return "fim de arquivo";
        
        default: return "desconhecido";
    }
}