/* Projeto PasKenzie - 6N */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "compiler.h"


TPalavraReservada palavras_reservadas_paskenzie[] = {
    {"program", PROGRAM}, {"var", VAR}, {"begin", BEGIN},
    {"end", END}, {"if", IF}, {"then", THEN}, {"else", ELSE},
    {"while", WHILE}, {"do", DO}, {"read", READ}, {"write", WRITE},
    {"char", CHAR}, {"integer", INTEGER}, {"boolean", BOOLEAN},
    {"div", DIV}, {"or", OR}, {"and", AND}, {"not", NOT},
    {"true", TRUE}, {"false", FALSE}, {NULL, ERRO}
};

/* VARIÁVEIS GLOBAIS */

//char lexema[20]; // Composição de fragmentos que compõem o átomo

/* FUNÇÕES DO ANALISADOR LÉXICO */

// TInfoAtomo obterAtomo();
// void reconhece_numero(TInfoAtomo *infoatomo); // Reconhecer números
// void reconhece_id_palavra_reservada(TInfoAtomo *infoAtomo); // Reconhecer identifcadores e palavras reservadas
// void reconhece_pontuacao(TInfoAtomo *infoAtomo); //Reconhecer operadores
// void reconhece_char(TInfoAtomo *infoAtomo); //Reconhece char
// void reconhece_comentario(TInfoAtomo *infoAtomo); //Reconhece comentários
// int  transforma_numero_exponencial(char *str);

// int main(void){
//     TInfoAtomo info_atomo;
//     buffer = "(* Este é um comentário *) program";
//     printf("Analisando %s => \n", buffer);
//     nLinha = 1;
//     do{
      
//       info_atomo = obterAtomo();

//       if( info_atomo.atomo == NUMERO )
//         printf("Atomo NUMERO reconhecido [%d] na linha %d.\n",info_atomo.atributo.numero, info_atomo.linha);
//       else if( info_atomo.atomo == IDENTIFICADOR )
//         printf("Atomo ID reconhecido [%s] na linha %d.\n",info_atomo.atributo.id, info_atomo.linha);
//         else if( info_atomo.atomo == CARACTER )
//         printf("Atomo CARACTER reconhecido [%c] na linha %d.\n",info_atomo.atributo.ch, info_atomo.linha);
//       else if ( info_atomo.atomo == ERRO )
//         printf("Atomo ID NAO reconhecido. Erro na linha %d\n", info_atomo.linha);
//       else if( info_atomo.atomo == EOS)
//         printf("Fim de buffer.\n");

//     }while( info_atomo.atomo != ERRO && info_atomo.atomo != EOS );

//     printf("fim de programa. %d linhas analisadas\n", info_atomo.linha);

//     return 0;
// }

/* ANALISADOR LÉXICO */

TInfoAtomo obterAtomo(){
  TInfoAtomo infoAtomo;

  infoAtomo.atomo = ERRO;

  while ( *buffer == '\n' || *buffer == ' ' || *buffer == '\t' ){
    if ( *buffer == '\n' ) {
      nLinha++;
    }
      
    buffer++;
  }
    
  infoAtomo.linha = nLinha;

  //Verifica se é um comentário
  if((*buffer) == '(' && (*(buffer+1)) == '*'){
    reconhece_comentario(&infoAtomo);
  }
  
  //Verifica se é um número inteiro 
  else if (isdigit(*buffer)){  //Reconhecendo números inteiros
    reconhece_numero(&infoAtomo);
  }

  //Verifica ser é um identificador ou palavra reservada da linguagem
  else if (isalpha(*buffer) || (*buffer)=='_') {  
    reconhece_id_palavra_reservada(&infoAtomo);
  }

  //Verficar se é um char
  else if ((*buffer) == '\'') {  
    reconhece_char(&infoAtomo);
  }

  //Reconhece operadores e pontução
  else if (ispunct(*buffer)){  //Reconhecendo operadores ( + ,  - , /, * )
    reconhece_pontuacao(&infoAtomo);
  }

  //Fim do Buffer
  else if ( *buffer == '\0' ){
    infoAtomo.atomo = EOS;  
  }

  return infoAtomo;
}


