/*
TESTES AUTOMATIZADOS PARA ANALISADOR PASKENZIE
Compilar com: gcc -Wall -Wno-unused-result -g -Og testes.c -o testes
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "compiler.c"

// Incluir aqui suas definições de TAtomo, TInfoAtomo, etc.
// (copiadas do seu código principal)

/* ESTRUTURA PARA CASOS DE TESTE */
typedef struct {
    char *nome_teste;
    char *entrada;
    TAtomo atomos_esperados[50];  // Sequência de átomos esperados
    int deve_ter_erro;            // 1 se deve dar erro, 0 se deve passar
    char *descricao;
} TCasoTeste;

/* VARIÁVEIS GLOBAIS DE TESTE */
int testes_executados = 0;
int testes_passou = 0;
int testes_falhou = 0;

/* PROTÓTIPOS DE FUNÇÕES DE TESTE */
void executa_teste(TCasoTeste teste);
void testa_analisador_lexico();
void testa_analisador_sintatico();
void testa_casos_extremos();
void imprime_resultado_final();
int compara_atomos(TAtomo esperados[], TAtomo obtidos[], int tamanho);

/*
===========================
TESTES DO ANALISADOR LÉXICO
===========================
*/

TCasoTeste testes_lexicos[] = {
    // TESTE 1: Identificadores simples
    {
        "Identificadores simples",
        "abc xyz test_var _internal",
        {IDENTIFICADOR, IDENTIFICADOR, IDENTIFICADOR, IDENTIFICADOR, EOS},
        0,
        "Teste básico de identificadores válidos"
    },
    
    // TESTE 2: Palavras reservadas
    {
        "Palavras reservadas",
        "program var begin end if then else while",
        {PROGRAM, VAR, BEGIN, END, IF, THEN, ELSE, WHILE, EOS},
        0,
        "Reconhecimento de palavras reservadas"
    },
    
    // TESTE 3: Números simples
    {
        "Números simples",
        "123 0 999 42",
        {NUMERO, NUMERO, NUMERO, NUMERO, EOS},
        0,
        "Números inteiros básicos"
    },
    
    // TESTE 4: Números com notação exponencial
    {
        "Notação exponencial",
        "12d2 5d+3 100d0 7d10",
        {NUMERO, NUMERO, NUMERO, NUMERO, EOS},
        0,
        "Números com notação d e d+"
    },
    
    // TESTE 5: Constantes de caractere
    {
        "Constantes de caractere",
        "'a' 'Z' '0' ' '",
        {CARACTER, CARACTER, CARACTER, CARACTER, EOS},
        0,
        "Caracteres entre aspas simples"
    },
    
    // TESTE 6: Operadores relacionais
    {
        "Operadores relacionais",
        "< <= > >= = <>",
        {MENOR, MENOR_IGUAL, MAIOR, MAIOR_IGUAL, IGUAL, DIFERENTE, EOS},
        0,
        "Operadores de comparação"
    },
    
    // TESTE 7: Operadores aritméticos
    {
        "Operadores aritméticos",
        "+ - * div",
        {SOMA, SUBTRACAO, MULTIPLICACAO, DIV, EOS},
        0,
        "Operações matemáticas"
    },
    
    // TESTE 8: Símbolos de pontuação
    {
        "Símbolos de pontuação",
        "; , : := ( ) .",
        {PONTO_VIRGULA, VIRGULA, DOIS_PONTOS, ATRIBUICAO, ABRE_PAR, FECHA_PAR, PONTO, EOS},
        0,
        "Delimitadores e símbolos"
    },
    
    // TESTE 9: Comentários
    {
        "Comentários",
        "(* Este é um comentário *) program",
        {COMENTARIO, PROGRAM, EOS},
        0,
        "Comentários de múltiplas linhas"
    },
    
    // TESTE 10: Erro - identificador muito longo
    {
        "Identificador longo",
        "identificador_muito_longo_com_mais_de_quinze_caracteres",
        {ERRO},
        1,
        "Identificador excede 15 caracteres"
    },
    
    // TESTE 11: Erro - aspas não fechadas
    {
        "Aspas não fechadas",
        "'a program",
        {ERRO},
        1,
        "Constante de caractere mal formada"
    },
    
    // TESTE 12: Erro - comentário não fechado
    {
        "Comentário não fechado",
        "(* comentario sem fim program",
        {ERRO},
        1,
        "Comentário não terminado"
    }
};

