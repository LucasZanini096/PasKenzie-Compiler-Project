/* Projeto PasKenzie - 6N 
   
   Lucas Zanini da Silva - 10417361
   Gabriel Alves - 10418133

*/


#include "include/global.h"
#include "src/parser.c"

int main(int argc, char *argv[]){

    file = fopen(argv[1], "r");
    if (file == NULL){
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    buffer = (char*) malloc(BUFFER_SIZE * sizeof(char));

    buffer[0] = '\0';

    char linha[256];
    while(fgets(linha, sizeof(linha), file) != NULL){
        strcat(buffer, linha);
    }

    fclose(file);
    nLinha = 1;

    info_atomo = obterAtomo();
    lookahead = info_atomo.atomo;
    analyse();

    return 0;
}

