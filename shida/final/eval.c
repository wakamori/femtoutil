#include <stdio.h>
#include"main.h"

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
    cons_t* stack_adr[STACKSIZE];
    int stack_arg[STACKSIZE];
    value_t stack_value[STACKSIZE];

    register value_t* sp_value = stack_value;
    register int* sp_arg = stack_arg;
    register cons_t** sp_adr = stack_adr;
    register cons_t* pc = pc_gloval;
    register int a,b,ret;
    register struct value_t *a_ptr,*ret_ptr;
    register cons_t* cons_ptr;

    if( i == 1 ){
        return table;
    }

    goto *(pc->instruction_ptr);

end:
    printf("answer = %d\n",stack_value[0].i);
    return;

push_pc:
    (sp_value)->type = NUM;
    (sp_value++)->i = pc->op[0].i;
    goto *((++pc)->instruction_ptr);

plus:
    (sp_value-2)->i += (--sp_value)->i;
    goto *((++pc)->instruction_ptr);

minus:
    (sp_value-2)->i -= (--sp_value)->i;
    goto *((++pc)->instruction_ptr);

mul:
    (sp_value-2)->i *= (--sp_value)->i;
    goto *((++pc)->instruction_ptr);

div:
    (sp_value-2)->i /= (--sp_value)->i;
    goto *((++pc)->instruction_ptr);

gt:
    ret_ptr = (--sp_value);
    a_ptr = (--sp_value);
    ret_ptr->type = ( ret_ptr->i > a_ptr->i && a_ptr->type != nil) ? T : nil;
    *(sp_value++) = *ret_ptr;
    goto *((++pc)->instruction_ptr);

gte:
    ret_ptr = (--sp_value);
    a_ptr = (--sp_value);
    ret_ptr->type = ( ret_ptr->i >= a_ptr->i && a_ptr->type != nil) ? T : nil; 
    *(sp_value++) = *ret_ptr;
    goto *((++pc)->instruction_ptr);

lt:
    ret_ptr = (--sp_value);
    a_ptr = (--sp_value);
    ret_ptr->type = ( ret_ptr->i < a_ptr->i && a_ptr->type != nil) ? T : nil;
    *(sp_value++) = *ret_ptr;
    goto *((++pc)->instruction_ptr);

lte:
    ret_ptr = (--sp_value);
    a_ptr = (--sp_value);
    ret_ptr->type = ( ret_ptr->i <= a_ptr->i && a_ptr->type != nil) ? T : nil;
    *(sp_value++) = *ret_ptr;
    goto *((++pc)->instruction_ptr);

eq:
    ret_ptr = (--sp_value);
    ret_ptr->type = ( ret_ptr->i == (--sp_value)->i ) ? T : nil;
    *(sp_value++) = *ret_ptr;
    goto *((++pc)->instruction_ptr);

jmp:
    cons_ptr = pc->op[ (--sp_value)->type ].adr;
    pc = cons_ptr;
    goto *((pc)->instruction_ptr);

go_to:
    *(sp_arg++) = (--sp_value)->i;
    *(sp_adr++) = pc + 1;
    pc = pc->op[0].adr;
    goto *((pc)->instruction_ptr);


re_turn:
    --sp_arg;
    pc = *(--sp_adr);
    goto *((pc)->instruction_ptr);

arg:
    sp_value->type = NUM;
    (sp_value++)->i = *(sp_arg-1);
    goto *((++pc)->instruction_ptr);
}
