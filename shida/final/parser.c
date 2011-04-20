#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include"parser.h"
#include"main.h"

cons_t* cons;

int parse( char* str, int id )
{
    int index = id;
    enum eINSTRUCTION operation_type;
    int operation_count = 0;
    int if_count = 0;
    int num_temp;
    cons_t* if_ptr;

    while( str[ index ] != '\0' && str[ index ] != '\n'){
        switch( str[ index ] ){
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                num_temp = atoi( &str[ index ] );
                if( num_temp != 0 )
                    index += (int)log10( (double)num_temp );
                cons = (cons_t*)myalloc( sizeof( cons_t ) );
                cons->instruction = PUSH_PC;
                cons->op[0].i = num_temp;
                if( operation_count > 0 ){
                    operation_count++;
                }
                if( operation_count > 2 ){
                    cons = (cons_t*)myalloc( sizeof( cons_t ) );
                    cons->instruction = operation_type;
                }
                if( if_count == 2){
                    if_count = 3;
                    cons = (cons_t*)myalloc( sizeof( cons_t ) );
                    cons->instruction = END;
                    if_ptr->op[1].adr = cons+1;
                }
                break;

            case '-':
                if ( str[ index+1 ] <= 57 && str[ index+1 ] >= 48 ){
                    num_temp = atoi( &str[index] );
                    index += (int)log10( (double)-num_temp ) + 1;
                    cons = (cons_t*)myalloc( sizeof( cons_t ) );
                    cons->op[0].i = num_temp;
                    cons->instruction = PUSH_PC;

                    if(operation_count > 0 ){
                        operation_count++;
                    }
                    if( operation_count > 2 ){
                        cons = (cons_t*)myalloc( sizeof( cons_t ) );
                        cons->instruction = operation_type;
                    }
                    if( if_count == 2 ){
                        if_count = 3;
                        cons = myalloc( sizeof( cons_t ) );
                        cons->instruction = END;
                        if_ptr->op[1].adr = cons + 1;
                    }
                } else {
                    operation_count++;
                    operation_type = MINUS;
                }
                break;

            case '*':
                operation_count++;
                operation_type = MUL;
                break;

            case '+':
                operation_count++;
                operation_type = PLUS;
                break;

            case '/':
                operation_count++;
                operation_type = DIV;
                break;

            case '>':
                operation_count++;
                if( str[ index+1 ] == '=' ){
                    operation_type = LTE;
                    index++;
                } else {
                    operation_type = LT;
                }
                break;

            case '<':
                operation_count++;
                if( str[ index+1 ] == '=' ){
                    operation_type = GTE;
                    index++;
                } else {
                    operation_type = GT;
                }
                break;

            case '=':
                if( str[ index+1 ] == '=' ){
                    operation_count++;
                    operation_type = EQ;
                    index++;
                }
                break;

            case '(':
                index = parse( str, index+1 );


                if( operation_count > 0 ){
                    operation_count++;
                }
                if( operation_count > 2){
                    cons = (cons_t*)myalloc( sizeof( cons_t ) );
                    cons->instruction = operation_type;
                }
                if( if_count == 2 ){
                    if_count = 3;
                    if_ptr->op[1].adr = cons+1;
                }
                if( if_count == 1 ){
                    cons = (cons_t*)myalloc( sizeof( cons_t ) );
                    cons->instruction = JMP;
                    if_ptr = cons;
                    if_ptr->op[0].adr = cons + 1;
                    if_count = 2;
                }
                break;

            case ')':
                return index++;
                break;

            case 'i':
                if( str[ index+1 ] == 'f' ){
                    if_count=1;
                }
                break;

        }
        index++;

    }
    cons = (cons_t*)myalloc( sizeof( cons_t ) );
    cons->instruction = END;
    return index;

}
void* myalloc( int size )
{
    if ( size > malloc_size ) {
        malloc_ptr = malloc( sizeof( char ) * MALLOC_SIZE );
        if( pc == NULL ){
            pc = malloc_ptr;
        }
        malloc_size = MALLOC_SIZE;
    }
    malloc_size -= size;
    malloc_ptr += size;
    return (malloc_ptr-size);
}
