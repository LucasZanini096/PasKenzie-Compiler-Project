#include "../include/lexer.h"

/* ANALISADOR LÉXICO */

TInfoAtom getAtom(){
  TInfoAtom infoAtom;

  infoAtom.atom = ERRO;

  while ( *buffer == '\n' || *buffer == ' ' || *buffer == '\t' || *buffer == '\r' ) {
    if ( *buffer == '\n' ) {
      nLine++; //Incrementa a linha caso haja quebra de linha
    }
      
    buffer++; //Aumenta a posiçã do buffer caso seja alguns dos elementos acima
  }
    
  infoAtom.line = nLine;

  //Verifica se é um comentário
  if((*buffer) == '(' && (*(buffer+1)) == '*'){
    acknowledge_comment(&infoAtom);
  }
  
  //Verifica se é um número inteiro 
  else if (isdigit(*buffer)){  //Reconhecendo números inteiros
    recognize_number(&infoAtom);
  }

  //Verifica ser é um identificador ou palavra reservada da linguagem
  else if (isalpha(*buffer) || (*buffer)=='_') {  
    recognize_id_reserved_word(&infoAtom);
  }

  //Verficar se é um char
  else if ((*buffer) == '\'') {  
    recognize_char(&infoAtom);
  }

  //Reconhece operadores e pontução
  else if (ispunct(*buffer)){  //Reconhecendo operadores ( + ,  - , /, * )
    recognize_score(&infoAtom);
  }

  //Fim do Buffer
  else if ( *buffer == '\0' ){
    infoAtom.atom = EOS;  
  }

  return infoAtom; //Retorna o infoAtomo
}

// Autômato reconhecedor de números inteiros
void recognize_number(TInfoAtom *infoAtom){
    char *ini_lexeme = buffer;
    
q0:

    if(isdigit(*buffer) ){ 
        buffer++; //Consome um dígito
        goto q0;
    }

    else if ((*buffer) == 'd' || (*buffer) == 'D'){
      buffer++; //Consome d ou D
      goto q1;
    }
    
    goto q3;

q1:

    if((*buffer) == '+'){
      buffer++; //Consome +
    }

    if(isdigit(*buffer)){
      buffer++; //Consome um dígito
      goto q2;
    }

    return;

q2:

    if(isdigit(*buffer)){
      buffer++; //Consome um dígito
      goto q2;
    }

    goto q3;

q3:

    strncpy(lexeme,ini_lexeme,buffer-ini_lexeme);
    lexeme[buffer-ini_lexeme] = '\0'; // aqui temos uma string

    infoAtom->attribute.number = transform_exponential_number(lexeme);
    infoAtom->atom = NUMERO;

    return;

}

// Conversor de números exponenciais
int transform_exponential_number(char *str){
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

void recognize_id_reserved_word(TInfoAtom *infoAtom){
  // Verificar se o infoAtom é um identificador ou uma palavra reservada
    char *ini_lexeme = buffer;
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

    //Extração do lexeme para realizar a verificação entre palavra reservada e identificador
    strncpy(lexeme, ini_lexeme, buffer-ini_lexeme);
    lexeme[buffer-ini_lexeme] = '\0';


    //Verificar se a palavra identificada é uma palavra reservada da linguagem
    for( i=0; reserved_words_paskenzie[i].word != NULL; i++ )
    {
      if(strcmp(lexeme, reserved_words_paskenzie[i].word) == 0)
      {
        infoAtom->atom = reserved_words_paskenzie[i].atom;
        return;
      }
    }


    //Caso contrário será um identificador
    strncpy(infoAtom->attribute.id,ini_lexeme,buffer-ini_lexeme);
    infoAtom->attribute.id[buffer-ini_lexeme] = '\0'; 
    infoAtom->atom = IDENTIFICADOR;

    return;
}

//Reconhecedor de char
void recognize_char(TInfoAtom *infoAtom){

    buffer++; //Consumindo '

    if((*buffer) == '\'' || (*buffer) == '\0' || (*buffer) == '\n') 
    {
      return;
    }

    infoAtom->attribute.ch = *buffer;
    buffer++;

    if((*buffer) != '\'')
    {
     return; 
    } 

    buffer++; //Consome '
    infoAtom->atom = CARACTER; //Atribui o infoAtom como CHAR
    return;

}

// Reconhecedor de comentários
void acknowledge_comment(TInfoAtom *infoAtom){
    buffer += 2; // Consome (*

q1:
   if((*buffer) == '\0'){
    return; //Retorna com erro, pois não foi fechado o comentário
   }

   if((*buffer) == '*' && (*(buffer+1)) == ')'){
    buffer += 2;
    infoAtom->atom = COMENTARIO;
    return;
   }

   if(*buffer == '\n'){ //Caso haja quebra de linha no comentário incrementa o nLine
    nLine++;
   }

   buffer++;
   goto q1;

};

// Reconhecedor de pontuação e operadores
void recognize_score(TInfoAtom *infoAtom){

  switch (*buffer)
  {
  case '+': //Identificação de sinal de adição
    buffer++;
    infoAtom->atom = SOMA;
    return;

  case '-': //Identificação de sinal de subtração
    buffer++;
    infoAtom->atom = SUBTRACAO;
    return;
  
  case '*': //Identificação de sinal de multiplicação
    buffer++;
    infoAtom->atom = MULTIPLICACAO;
    return;

  case ';': //Identificação de sinal de ponto e vígula
    buffer++;
    infoAtom->atom = PONTO_VIRGULA;
    return;
  
  case ':': 
    
   buffer++;

    if((*buffer) == '='){
      buffer++;
      infoAtom->atom = ATRIBUICAO; //Identificação de sinal de atribuição
      return;
    } else {
    infoAtom->atom = DOIS_PONTOS; //Identificação de sinal de dois pontos
    } 

    return;
  
  case '(': //Identificação de abre parênteses
    buffer++;
    infoAtom->atom = ABRE_PAR;
    return;
  
  case ')': //Identifcação de fecha parênteses
    buffer++;
    infoAtom->atom = FECHA_PAR;
    return;
  
  case '.': //Identificação de ponto final
    buffer++;
    infoAtom->atom = PONTO;
    return;

  case ',': //Identificação de vírgula
    buffer++;
    infoAtom->atom = VIRGULA;
    return;

  case '=': //Identificação do sinal de igual
    buffer++;
    infoAtom->atom = IGUAL;
    return;

  case '>': 
    buffer++;

    if((*buffer) == '='){
      buffer++;
      infoAtom->atom = MAIOR_IGUAL; //Identificação do sinal de maior igual
    }
    else {
      infoAtom->atom = MAIOR; //Identificação do sinal de maior
    }

    return;
  
  case '<':

    buffer++;

    if((*buffer) == '>'){
      buffer++;
      infoAtom->atom = DIFERENTE; //Identificação do sinal de diferente
    }
    else if ((*buffer) == '=')
    {
      buffer++;
      infoAtom->atom = MENOR_IGUAL; //Identificação do sinal de menor igual
    }
    else {
      infoAtom->atom = MENOR; //Identificação do sinal de menor
    }

    return;
  
  default:
    return; //Retorna um erro
  }
}