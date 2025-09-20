#include "lexer.c"
#include "../include/parser.h"

void consome( TAtomo atomo ){
    if( lookahead == atomo ){
      if (lookahead == IDENTIFICADOR){
        printf("\n# %2d:identifier : %s", info_atomo.linha, info_atomo.atributo.id);
      } else {
        printf("\n# %2d:%s", info_atomo.linha, imprimir_atomo(lookahead)); // Imprime o token ATUAL
      }
      info_atomo = obterAtomo();  // Depois obtém o próximo
      lookahead = info_atomo.atomo;
    }
    else{
        printf("\n# %2d:Erro sintatico: esperado [%s] encontrado [%s]\n",info_atomo.linha, imprimir_atomo(atomo), imprimir_atomo(lookahead));
        //printf("\n#%2d:Erro sintatico\n ", info_atomo.linha);
        exit(1);
    }
}

/* ------------------------
 * FUNÇÕES DO PARSER
 * ------------------------ */
void program(){

  if(lookahead==COMENTARIO){
    consome(COMENTARIO);
  }

  consome(PROGRAM); //Consumir 'program' 
  consome(IDENTIFICADOR); //Consumir um identificador
  consome(PONTO_VIRGULA); //Consumir ';'
  block(); //Vai para a função de block
  consome(PONTO); //Consome um ponto 

  if(lookahead==COMENTARIO){
    consome(COMENTARIO);
  }
  
}


void block(){

    variable_declaration_part(); //Vai para a função de declaração de variáveis
    statement_part(); //Vai para função lista de statements - <statement_part>
}


void variable_declaration_part(){

  if(lookahead==VAR){
    consome(VAR);

    variable_declaration();
    consome(PONTO_VIRGULA);

    while (lookahead==IDENTIFICADOR)
    {
      variable_declaration();
      consome(PONTO_VIRGULA);
    }
  } 
}

void variable_declaration(){

  consome(IDENTIFICADOR);

  while (lookahead == VIRGULA){
    consome(VIRGULA);
    consome(IDENTIFICADOR);
  }

  consome(DOIS_PONTOS);
  type();
  
}

void type(){
  if((lookahead==INTEGER || lookahead==BOOLEAN || lookahead==CHAR)){
                //erro_sintatico("type inválido");
    consome(lookahead);
  }
  else
  {
    printf("\n#%2d:Erro sintatico: type esperado\n", info_atomo.linha);
    exit(1);
  }
  
}

void statement_part(){

    consome(BEGIN);
    statement();
    while(lookahead==PONTO_VIRGULA){
        //consome(lookahead);
        consome(PONTO_VIRGULA);
        statement();
    }
    consome(END);

}

void statement(){

  if(lookahead==COMENTARIO){
    consome(COMENTARIO);
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
    consome(COMENTARIO);
  }
}

void assignment_statement(){
  consome(IDENTIFICADOR);
  consome(ATRIBUICAO);
  expression();
}

void read_statement(){
  consome(READ);
  consome(ABRE_PAR);
  consome(IDENTIFICADOR);
  while (lookahead==VIRGULA){
    consome(VIRGULA);
    consome(IDENTIFICADOR);
  }
  consome(FECHA_PAR);
}

void write_statement(){
  consome(WRITE);
  consome(ABRE_PAR);
  consome(IDENTIFICADOR);

  while (lookahead==VIRGULA){
    consome(VIRGULA);
    consome(IDENTIFICADOR);
  }
  consome(FECHA_PAR);
  
}

void if_statement(){
  consome(IF);
  expression();
  consome(THEN);
  statement();
  if(lookahead==ELSE){
    consome(ELSE);
    statement();
  }
}

void while_statement(){
  consome(WHILE);
  expression();
  consome(DO);
  statement();
}

/* ------------------------
 * EXPRESSÕES
 * ------------------------ */
void expression(){
    simple_expression();

    if(lookahead==IGUAL || lookahead==DIFERENTE || lookahead==MENOR ||
        lookahead==MENOR_IGUAL || lookahead==MAIOR || lookahead==MAIOR_IGUAL ||
        lookahead==AND || lookahead==OR){
    
        relational_operator();
        simple_expression();
      }
}



void simple_expression(){

   term();
     
  while(lookahead==SOMA || lookahead==SUBTRACAO){
      adding_operator();
      term();
  }
}

void term(){

  factor();

  while(lookahead==MULTIPLICACAO || lookahead==DIV){
    multiplying_operator();
    factor();
  }
}

void factor(){
    
  if(lookahead==IDENTIFICADOR || lookahead==NUMERO || lookahead==CARACTER
        || lookahead==TRUE || lookahead==FALSE){
        consome(lookahead);
    } 
    
  else if(lookahead==ABRE_PAR){
      consome(ABRE_PAR);
      expression();
      consome(FECHA_PAR);
  } 
  
  else if(lookahead==NOT){
      consome(NOT);
      factor();
  } 
}

void relational_operator(){ 
  consome(lookahead); 
}

void adding_operator(){ 
  consome(lookahead); 
}

void multiplying_operator(){ 
  consome(lookahead); 
}

/* ------------------------
 * FUNÇÃO DE ENTRADA DO PARSER
 * ------------------------ */
void analyse(){
  
    program();

    if(lookahead!=EOS){
      printf("\n# %2d:Erro sintático: codigo extra após o fim do program\n", info_atomo.linha);
      exit(1);
    }

    printf("\n%d linhas analisadas, programa sintaticamente correto\n", info_atomo.linha);
}


const char* imprimir_atomo(TAtomo atomo) {

    switch(atomo) {
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
        case ERRO: return "erro";
        
        default: return "desconhecido";
    }
}