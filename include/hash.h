#ifndef HASH_H
#define HASH_H

#include "global.h"

#define PRIME_NUMBER 211
#define EOS '\0'

// Definição de estruturas para a tabela de símbolos
// Nó da tabela de símbolos
typedef struct _TNo{ 
   char ID[16]; 
   int endereco; 
   struct _TNo *prox; 
} TNo; 
 
// Tabela de símbolos
typedef struct { 
    TNo *entradas[PRIME_NUMBER]; 
} TTabelaSimbolos;

// Declaração das variáveis globais
extern TTabelaSimbolos tabelaSimbolos;
extern int nextAddress;

// Protótipos das funções
int hashMack(char *s);
void initializeSymbolTable(TTabelaSimbolos *table);
int insertSymbol(TTabelaSimbolos *table, char *id, int endereco);
TNo* searchSymbol(TTabelaSimbolos *table, char *id);
int countSymbols(TTabelaSimbolos *table);
void printSymbolTable(TTabelaSimbolos *table);

#endif