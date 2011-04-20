#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include"parser.h"
#include"main.h"

int parse( char* str, int id )
{
    int index = id;
    enum eINSTRUCTION operation_type;
    int operation_count = 0;
    int num_temp;
    cons_t* cons;

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
                cons = (cons_t*)malloc_original( sizeof( cons_t ) );
                cons->instruction = PUSH_PC;
                cons->op1.i = num_temp;
                if( operation_count > 0 ){
                    operation_count++;
                }
                if( operation_count > 2 ){
                    cons = (cons_t*)malloc_original( sizeof( cons_t ) );
                    cons->instruction = operation_type;
                }
                break;

            case '-':
                if ( str[ index+1 ] <= 57 && str[ index+1 ] >= 48 ){
                    num_temp = atoi( &str[index] );
                    index += (int)log10( (double)-num_temp ) + 1;
                    cons = (cons_t*)malloc_original( sizeof( cons_t ) );
                    cons->op1.i = num_temp;
                    cons->instruction = PUSH_PC;
                    if(operation_count > 0 ){
                        operation_count++;
                    }
                    if( operation_count > 2 ){
                        cons = (cons_t*)malloc_original( sizeof( cons_t ) );
                        cons->instruction = operation_type;
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
                if( operation_count>0 ){
                    operation_count++;
                }
                if( operation_count > 2){
                    cons = (cons_t*)malloc_original( sizeof( cons_t ) );
                    cons->instruction = operation_type;
                }
                break;

            case ')':
                return index++;
                break;

        }
        index++;

    }
    cons = (cons_t*)malloc_original( sizeof( cons_t ) );
    cons->instruction = END;
    return index;

}
void* malloc_original( int size )
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
