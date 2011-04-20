#include <stdio.h>
#include"main.h"
#include"parser.h"
#include"eval.h"

int malloc_size = MALLOC_SIZE;
value_t* sp_value = &stack_value[0];
char str[200];
int main( int argc, char* argv[] )
{
    FILE* file;
    
    pc = NULL;
    malloc_size = 0;
    if( argc == 2 ){
        file = fopen( argv[1] , "r" );
    }

    while(1){
        sp_value = &stack_value[0];
        printf(">>>");

        fgets( str, sizeof(str), stdin );
        parse( str, 0 );
        eval();


    }

}
