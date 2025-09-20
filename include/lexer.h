#ifndef LEXER_H
#define LEXER_H
#include "global.h"

TInfoAtomo obterAtomo();
void recognize_number(TInfoAtomo *infoatomo); // Reconhecer números
void recognize_id_reserved_word(TInfoAtomo *infoAtomo); // Reconhecer identifcadores e palavras reservadas
void recognize_score(TInfoAtomo *infoAtomo); //Reconhecer operadores
void recognize_char(TInfoAtomo *infoAtomo); //Reconhece char
void acknowledge_comment(TInfoAtomo *infoAtomo); //Reconhece comentários
int  transform_exponential_number(char *str);

#endif 