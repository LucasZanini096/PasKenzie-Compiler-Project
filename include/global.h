#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define BUFFER_SIZE 1024 //Tamanho do buffer de entrada

typedef enum{
    ERRO,
    // Palavras reservadas
    PROGRAM, VAR, BEGIN, END, IF, THEN, ELSE, WHILE, DO,
    READ, WRITE, CHAR, INTEGER, BOOLEAN, DIV, OR, AND, NOT,
    TRUE, FALSE,
    // Identificadores e constantes
    IDENTIFICADOR, NUMERO, CARACTER,
    // Operadores relacionais
    IGUAL, DIFERENTE, MENOR, MENOR_IGUAL, MAIOR, MAIOR_IGUAL,
    // Operadores aritméticos
    SOMA, SUBTRACAO, MULTIPLICACAO,
    // Símbolos
    PONTO_VIRGULA, VIRGULA, DOIS_PONTOS, ATRIBUICAO,
    ABRE_PAR, FECHA_PAR, PONTO,
    // Comentário e fim de arquivo
    COMENTARIO, EOS
}TAtom; //Definição dos tipos de átomos presentes na linguagem

typedef struct{ 
  TAtom atom;  // Identificação do tipo do átomo
  int line;  // Linha que reconheceu o átomo
  union{ 
    int number;   // atributo do átomo constint (constante inteira)  
    char id[16];  // atributo identifier 
    char ch;      // atributo do átomo constchar (constante carecter) 
  }attribute; 
}TInfoAtom;

typedef struct {
  char *word;
  TAtom atom;
} TReservedWord;

TReservedWord reserved_words_paskenzie[] = {
    {"program", PROGRAM}, {"var", VAR}, {"begin", BEGIN},
    {"end", END}, {"if", IF}, {"then", THEN}, {"else", ELSE},
    {"while", WHILE}, {"do", DO}, {"read", READ}, {"write", WRITE},
    {"char", CHAR}, {"integer", INTEGER}, {"boolean", BOOLEAN},
    {"div", DIV}, {"or", OR}, {"and", AND}, {"not", NOT},
    {"true", TRUE}, {"false", FALSE}, {NULL, ERRO}
};

char *buffer;
char lexeme[20]; // Composição de fragmentos que compõem o átomo - Lexema
int nLine;
TInfoAtom info_atom;
TAtom lookahead;
FILE *file;

#endif