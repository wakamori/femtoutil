#include <stdio.h>
#include"main.h"
#include"eval.h"

/*pc_0 = {};
void ** __thcode__;
void eval(void *pc)
{
    void *table[];
    goto *(table[pc->opcode]);

    L_thcode:
        __thcode = table;
        return;
}*/


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
        &&jmp,
        &&go_to,
        &&re_turn,
        &&arg
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
    //ret_ptr = pop();
    //printf( "if %d\n",ret_ptr->type);
    cons_ptr = pc->op[ pop()->type ].adr;
    pc = cons_ptr;
    goto *((pc)->instruction_ptr);

go_to:
    *(sp_arg++) = pop()->u.i;
    *(sp_adr++) = pc + 1;
    pc = pc->op[0].adr;
    goto *((pc)->instruction_ptr);


re_turn:
    //printf("return\n");
    --sp_arg;
    pc = *(--sp_adr);
    goto *((pc)->instruction_ptr);

arg:
    sp_value->type = NUM;
    push_i( *(sp_arg-1) );
    //printf("arg%d\n",*(sp_arg-1));
    goto *((++pc)->instruction_ptr);
}



static inline void push_value_t( value_t* t )
{
    *(sp_value++) = *t;
}

static inline void push_i( int i)
{
    (sp_value++)->u.i = i;
}

static inline void push_bool( enum eTYPE t )
{
    (sp_value++)->type = t;
}
static inline value_t* pop( void )
{
    return (--sp_value);
}

value_t* pop_ans( void )
{
    return (--sp_value);
}