void reconhece_numero(TInfoAtomo *infoAtomo){
    char *ini_lexema = buffer;
    
q0:

    if(isdigit(*buffer) ){
        buffer++;
        goto q0;
    }

    else if ((*buffer) == 'd' || (*buffer) == 'D'){
      buffer++;
      goto q1;
    }
    
    goto q3;

q1:

    if((*buffer) == '+'){
      buffer++;
    }

    if(isdigit(*buffer)){
      buffer++;
      goto q2;
    }

    return;

q2:

    if(isdigit(*buffer)){
      buffer++;
      goto q2;
    }

    goto q3;

q3:
    // recorta lexema 
    strncpy(lexema,ini_lexema,buffer-ini_lexema);
    lexema[buffer-ini_lexema] = '\0'; // aqui temos uma string

    infoAtomo->atributo.numero = transforma_numero_exponencial(lexema);
    infoAtomo->atomo = NUMERO;

    return;

}

int transforma_numero_exponencial(char *str){
  char *d_pos = strchr(str, 'd'); //Procura a primeira ocorrência do caracter d numa string

  if(d_pos == NULL){
     return atoi(str); //Retorna número simples
  }
  
  *d_pos = '\0'; //Separando a base do expoente
  int resultado = atoi(str); //Armazenando o valor base

  char *exp_str = d_pos++; //Salvando a referência do expoente
  if(*exp_str == '+'){ //Verifica se há um sinal de +
    exp_str++; //Pula para a próxima posição
  }

  int expoente = atoi(exp_str); //Converte para inteiro

  for (int i = 0; i < expoente; i++){
    resultado *= 10; //Calcula o valor final do resultado =
  }

  *d_pos = 'd'; //Retorna com o caracter d na posição de origem
  return resultado; //Retorna com o número inteiro

}

void reconhece_id_palavra_reservada(TInfoAtomo *infoAtomo){
  // Verificar se o infoAtomo é um identificador ou uma palavra reservada
    char *ini_lexema = buffer;
    int tamanho_str = 0;
    int i;

q1: 
    if(isalpha(*buffer) || (*buffer)=='_' || isdigit(*buffer)){
      if(tamanho_str > 15){
        return; //Não pode variáveis com mais de 15 caracteres
      }

      buffer++;
      tamanho_str++;
      goto q1;

    }

    //Extração do Lexema para realizar a verificação entre palavra reservada e identificador
    strncpy(lexema, ini_lexema, buffer-ini_lexema);
    lexema[buffer-ini_lexema] = '\0';


    //Verificar se a palavra identificada é uma palavra reservada da linguagem
    for( i=0; palavras_reservadas_paskenzie[i].palavra != NULL; i++ )
    {
      if(strcmp(lexema, palavras_reservadas_paskenzie[i].palavra) == 0)
      {
        infoAtomo->atomo = palavras_reservadas_paskenzie[i].atomo;
        return;
      }
    }


    //Caso contrário será um identificador
    strncpy(infoAtomo->atributo.id,ini_lexema,buffer-ini_lexema);
    infoAtomo->atributo.id[buffer-ini_lexema] = '\0'; 
    infoAtomo->atomo = IDENTIFICADOR;

    return;
}

void reconhece_char(TInfoAtomo *infoAtomo){

q0:
    if((*buffer) != '\'')
    {
      return;
    }

    buffer++;
    goto q1;

q1:
    if((*buffer) == '\'' || (*buffer) == '\0' || (*buffer) == '\n') 
    {
      return;
    }

    infoAtomo->atributo.ch = *buffer;
    buffer++;
    goto q2;

q2:

    if((*buffer) != '\'')
    {
     return; 
    } 

    buffer++; //Consome a segunda aspa
    infoAtomo->atomo = CARACTER; //Atribui o infoAtomo como CHAR
    return;
}


void reconhece_comentario(TInfoAtomo *infoAtomo){

q0:
  
    if((*buffer) == '(' && (*(buffer+1) == '*')){
      buffer += 2;
      goto q1;
    }
    
    return;

q1:

   if((*buffer) == '\0'){
    return; //Retorna com erro, pois não foi fechado o comentário
   }

   if((*buffer) == '*' && (*(buffer+1)) == ')'){
    buffer += 2;
    infoAtomo->atomo = COMENTARIO;
    return;
   }

   buffer++;
   goto q1;

};

