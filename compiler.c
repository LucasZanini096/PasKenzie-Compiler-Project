/* Projeto PasKenzie - 6N 
   
   Lucas Zanini da Silva - 10417361
   Gabriel Alves - 10418133

*/

#include "include/global.h"
#include "src/hash.c" 
#include "src/parser.c"

char *buffer;
char lexeme[20];
int nLine;
TInfoAtom info_atom;
TAtom lookahead;
FILE *file;
int labelCount;

int main(int argc, char *argv[]){

    if (argc < 2){
        printf("Erro: falta de argumentos.\nPara compilar use: %s <arquivo.txt>\n", argv[0]);
        return 1;
    }

    if (argc > 2){
        printf("Erro: muitos argumentos.\nPara compilar use: %s <arquivo.txt>\n", argv[0]);
        return 1;
    }
    

    file = fopen(argv[1], "r");
    if (file == NULL){
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    buffer = (char*) malloc(BUFFER_SIZE * sizeof(char)); //Alocando memória para o buffer de entrada

    buffer[0] = '\0';

    char line[256];
    while(fgets(line, sizeof(line), file) != NULL){
        strcat(buffer, line); //Copiando cada linha do arquivo para o buffer
    }

    fclose(file); //Fechando o arquivo 
    nLine = 1;
    labelCount = 0;

    initializeSymbolTable(&tabelaSimbolos);
    nextAddress = 0;

    info_atom = getAtom();  //Atribuindo o info_atom
    lookahead = info_atom.atom; //Atribuindo o lookahead
    syntactic_analysis(); //Função de anaálise sintática

    printSymbolTable(&tabelaSimbolos); // Printando a tabela de símbolos

    return 0;
}

