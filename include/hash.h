#ifndef HASH_H
#define HASH_H

#include "global.h"

#define PRIME_NUMBER 211
#define EOS '\0'

// Primeiro defina as estruturas
typedef struct _TNo{ 
   char ID[16]; 
   int endereco; 
   struct _TNo *prox; 
} TNo; 
 
typedef struct { 
    TNo *entradas[PRIME_NUMBER]; 
} TTabelaSimbolos;

// DEPOIS declare as variáveis globais
extern TTabelaSimbolos tabelaSimbolos;
extern int nextAddress;

// Protótipos das funções
int hashMack(char *s);
void initializeSymbolTable(TTabelaSimbolos *table);
int insertSymbol(TTabelaSimbolos *table, char *id, int endereco);
TNo* searchSymbol(TTabelaSimbolos *table, char *id);

#endif