void reconhece_pontuacao(TInfoAtomo *infoAtomo){

  switch (*buffer)
  {
  case '+': //Identificação de sinal de adição
    buffer++;
    infoAtomo->atomo = SOMA;
    return;

  case '-': //Identificação de sinal de subtração
    buffer++;
    infoAtomo->atomo = SUBTRACAO;
    return;
  
  case '*': //Identificação de sinal de multiplicação
    buffer++;
    infoAtomo->atomo = MULTIPLICACAO;
    return;

  case ';': //Identificação de sinal de ponto e vígula
    buffer++;
    infoAtomo->atomo = PONTO_VIRGULA;
    return;
  
  case ':': //Identificação de sinal de dois pontos ou atribuição
    
   buffer++;

    if((*buffer) == '='){
      buffer++;
      infoAtomo->atomo = ATRIBUICAO;
      return;
    } else {
    infoAtomo->atomo = DOIS_PONTOS;
    }

    return;
  
  case '(': //Identificação de abre parênteses
    buffer++;
    infoAtomo->atomo = ABRE_PAR;
    return;
  
  case ')': //Identifcação de fecha parênteses
    buffer++;
    infoAtomo->atomo = FECHA_PAR;
    return;
  
  case '.': //Identificação de ponto final
    buffer++;
    infoAtomo->atomo = PONTO;
    return;

  case ',': //Identificação de vírgula
    buffer++;
    infoAtomo->atomo = VIRGULA;
    return;

  case '=':
    buffer++;
    infoAtomo->atomo = IGUAL;
    return;

  case '>':
    buffer++;

    if((*buffer) == '='){
      buffer++;
      infoAtomo->atomo = MAIOR_IGUAL;
    }
    else {
      infoAtomo->atomo = MAIOR;
    }

    return;
  
  case '<':

    buffer++;

    if((*buffer) == '>'){
      buffer++;
      infoAtomo->atomo = DIFERENTE;
    }
    else if ((*buffer) == '=')
    {
      buffer++;
      infoAtomo->atomo = MENOR_IGUAL;
    }
    else {
      infoAtomo->atomo = MENOR;
    }

    return;
    

  
  default:
    return;
  }
}

/* ANALISADOR SINTÁTICO */
TInfoAtomo tokenAtual;

void proximo_token(){ 
  tokenAtual = obterAtomo(); 
}

void erro_sintatico(const char *msg){
  fprintf(stderr,"Erro sintático na linha %d: %s (token %d)\n", tokenAtual.linha,msg,tokenAtual.atomo);
  exit(EXIT_FAILURE);
}

/* Protótipos */
void programa(); 
void bloco(); 
void declaracoes();
void lista_comandos(); 
void comando();
void expressao(); 
void expressao_simples();
void termo(); 
void fator();
void operador_relacional(); 
void operador_adicao(); 
void operador_multiplicacao();

/* ------------------------
 * FUNÇÕES DO PARSER
 * ------------------------ */
void programa(){
    
  if(tokenAtual.atomo!=PROGRAM) {
    erro_sintatico("Esperado 'program'");
  }
    
    proximo_token();
    
    if(tokenAtual.atomo!=IDENTIFICADOR) {
      erro_sintatico("Esperado identificador");
    }
    
    proximo_token();
    
    if(tokenAtual.atomo!=PONTO_VIRGULA) {
      erro_sintatico("Esperado ';'");
    }
    
    proximo_token();
    
    bloco();
    
    if(tokenAtual.atomo!=PONTO) {
      erro_sintatico("Esperado '.'");
    }
    
    proximo_token();
}


void bloco(){
    declaracoes();

    if(tokenAtual.atomo!=BEGIN) {
      erro_sintatico("Esperado 'begin'");
    }

    proximo_token();

    lista_comandos();

    if(tokenAtual.atomo!=END) {
      erro_sintatico("Esperado 'end'");
    }

    proximo_token();
}


void declaracoes(){

  if(tokenAtual.atomo==VAR){

      proximo_token();

        while(tokenAtual.atomo==IDENTIFICADOR){

          proximo_token();

            while(tokenAtual.atomo==VIRGULA) proximo_token(), proximo_token(); // lista de ids
            
            if(tokenAtual.atomo!=DOIS_PONTOS) {
              erro_sintatico("Esperado ':'");
            }

            proximo_token();

            if(!(tokenAtual.atomo==INTEGER || tokenAtual.atomo==BOOLEAN || tokenAtual.atomo==CHAR)){
                erro_sintatico("Tipo inválido");
              }

                proximo_token();

            if(tokenAtual.atomo!=PONTO_VIRGULA) erro_sintatico("Esperado ';'");
            proximo_token();
        }
    }
}


void lista_comandos(){
    comando();
    while(tokenAtual.atomo==PONTO_VIRGULA){
        proximo_token();
        comando();
    }
}


