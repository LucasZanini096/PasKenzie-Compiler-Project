#ifndef LEXER_H
#define LEXER_H
#include "global.h"

/* FUNÇÕES DO ANALISADOR LÉXICO */

TInfoAtom getAtom();
void recognize_number(TInfoAtom *infoAtom); // Reconhecer números
void recognize_id_reserved_word(TInfoAtom *infoAtom); // Reconhecer identifcadores e palavras reservadas
void recognize_score(TInfoAtom *infoAtom); //Reconhecer operadores
void recognize_char(TInfoAtom *infoAtom); //Reconhece char
void acknowledge_comment(TInfoAtom *infoAtom); //Reconhece comentários
int  transform_exponential_number(char *str); //Reconhecedor de números exponenciais 

#endif 