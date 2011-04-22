#include <stdio.h>
#include <stdlib.h>
#include"main.h"
#include"parser.h"
#include"eval.h"
void print_ans( void );
int malloc_size = MALLOC_SIZE;
int i_temp;
char str[200];
int main( int argc, char* argv[] )
{
    void** table = eval( 1 );
    cons_t* cons;
    int count = 0;
    int flag = 0;
    FILE *file;
    for( i_temp = 0; i_temp < sizeof(function_Data) / sizeof(function_Data[0]); i_temp++){
        function_Data->name[0] = '\0';
    }
    pc = NULL;
    malloc_size = 0;

    if( (file = fopen(argv[1],"r")) == NULL) {
        printf("error\n");
    } else {
        flag = 1;
    }
    sp_value = &stack_value[0];
    sp_adr = &stack_adr[0];
    sp_arg = &stack_arg[0];
    pc = pc_next;

    strcpy( str,"defun func(n) if(< n 3) 1 (+ func(- n 1) func(- n 2) )\n");
    parse( str, 0, NULL );

    sp_value = &stack_value[0];
    sp_adr = &stack_adr[0];
    sp_arg = &stack_arg[0];
    pc = pc_next;


    strcpy( str,"func 36\n");
    parse( str, 0, NULL );
    eval(0);
    print_ans();
    exit(0);
    while(1){
        sp_value = &stack_value[0];
        sp_adr = &stack_adr[0];
        sp_arg = &stack_arg[0];
        pc = pc_next;
        printf(">>>");
        if( flag == 1)
            fgets( str, sizeof(str), file );
        else
            fgets( str, sizeof(str), stdin );
        if (parse( str, 0, NULL ) != -10 ){
            printf("\n\n");
            eval( 0 );
            print_ans();
        }

        count++;
    }

}
void print_ans( void ){
    value_t* ret = pop_ans();
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

        default:
            printf("default%d\n",ret->u.i);
    }
    pc++;
}
