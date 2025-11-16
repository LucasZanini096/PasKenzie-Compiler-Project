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

// Função para obter o número de entradas na tabela de símbolos
int countSymbols(TTabelaSimbolos *table) {
    int count = 0;
    for (int i = 0; i < PRIME_NUMBER; i++) {
        TNo *atual = table->entradas[i];
        while(atual != NULL) {  
            count++;
            atual = atual->prox;
        }
    } 
    return count;
}

// Função para printar a tabela de símbolos (para depuração)
void printSymbolTable(TTabelaSimbolos *table) {
    printf("\nTabela de Simbolos:\n");
    for (int i = 0; i < PRIME_NUMBER; i++) {
        TNo *atual = table->entradas[i];
        if(atual != NULL) {
            printf("Entrada Tabela de Símbolos: [%d] => ", i);
            while(atual != NULL) {  
                printf("%s | Endereco: %d\n", atual->ID, atual->endereco);
                atual = atual->prox;
            }
        }
    } 
}


/*

TABELA DE SIMBOLOS 
Entrada Tabela Simbolos: [81] => cont | Endereco: 2 
Entrada Tabela Simbolos: [139] => fat | Endereco: 0  
Entrada Tabela Simbolos: [179] => num | Endereco: 1 


*/