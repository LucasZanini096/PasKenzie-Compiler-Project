#ifndef COMPILER_H
#define COMPILER_H

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
}TAtomo;

typedef struct{ 
  TAtomo atomo;  // Identificação do tipo do átomo
  int linha;  // Linha que reconheceu o átomo
  union{ 
    int numero;   // atributo do átomo constint (constante inteira)  
    char id[16];  // atributo identifier 
    char ch;      // atributo do átomo constchar (constante carecter) 
  }atributo; 
}TInfoAtomo;

typedef struct {
  char *palavra;
  TAtomo atomo;
} TPalavraReservada;

char *buffer;
char lexema[20]; // Composição de fragmentos que compõem o átomo
int nLinha;

TInfoAtomo obterAtomo();
void reconhece_numero(TInfoAtomo *infoatomo); // Reconhecer números
void reconhece_id_palavra_reservada(TInfoAtomo *infoAtomo); // Reconhecer identifcadores e palavras reservadas
void reconhece_pontuacao(TInfoAtomo *infoAtomo); //Reconhecer operadores
void reconhece_char(TInfoAtomo *infoAtomo); //Reconhece char
void reconhece_comentario(TInfoAtomo *infoAtomo); //Reconhece comentários
int  transforma_numero_exponencial(char *str);

#endif 