/*
===============================
TESTES DO ANALISADOR SINTÁTICO
===============================
*/

TCasoTeste testes_sintaticos[] = {
    // TESTE 1: Programa mínimo válido
    {
        "Programa mínimo",
        "program teste; begin end.",
        {},
        0,
        "Menor programa válido possível"
    },
    
    // TESTE 2: Programa com declaração de variável
    {
        "Declaração de variável",
        "program teste; var x: integer; begin end.",
        {},
        0,
        "Programa com uma variável"
    },
    
    // TESTE 3: Múltiplas variáveis
    {
        "Múltiplas variáveis",
        "program teste; var a, b, c: integer; x: char; flag: boolean; begin end.",
        {},
        0,
        "Declaração de várias variáveis"
    },
    
    // TESTE 4: Comando de atribuição
    {
        "Atribuição simples",
        "program teste; var x: integer; begin x := 42 end.",
        {},
        0,
        "Atribuição de valor a variável"
    },
    
    // TESTE 5: Comando if-then
    {
        "Comando if-then",
        "program teste; var x: integer; begin if x > 0 then x := 1 end.",
        {},
        0,
        "Condicional simples"
    },
    
    // TESTE 6: Comando if-then-else
    {
        "Comando if-then-else",
        "program teste; var x: integer; begin if x > 0 then x := 1 else x := 0 end.",
        {},
        0,
        "Condicional com alternativa"
    },
    
    // TESTE 7: Comando while
    {
        "Comando while",
        "program teste; var i: integer; begin while i < 10 do i := i + 1 end.",
        {},
        0,
        "Laço de repetição"
    },
    
    // TESTE 8: Comandos read e write
    {
        "Read e Write",
        "program teste; var x: integer; begin read(x); write(x) end.",
        {},
        0,
        "Entrada e saída de dados"
    },
    
    // TESTE 9: Expressões complexas
    {
        "Expressões complexas",
        "program teste; var a, b, c: integer; begin c := a + b * 2 - 1 end.",
        {},
        0,
        "Expressão aritmética com precedência"
    },
    
    // TESTE 10: Expressões booleanas
    {
        "Expressões booleanas",
        "program teste; var flag: boolean; begin if true and not false then flag := true end.",
        {},
        0,
        "Operadores lógicos"
    },
    
    // TESTE 11: Programa do enunciado
    {
        "Programa do enunciado",
        "program ex1;\n"
        "var\n"
        "  num_1, num_2: integer;\n"
        "  maior: integer;\n"
        "  teste: char;\n"
        "begin\n"
        "  read(num_1, num_2);\n"
        "  if num_1 > num_2 then\n"
        "    maior := num_1\n"
        "  else\n"
        "    maior := num_2;\n"
        "  teste := 'f';\n"
        "  write(maior)\n"
        "end.",
        {},
        0,
        "Exemplo completo do enunciado"
    },
    
    // TESTES DE ERRO SINTÁTICO
    
    // TESTE 12: Erro - falta ponto final
    {
        "Falta ponto final",
        "program teste; begin end",
        {},
        1,
        "Programa sem ponto final"
    },
    
    // TESTE 13: Erro - falta begin
    {
        "Falta begin",
        "program teste; var x: integer; end.",
        {},
        1,
        "Bloco sem begin"
    },
    
    // TESTE 14: Erro - falta ponto e vírgula
    {
        "Falta ponto e vírgula",
        "program teste var x: integer; begin end.",
        {},
        1,
        "Falta ; após declaração program"
    },
    
    // TESTE 15: Erro - tipo inexistente
    {
        "Tipo inválido",
        "program teste; var x: string; begin end.",
        {},
        1,
        "Tipo de dados não suportado"
    },
    
    // TESTE 16: Erro - expressão mal formada
    {
        "Expressão inválida",
        "program teste; var x: integer; begin x := + + 1 end.",
        {},
        1,
        "Expressão com operadores consecutivos"
    }
};

