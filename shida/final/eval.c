#include <stdio.h>
#include"main.h"
#include"eval.h"


void** eval( int i )
{

    static const void *table [] = {
        &&push_pc,
        &&plus,
        &&minus,
        &&mul,
        &&div,
        &&end,
        &&gt,
        &&gte,
        &&lt,
        &&lte,
        &&eq,
        &&jmp
    };
    int a,ret;
    struct value_t *a_ptr,*ret_ptr;
    cons_t* cons_ptr;

    if( i == 1 ){
        return table;
    }

    goto *(pc->instruction_ptr);

end:
    return;

push_pc:
    (sp_value)->type = NUM;
    (sp_value++)->u.i = pc->op[0].i;
    goto *((++pc)->instruction_ptr);

plus:
    ret = pop()->u.i + pop()->u.i;
    push_i( ret );
    goto *((++pc)->instruction_ptr);

minus:
    a = pop()->u.i;
    ret = pop()->u.i - a;
    push_i( ret );
    goto *((++pc)->instruction_ptr);

mul:
    ret = pop()->u.i * pop()->u.i;
    push_i( ret );
    goto *((++pc)->instruction_ptr);

div:
    a = pop()->u.i;
    ret = pop()->u.i / a;
    push_i( ret );
    goto *((++pc)->instruction_ptr);

gt:
    ret_ptr = pop();
    a_ptr = pop();
    ret_ptr->type = ( ret_ptr->u.i > a_ptr->u.i && a_ptr->type != nil) ? T : nil;
    push_value_t( ret_ptr );
    goto *((++pc)->instruction_ptr);

gte:
    ret_ptr = pop();
    a_ptr = pop();
    ret_ptr->type = ( ret_ptr->u.i >= a_ptr->u.i && a_ptr->type != nil) ? T : nil; 
    push_value_t( ret_ptr );
    goto *((++pc)->instruction_ptr);

lt:
    ret_ptr = pop();
    a_ptr = pop();
    ret_ptr->type = ( ret_ptr->u.i < a_ptr->u.i && a_ptr->type != nil) ? T : nil;
    push_value_t( ret_ptr );
    goto *((++pc)->instruction_ptr);

lte:
    ret_ptr = pop();
    a_ptr = pop();
    ret_ptr->type = ( ret_ptr->u.i <= a_ptr->u.i && a_ptr->type != nil) ? T : nil;
    push_value_t( ret_ptr );
    goto *((++pc)->instruction_ptr);

eq:
    ret_ptr = pop();
    ret_ptr->type = ( ret_ptr->u.i == pop()->u.i ) ? T : nil;
    push_value_t( ret_ptr );
    goto *((++pc)->instruction_ptr);

jmp:
    ret_ptr = pop();
    printf( "if %d\n",ret_ptr->type);
    cons_ptr = pc->op[ ret_ptr->type ].adr;
    pc = cons_ptr;
    goto *((pc)->instruction_ptr);
}



void push_value_t( value_t* t )
{
    *(sp_value++) = *t;
}

void push_i( int i)
{
    (sp_value++)->u.i = i;
}

void push_bool( enum eTYPE t )
{
    (sp_value++)->type = t;
}
value_t* pop( void )
{
    return (--sp_value);
}