void comando(){
    if(tokenAtual.atomo==IDENTIFICADOR){
        proximo_token();
        
        if(tokenAtual.atomo==ATRIBUICAO){
            proximo_token();
            expressao();
        }

    }
    
    else if(tokenAtual.atomo==READ){
        proximo_token();
        
        if(tokenAtual.atomo!=ABRE_PAR) {
          erro_sintatico("Esperado '('");
        }
        
        proximo_token();
        
        if(tokenAtual.atomo!=IDENTIFICADOR) {
          erro_sintatico("Esperado identificador");
        }
        
        proximo_token();

        while(tokenAtual.atomo==VIRGULA){
            proximo_token();
            
            if(tokenAtual.atomo!=IDENTIFICADOR) {
              erro_sintatico("Esperado identificador");
            }
            
            proximo_token();
        }
        if(tokenAtual.atomo!=FECHA_PAR) {
          erro_sintatico("Esperado ')'");
        }
        proximo_token();
    } 
    else if(tokenAtual.atomo==WRITE){
        
      proximo_token();
        
        if(tokenAtual.atomo!=ABRE_PAR) {erro_sintatico("Esperado '('");}
        
        proximo_token();
        
        if(tokenAtual.atomo!=IDENTIFICADOR) {
          erro_sintatico("Esperado identificador");
        }

        proximo_token();
        
        while(tokenAtual.atomo==VIRGULA){
            proximo_token();
            if(tokenAtual.atomo!=IDENTIFICADOR) erro_sintatico("Esperado identificador");
            proximo_token();
        }

        if(tokenAtual.atomo!=FECHA_PAR) {
          erro_sintatico("Esperado ')'");
        }

        proximo_token();
    } 
    else if(tokenAtual.atomo==IF){
        proximo_token();
        expressao();
        if(tokenAtual.atomo!=THEN) {
          erro_sintatico("Esperado 'then'");
        }
        proximo_token();
        comando();
        if(tokenAtual.atomo==ELSE){
            proximo_token();
            comando();
        }
    } 
    else if(tokenAtual.atomo==WHILE){
        proximo_token();
        expressao();
        if(tokenAtual.atomo!=DO) {
          erro_sintatico("Esperado 'do'");
        }
        proximo_token();
        comando();
    } 
    else if(tokenAtual.atomo==BEGIN){
        proximo_token();
        lista_comandos();
        
        if(tokenAtual.atomo!=END) {
          erro_sintatico("Esperado 'end'");
        }
        
        proximo_token();
    }
}


/* ------------------------
 * EXPRESSÕES
 * ------------------------ */
void expressao(){
    expressao_simples();

    if(tokenAtual.atomo==IGUAL || tokenAtual.atomo==DIFERENTE || tokenAtual.atomo==MENOR ||
        tokenAtual.atomo==MENOR_IGUAL || tokenAtual.atomo==MAIOR || tokenAtual.atomo==MAIOR_IGUAL ||
        tokenAtual.atomo==AND || tokenAtual.atomo==OR){
    
          operador_relacional();
        expressao_simples();
    
      }
}

void expressao_simples(){
    
  if(tokenAtual.atomo==SOMA || tokenAtual.atomo==SUBTRACAO) {
    proximo_token();
  } // sinal opcional
  
  termo();
  
  while(tokenAtual.atomo==SOMA || tokenAtual.atomo==SUBTRACAO){
      operador_adicao();
      termo();
  }
}

void termo(){
  fator();

  while(tokenAtual.atomo==MULTIPLICACAO || tokenAtual.atomo==DIV){
    operador_multiplicacao();
    fator();
  }
}

void fator(){
    
  if(tokenAtual.atomo==IDENTIFICADOR || tokenAtual.atomo==NUMERO || tokenAtual.atomo==CARACTER
        || tokenAtual.atomo==TRUE || tokenAtual.atomo==FALSE){
        proximo_token();
    } 
    
  else if(tokenAtual.atomo==ABRE_PAR){
      proximo_token();
      expressao();
      if(tokenAtual.atomo!=FECHA_PAR) erro_sintatico("Esperado ')'");
      proximo_token();
  } 
  
  else if(tokenAtual.atomo==NOT){
      proximo_token();
      fator();
  } 
  
  else {
      erro_sintatico("Fator inválido");
  }
}

void operador_relacional(){ 
  proximo_token(); 
}

void operador_adicao(){ 
  proximo_token(); 
}

void operador_multiplicacao(){ 
  proximo_token(); 
}

/* ------------------------
 * FUNÇÃO DE ENTRADA DO PARSER
 * ------------------------ */
void analisar(){
    proximo_token();
    programa();
    if(tokenAtual.atomo!=EOS)
        erro_sintatico("Tokens extras após fim do programa");
    printf("%d linhas analisadas, programa sintaticamente correto\n", nLinha);
}
