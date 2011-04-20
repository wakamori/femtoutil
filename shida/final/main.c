#include <stdio.h>
#include"main.h"
#include"parser.h"
#include"eval.h"
void print_ans( void );
int malloc_size = MALLOC_SIZE;
value_t* sp_value = &stack_value[0];
char str[200];
int main( int argc, char* argv[] )
{
    
    pc = NULL;
    malloc_size = 0;

    while(1){
        sp_value = &stack_value[0];
        printf(">>>");

        fgets( str, sizeof(str), stdin );
        parse( str, 0 );
        eval();
        print_ans();

    }

}
void print_ans( void ){
    value_t* ret = pop();
    switch( ret->type ){
        case NUM:
            printf( "%d\n", ret->u.i );
            break;

        case T:
            printf( "T\n%d\n", ret->u.i );
            break;

        case nil:
            printf( "nil\n%d\n", ret->u.i );
            break;

    }
    pc++;
}
