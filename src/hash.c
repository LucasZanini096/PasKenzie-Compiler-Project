#include "../include/hash.h"

// Definição das variáveis globais (aqui sim)
TTabelaSimbolos tabelaSimbolos;
int nextAddress = 0;

// Função para a obtenção de um hash
int hashMack( char * s )
{
    char *p;
    unsigned int h = 0, g;
    for ( p = s; *p != EOS; p = p + 1 ){
        h = ( h << 4 ) + (*p);
        g = h&0xf0000000U;
        if ( g ){
            h = h ^ ( g >> 24 );
            h = h ^ g;
        }
    }
    return h % PRIME_NUMBER;
}

// Inicializa a tabela de símbolos
void initializeSymbolTable(TTabelaSimbolos *table) {
    for (int i = 0; i < PRIME_NUMBER; i++) {
        table->entradas[i] = NULL;
    }
}

// Insere um identificador na tabela
int insertSymbol(TTabelaSimbolos *table, char *id, int endereco) {
    int hash = hashMack(id);
    TNo *novo = (TNo*)malloc(sizeof(TNo));
    
    if(novo == NULL) return 0;
    
    strcpy(novo->ID, id);
    novo->endereco = endereco;
    novo->prox = table->entradas[hash];
    table->entradas[hash] = novo;
    
    return 1;
}

// Função que retona um nó ( indentificador ) da tabela de símbolos
TNo* searchSymbol(TTabelaSimbolos *table, char *id) {
    int hash = hashMack(id);
    TNo *atual = table->entradas[hash];
    
    while(atual != NULL) {
        if(strcmp(atual->ID, id) == 0) {
            return atual;
        }
        atual = atual->prox;
    }
    
    return NULL;
}
