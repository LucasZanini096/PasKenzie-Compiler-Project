/******************************************************************************
<expressão> ::= <expressão> ‘+’<termo> | <expressão> ‘–’<termo> | <termo>

Eliminando a recursividade à esquerda
<expressão> ::= <termo> {‘+’<termo> |  ‘–’<termo> }

Fatorando à direita
<expressão> ::= <termo> {‘+’<termo> |  ‘–’<termo> }
<expressão> ::= <termo> { (‘+’ |‘-’)<termo> }

####################################################
<termo> ::= <termo>’*’<fator>| <termo> ‘/’<fator> | <fator>

Eliminando a recursividade à esquerda e fatorando
<termo> ::=  <fator> {(’*’|’/’) <fator>}

####################################################

<expressão> ::= <termo> { (‘+’ |‘-’)<termo> }
<termo> ::=  <fator> {(’*’|’/’) <fator>}
<fator> ::= ‘a’| ‘b’| ‘c’|...| ‘1’| ‘2’| ‘3’|...| ‘(‘<expressão> ‘)’

para compilar use:
gcc ASDR5.c -Wall -Og -g -o ASDR5

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> // isalpha, isdigit

// variavel global
char *buffer ="(a+b)*(c-d+)(e)"; // posfixa abc1-*+
char lookahead;

// funcoes do analisador sintatico
void consome(char atomo);

// <expressão> ::= <termo> { (‘+’ |‘-’)<termo> }
// <termo> ::=  <fator> {(’*’|’/’) <fator>}
// <fator> ::= ‘a’| ‘b’| ‘c’|...| ‘1’| ‘2’| ‘3’|...| ‘(‘<expressão> ‘)’
// prototipacao de funcao
void expressao();
void termo();
void fator();



int main(){
    printf("\nAnalisando: %s",buffer);
    printf("\nposfixa   : ");
    // lookahead == obter_atomo()
    lookahead = *buffer++; // recebe o simbolo inicial e atualiza o buffer

    expressao(); // chama o simbolo inicial da gramatica
    consome('\0');

    printf("\n\nAnalise sintatica finalizada com sucesso.\n");

    return 0;
}
// <expressão> ::= <termo> { (‘+’ |‘-’)<termo> }
void expressao(){
   termo(); // esquerda
   while(lookahead == '+'|| lookahead == '-'){
       char operador = lookahead;
       consome(lookahead);
       termo(); // direita
       printf("%c",operador);
   }
   
}

// <termo> ::=  <fator> {(’*’|’/’) <fator>}
void termo(){
   fator(); // esquerda
   while(lookahead == '*'|| lookahead == '/'){
       char operador = lookahead;
       consome(lookahead);
       fator(); // direita
       printf("%c",operador);
   }
   
}
// <fator> ::= ‘a’| ‘b’| ‘c’|...| ‘1’| ‘2’| ‘3’|...| ‘(‘<expressão> ‘)’
void fator(){
    if(isalpha(lookahead)){
        printf("%c",lookahead);
        consome(lookahead);
    }
    else if (isdigit(lookahead)){
        printf("%c",lookahead);
        consome(lookahead);
    }
    else{
        consome('(');
        expressao();
        consome(')');
    }
}
// recebe um atomo que deveria estar no inicio do buffer, ou seja, lookhead
void consome( char atomo ){
    if( lookahead == atomo )
        lookahead = *buffer++; // obter_atomo();
    else{
        printf("\nErro sintatico: esperado [%c] encontrado [%c]\n",atomo,lookahead);
        exit(1);
    }
}
