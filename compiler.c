/* Projeto PasKenzie - 6N 
   
   Lucas Zanini da Silva - 10417361
   Gabriel Alves - 10418133

*/

#include "include/global.h"
#include "src/parser.c"

int main(void){

    file = fopen("program.txt", "r"); //Abrindo o arquivo txt que possui o programa
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

    info_atom = getAtom();  //Atribuindo o info_atom
    lookahead = info_atom.atom; //Atribuindo o lookahead
    syntactic_analysis(); //Função de anaálise sintática

    return 0;
}

