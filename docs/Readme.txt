==========================================================
== Gabriel Alves de Freitas Spinola Sucupira - 10418133 ==
== Lucas Zanini da Silva - 10417361                     ==
==========================================================

=================
== 1. Objetivo ==
=================

Este projeto implementa um compilador em estágio inicial, com as fases de análise léxica e análise sintática para a linguagem PasKenzie. A linguagem aceita variáveis dos tipos integer, boolean e char, comandos condicionais if/then/else e de repetição while/do, além das funções de entrada read e saída write.

======================
== 2. Implementação ==
======================
A implementação foi estruturada em módulos para maior clareza, tendo lexer.c e lexer.h para a análise léxica, parser.c e parser.h para a análise sintática, e global.h para definições globais. Foi adotado um token global lookahead e uma função consome() para avançar, permitindo um parser LL(1) simples. Os comentários são reconhecidos pelo lexer e descartados pelo parser, a contagem de linhas é mantida inclusive dentro dos comentários.

=========================================
== 3. Bugs ou limitações identificadas ==
=========================================
Não foram identificados bugs relevantes durante a execução dos testes com arquivos fonte que seguem a gramática especificada. Porém, aspas tipográficas (‘ ’) não são reconhecidas pelo analisador léxico (erro encontrado ao copiar e colar os testes do pdf de documento), aspas comuns ASCII ('') são reconhececidas normalmente.

==============================
== 4. Compilação e execução ==
==============================
gcc -Wall -Wno-unused-result -g -Og compiler.c -o compilador

./compilador {NOME_DO_ARQUIVO}.txt

