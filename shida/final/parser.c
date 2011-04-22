#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include"parser.h"

cons_t* cons;

int parse( char* str, int id, struct function_Data_t* p )
{
    int index = id;
    int count_temp;
    enum eINSTRUCTION operation_type;
    function_Data_t* p_temp;
    int operation_count = 0;
    int if_count = 0;
    char token_temp[40];
    int num_temp;
    cons_t* if_ptr;
    void **table=eval( 1 );

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
                printf("PUSH_PC\n");
                cons->instruction_ptr = table[ cons->instruction ];
                cons->op[0].i = num_temp;
                if( operation_count > 0 ){
                    operation_count++;
                }
                if( operation_count > 2 ){
                    cons = (cons_t*)myalloc( sizeof( cons_t ) );
                    printf("operation\n");
                    cons->instruction = operation_type;
                    cons->instruction_ptr = table[ cons->instruction ];
                }
                if( if_count == 2){
                    if_count = 3;
                    cons = (cons_t*)myalloc( sizeof( cons_t ) );
                    printf("return or end\n");
                    cons->instruction = (p != NULL) ? RETURN :END;
                    cons->instruction_ptr = table[ cons->instruction ];
                    if_ptr->op[1].adr = cons+1;
                }
                break;

            case '-':
                if ( str[ index+1 ] <= 57 && str[ index+1 ] >= 48 ){
                    num_temp = atoi( &str[index] );
                    index += (int)log10( (double)-num_temp ) + 1;
                    cons = (cons_t*)myalloc( sizeof( cons_t ) );
                    printf("num\n");
                    cons->op[0].i = num_temp;
                    cons->instruction = PUSH_PC;
                    cons->instruction_ptr = table[ cons->instruction ];

                    if(operation_count > 0 ){
                        operation_count++;
                    }
                    if( operation_count > 2 ){
                        cons = (cons_t*)myalloc( sizeof( cons_t ) );
                        printf("operation\n");
                        cons->instruction = operation_type;
                        cons->instruction_ptr = table[ cons->instruction ];
                    }
                    if( if_count == 2 ){
                        if_count = 3;
                        cons = myalloc( sizeof( cons_t ) );
                        printf("return or end\n");
                        cons->instruction = (p != NULL) ? RETURN : END;
                        cons->instruction_ptr = table[ cons->instruction ];
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
                index = parse( str, index+1, p );


                if( operation_count > 0 ){
                    operation_count++;
                }
                if( operation_count > 2){
                    cons = (cons_t*)myalloc( sizeof( cons_t ) );
                    printf("operation\n");
                    cons->instruction = operation_type;
                    cons->instruction_ptr = table[cons->instruction];
                }
                if( if_count == 2 ){
                    if_count = 3;
                    if_ptr->op[1].adr = cons+1;
                }
                if( if_count == 1 ){
                    cons = (cons_t*)myalloc( sizeof( cons_t ) );
                    printf("jump\n");
                    cons->instruction = JMP;
                    cons->instruction_ptr = table[ cons->instruction ];
                    if_ptr = cons;
                    if_ptr->op[0].adr = cons + 1;
                    if_count = 2;
                }
                break;

            case ' ':
                break;

            case '\n':
                break;

            case ')':
                return index++;
                break;

            case 'i':
                if( str[ index+1 ] == 'f' ){
                    if_count=1;
                    index++;
                }
                break;

            case 'd':
                if( str[ index+1 ] == 'e' && str[ index+2 ] == 'f' && str[ index+3 ] == 'u' && str[ index+4 ] == 'n' ){
                    index+= 5;
                    index = setf( str, index );
                    cons = (cons_t*)myalloc( sizeof( cons_t) );
                    cons->instruction = RETURN;
                    cons->instruction_ptr = table[ cons->instruction ];
                    printf("return\n");
                    return -10;
                }
                break;

            default:
                count_temp=0;
                while( 1 ){
                    if( str[ index ] == ' ' || str[ index ] == '(' || str[index] == ')' || str[index] == '\0' ){
                        break;
                    } else {
                        token_temp[ count_temp ] = str[index];
                        count_temp++;
                        index++;
                    }
                }
                token_temp[ count_temp ] = '\0';
                if ( searchf( token_temp ) != NULL ){
                    p_temp = searchf( token_temp );
                    index = parse( str, index + 1, p_temp );
                    cons = (cons_t*)myalloc( sizeof( cons_t) );
                    printf("goto\n");
                    cons->instruction = GOTO;
                    cons->op[0].adr = p_temp->adr;
                    cons->instruction_ptr = table[ cons->instruction ];
                    if( operation_count > 0 ){
                        operation_count++;
                    }
                    if( operation_count > 2){
                        cons = (cons_t*)myalloc( sizeof( cons_t ) );
                        printf("operation\n");
                        cons->instruction = operation_type;
                        cons->instruction_ptr = table[cons->instruction];
                    }
                    if( if_count == 2 ){
                        if_count = 3;
                        if_ptr->op[1].adr = cons+1;
                    }
                    if( if_count == 1 ){
                        cons = (cons_t*)myalloc( sizeof( cons_t ) );
                        printf("jump\n");
                        cons->instruction = JMP;
                        cons->instruction_ptr = table[ cons->instruction ];
                        if_ptr = cons;
                        if_ptr->op[0].adr = cons + 1;
                        if_count = 2;
                    }
                }


                if( p != NULL && strcmp( p->arg, token_temp ) == 0 ){
                    cons = (cons_t*)myalloc(sizeof( cons_t ) );
                    printf("arg\n");
                    cons->instruction = ARG;
                    cons->instruction_ptr = table[ cons->instruction ];
                    if( operation_count > 0 ){
                        operation_count++;
                    }
                    if( operation_count > 2 ){
                        cons = (cons_t*)myalloc( sizeof( cons_t ) );
                        printf("operation\n");
                        cons->instruction = operation_type;
                        cons->instruction_ptr = table[ cons->instruction ];
                    }
                    if( if_count == 2){
                        if_count = 3;
                        cons = (cons_t*)myalloc( sizeof( cons_t ) );
                        printf("return or end\n");
                        cons->instruction = (p != NULL) ? RETURN :END;
                        cons->instruction_ptr = table[ cons->instruction ];
                        if_ptr->op[1].adr = cons+1;
                    }
                }
                break;

        }
        if (str[index] != '\0' ){
            index++;
        }

    }
    if (p == NULL ){
        cons = (cons_t*)myalloc( sizeof( cons_t ) );
        printf("return or end\n");
        cons->instruction = (p != NULL) ? RETURN : END;
        cons->instruction_ptr = table[ cons->instruction ];
    }

    return index;


}
int setf( char* str, int i ){
    function_Data_t* p;
    function_Data_t* p_next;
    char name_temp[40];
    char arg_temp[40];
    int index = i;
    int flag_temp = 0;
    int char_count = 0;

    while( 1 ){
        if( str[ index ] != ' ' && str[ index ] != '(' ){
            flag_temp = 1;
            name_temp[ char_count ] = str[ index ];
            char_count++;
        } else {
            if( flag_temp == 1)
                break;
        }
        index++;
    }
    name_temp[ char_count ] = '\0';
    char_count = 0;
    flag_temp = 0;
    while( 1 ){
        if( /*str[ index ] != ' ' &&*/ str[ index ] != '(' && str[ index ] != ')' ){
            arg_temp[ char_count ] = str[ index ];
            char_count++;
        } else if( str[ index ] == '(' ){
            flag_temp = 1;
        } else if( flag_temp == 1 ){
            break;
        }
        index++;
    }
    index++;
    arg_temp[char_count ] = '\0';
    p = &function_Data[ ( name_temp[0] * name_temp[1] ) % ( sizeof(function_Data) / sizeof(function_Data[0]) ) ];
    while( 1 ){
        if( p->name[0] == '\0' ){
            strcpy( p->name, name_temp );
            strcpy(p->arg, arg_temp );
            printf("name = %s\narg = %s\n", name_temp, arg_temp);
            cons = (cons_t*)myalloc( sizeof(cons_t) );
            p->adr = cons + 1 ;
            index = parse( str, index, p );
            break;
        } else if( strcmp(p->name, name_temp ) == 0 ){
            break;
        } else if( p->next == NULL ){
            p->next = (function_Data_t*)malloc( sizeof( function_Data_t ) );
            p->next->name[0] = '\0';
            p = p->next;
        } else {
            p = p->next;
        }
    }
    return index;
}

struct function_Data_t* searchf( char* str )
{
    struct function_Data_t* p = &function_Data[ (str[0] * str[1] ) % ( sizeof( function_Data ) / sizeof( function_Data[0] ) ) ];
    while(1){
        if( strcmp( p->name, str ) == 0 ){
            return p;
        } else if( p->next != NULL ){
            p = p->next;
        } else {
            return NULL;
        }
    }
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
    pc_next = malloc_ptr;
    return (malloc_ptr-size);
}