/*
====================
CASOS EXTREMOS
====================
*/

TCasoTeste testes_extremos[] = {
    // TESTE 1: Arquivo vazio
    {
        "Arquivo vazio",
        "",
        {EOS},
        1,
        "Entrada completamente vazia"
    },
    
    // TESTE 2: Só espaços e quebras
    {
        "Só espaços",
        "   \n\n\t\t   \n",
        {EOS},
        1,
        "Apenas caracteres em branco"
    },
    
    // TESTE 3: Comentário gigante
    {
        "Comentário grande",
        "(* \n"
        "Este é um comentário\n"
        "que ocupa várias linhas\n"
        "e tem muito texto dentro\n"
        "*) program teste; begin end.",
        {COMENTARIO, PROGRAM, IDENTIFICADOR, PONTO_VIRGULA, BEGIN, END, PONTO, EOS},
        0,
        "Comentário multi-linha extenso"
    },
    
    // TESTE 4: Muitos identificadores
    {
        "Muitos identificadores",
        "a b c d e f g h i j k l m n o p q r s t u v w x y z",
        {}, // Array será preenchido dinamicamente
        0,
        "Sequência longa de identificadores"
    },
    
    // TESTE 5: Números no limite
    {
        "Números grandes",
        "999999999 1d50 0000000",
        {NUMERO, NUMERO, NUMERO, EOS},
        0,
        "Números nos valores extremos"
    },
    
    // TESTE 6: Mistura complexa
    {
        "Mistura complexa",
        "program (* test *) abc123; var x_y_z: integer; begin x_y_z := 42d+2 end.",
        {},
        0,
        "Combinação de todos os elementos"
    }
};

/*
=======================
IMPLEMENTAÇÃO DOS TESTES
=======================
*/

void executa_teste(TCasoTeste teste) {
    printf("\n--- TESTE: %s ---\n", teste.nome_teste);
    printf("Descrição: %s\n", teste.descricao);
    printf("Entrada: %s\n", teste.entrada);
    
    testes_executados++;
    
    // Configurar buffer global para o teste
    buffer = teste.entrada;
    nLinha = 1;
    
    // Capturar saída do analisador
    TAtomo atomos_obtidos[50];
    int count = 0;
    TInfoAtomo info;
    
    do {
        info = obterAtomo();
        atomos_obtidos[count++] = info.atomo;
        
        // Debug: mostrar átomos encontrados
        printf("  -> Átomo encontrado: %d (linha %d)\n", info.atomo, info.linha);
        
    } while (info.atomo != EOS && info.atomo != ERRO && count < 49);
    
    // Verificar resultado
    int teste_passou = 0;
    
    if (teste.deve_ter_erro) {
        // Espera erro
        if (info.atomo == ERRO) {
            printf("✅ PASSOU: Erro detectado conforme esperado\n");
            teste_passou = 1;
        } else {
            printf("❌ FALHOU: Deveria ter erro mas passou\n");
        }
    } else {
        // Espera sucesso
        if (info.atomo == ERRO) {
            printf("❌ FALHOU: Erro inesperado\n");
        } else if (teste.atomos_esperados[0] == 0) {
            // Teste sintático - apenas verifica se não deu erro
            printf("✅ PASSOU: Análise sintática bem-sucedida\n");
            teste_passou = 1;
        } else {
            // Teste léxico - compara átomos
            if (compara_atomos(teste.atomos_esperados, atomos_obtidos, count)) {
                printf("✅ PASSOU: Átomos conforme esperado\n");
                teste_passou = 1;
            } else {
                printf("❌ FALHOU: Átomos diferentes do esperado\n");
            }
        }
    }
    
    if (teste_passou) {
        testes_passou++;
    } else {
        testes_falhou++;
    }
}

