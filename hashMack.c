/*
Linux
gcc -g -Og -c hashMack.c -Wall -Wno-unused-result -o hashMack.o

Windows
gcc -g -Og -c hashMack.c -Wall -Wno-unused-result -o hashMack.obj
*/
#define PRIME_NUMBER 211
#define EOS '\0'

/*
função hashMack: implementa a dispersão padrão para um hash, 
usa um numero primo como tamanho do vetor para minimizar colisões
*/
int hashMack( char * s )
{
    char *p;
    unsigned int h = 0, g;
    for ( p = s; *p != EOS; p = p + 1 ){
        h = ( h << 4 ) + (*p);
        g = h&0xf0000000U;
        if ( g ){
            h = h ^ ( g >> 24 );
            h = h ^ g;
        }
    }
    return h % PRIME_NUMBER;
}