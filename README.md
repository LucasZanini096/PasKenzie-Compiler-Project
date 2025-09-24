# Projeto PasKenzie

## Compiladores

### Prof. Me. Fábio Aparecido Gamarra Lubacheski

### Integrantes

- Gabriel Alves de Freitas Spinola Sucupira - 10418133

- Lucas Zanini da Silva - 10417361

---

## Objetivo

Este projeto implementa um compilador em estágio inicial, com as fases de **análise léxica** e **análise sintática** para a linguagem **PasKenzie**.  
A linguagem aceita variáveis dos tipos `integer`, `boolean` e `char`, comandos condicionais `if/then/else` e de repetição `while/do`, além das funções de entrada `read` e saída `write`.

---

## Implementação e Decisões de Design

A implementação foi estruturada em módulos para maior clareza:

- **lexer.c / lexer.h** – responsável pela análise léxica
- **parser.c / parser.h** – responsável pela análise sintática
- **global.h** – definições globais

Foi adotado um **token global lookahead** e uma função `consome()` para avançar, permitindo um parser $LL(1)$ simples.

Os comentários são reconhecidos pelo lexer e descartados pelo parser, e a contagem de linhas é mantida inclusive dentro dos comentários.

O compilador imprime os tokens reconhecidos com suas linhas e, ao término da análise sintática, informa se o programa está sintaticamente correto.

---

## Bugs ou Limitações Identificadas

Não foram identificados bugs relevantes durante a execução dos testes com arquivos fonte que seguem a gramática especificada.

Porém, **aspas tipográficas** (`‘ ’`) não são reconhecidas pelo analisador léxico (erro encontrado ao copiar e colar os testes do PDF do enunciado). **Aspas comuns $ASCII$** (`''`) são reconhecidas normalmente.

---

## Requisitos

Para compilar e rodar o projeto corretamente é necessário:

- Um compilador C compatível com o padrão **C99 ou superior** (ex.: GCC/MinGW no Windows ou GCC no Linux);
- Arquivos-fonte `.txt` da linguagem PasKenzie salvos em **ASCII com apóstrofos simples** para constantes caractere;

Ambiente testado com as seguintes flags:

- `Wall` $\to$ ativa avisos úteis.
- `Wno-unused-result` $\to$ evita avisos de resultados não usados.
- `g` $\to$ permite depurar com gdb mostrando código-fonte e variáveis.
- `Og` $\to$ otimização leve, mantendo a depuração funcional.

---

## Compilação e Execução

Compile com:

```bash
gcc -Wall -Wno-unused-result -g -Og compiler.c -o compilador

```

E execute usando:

```
./compilador {NOME_DO_ARQUIVO}.txt
```