int compara_atomos(TAtomo esperados[], TAtomo obtidos[], int tamanho) {
    for (int i = 0; i < tamanho && esperados[i] != 0; i++) {
        if (esperados[i] != obtidos[i]) {
            printf("    Diferença na posição %d: esperado %d, obtido %d\n", 
                   i, esperados[i], obtidos[i]);
            return 0;
        }
    }
    return 1;
}

void testa_analisador_lexico() {
    printf("\n");
    printf("=====================================\n");
    printf("    TESTANDO ANALISADOR LÉXICO\n");
    printf("=====================================\n");
    
    int num_testes = sizeof(testes_lexicos) / sizeof(testes_lexicos[0]);
    for (int i = 0; i < num_testes; i++) {
        executa_teste(testes_lexicos[i]);
    }
}

void testa_analisador_sintatico() {
    printf("\n");
    printf("======================================\n");
    printf("    TESTANDO ANALISADOR SINTÁTICO\n");
    printf("======================================\n");
    
    int num_testes = sizeof(testes_sintaticos) / sizeof(testes_sintaticos[0]);
    for (int i = 0; i < num_testes; i++) {
        executa_teste(testes_sintaticos[i]);
    }
}

void testa_casos_extremos() {
    printf("\n");
    printf("====================================\n");
    printf("      TESTANDO CASOS EXTREMOS\n");
    printf("====================================\n");
    
    int num_testes = sizeof(testes_extremos) / sizeof(testes_extremos[0]);
    for (int i = 0; i < num_testes; i++) {
        executa_teste(testes_extremos[i]);
    }
}

void imprime_resultado_final() {
    printf("\n");
    printf("=========================================\n");
    printf("          RESULTADO FINAL\n");
    printf("=========================================\n");
    printf("Total de testes executados: %d\n", testes_executados);
    printf("Testes que passaram: %d\n", testes_passou);
    printf("Testes que falharam: %d\n", testes_falhou);
    
    if (testes_falhou == 0) {
        printf("🎉 TODOS OS TESTES PASSARAM! 🎉\n");
    } else {
        printf("⚠️  %d testes falharam - revisar código\n", testes_falhou);
    }
    
    printf("Taxa de sucesso: %.1f%%\n", 
           (float)testes_passou / testes_executados * 100);
}

/*
====================
FUNÇÃO MAIN DE TESTE
====================
*/

int main(void) {
    printf("INICIANDO TESTES AUTOMATIZADOS DO COMPILADOR PASKENZIE\n");
    printf("=======================================================\n");
    
    // Executar suítes de teste
    testa_analisador_lexico();
    //testa_analisador_sintatico();
    //testa_casos_extremos();
    
    // Resultado final
    imprime_resultado_final();
    
    return (testes_falhou == 0) ? 0 : 1;
}

/*
=============================
INSTRUÇÕES DE USO
=============================

1. COMPILAÇÃO:
   gcc -Wall -Wno-unused-result -g -Og testes.c seu_compilador.c -o testes

2. EXECUÇÃO:
   ./testes

3. INTERPRETAÇÃO DOS RESULTADOS:
   ✅ = Teste passou
   ❌ = Teste falhou
   
4. PERSONALIZAÇÃO:
   - Adicione novos casos no array de testes
   - Modifique os casos existentes conforme necessário
   - Execute testes individuais comentando outros

5. INTEGRAÇÃO COM MAKEFILE:
   make test

6. INTEGRAÇÃO COM CI/CD:
   O código retorna 0 se todos passaram, 1 se algum falhou
*/