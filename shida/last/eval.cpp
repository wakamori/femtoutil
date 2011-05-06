#include <stdio.h>
#include"main.h"
void** eval (int i )
{
    static void *table [] = {
        &&push,
        &&plus,
        &&minus,
        &&mul,
        &&div,
        &&gt,
        &&gte,
        &&lt,
        &&lte,
        &&eq,
        &&plus2,
        &&minus2,
        &&mul2,
        &&div2,
        &&gt2,
        &&gte2,
        &&lt2,
        &&lte2,
        &&eq2,
        &&end,
        &&jmp,
        &&funccall,
        &&nfunccall,
        &&Return,
        &&nReturn,
        &&arg,
        &&narg,
        &&funcdef,
        &&setq,

    };

    if( i == 1 ){
        return table;
    }

    cons_t* stack_adr[STACKSIZE];
    int stack_arg[STACKSIZE];
    value_t stack_value[STACKSIZE];

    register value_t* sp_value = stack_value;
    register int* sp_arg = stack_arg;
    register cons_t** sp_adr = stack_adr;
    register cons_t* pc = memory + CurrentIndex;
    register int a = 0; 
    register struct value_t *a_ptr = NULL,*ret_ptr = NULL;


    goto *(pc->instruction_ptr);

plus:
    (sp_value-2)->i += (sp_value-1)->i;
    sp_value--;
    goto *((++pc)->instruction_ptr);

minus:
    (sp_value-2)->i -= (sp_value-1)->i;
    sp_value--;
    goto *((++pc)->instruction_ptr);

mul:
    (sp_value-2)->i *= (sp_value-1)->i;
    sp_value--;
    goto *((++pc)->instruction_ptr);

div:
    (sp_value-2)->i /= (sp_value-1)->i;
    sp_value--;
    goto *((++pc)->instruction_ptr);

funcdef:
    //if (i == 2)
        printf("%s\n",pc->op[1].c);
    return NULL;

end:
    //if (i == 2) 
        printf("%d\n",stack_value[0].i);
    return NULL;

push:
    sp_value->type = NUM;
    (sp_value++)->i = pc->op[0].i;
    goto *((++pc)->instruction_ptr);

plus2:
    (sp_value - 1)->i += pc->op[0].i;
    goto *((++pc)->instruction_ptr);

gt2:
    a_ptr = (sp_value - 1);
    a_ptr->type = ( pc->op[0].i > a_ptr->i && a_ptr->type != nil) ? T : nil; 
    goto *((++pc)->instruction_ptr);

gt:
    ret_ptr = (--sp_value);
    ret_ptr->type = ( ret_ptr->i > (--sp_value)->i && ret_ptr->type != nil) ? T : nil;
    *(sp_value++) = *ret_ptr;
    goto *((++pc)->instruction_ptr);

lte:
    ret_ptr = (--sp_value);
    ret_ptr->type = ( ret_ptr->i <= (--sp_value)->i && ret_ptr->type != nil) ? T : nil;
    *(sp_value++) = *ret_ptr;
    goto *((++pc)->instruction_ptr);

eq:
    ret_ptr = (--sp_value);
    ret_ptr->type = ( ret_ptr->i == (--sp_value)->i ) ? T : nil;
    *(sp_value++) = *ret_ptr;
    goto *((++pc)->instruction_ptr);

jmp:
    pc = pc->op[ (--sp_value)->type ].adr;
    goto *((pc)->instruction_ptr);

funccall:
    *(sp_arg++) = (--sp_value)->i;
    *(sp_adr++) = pc + 1;
    pc = pc->op[0].adr;
    goto *((pc)->instruction_ptr);

nfunccall:
    a = pc->op[1].i;
    while (a-- != 0){
        *(sp_arg++) = (--sp_value)->i;
    }
    *(sp_adr++) = pc + 1;
    pc = pc->op[0].adr;
    goto *((pc)->instruction_ptr);

Return:
    --sp_arg;
    pc = *(--sp_adr);
    goto *((pc)->instruction_ptr);

nReturn:
    sp_arg -= pc->op[0].i;
    pc = *(--sp_adr);
    goto *((pc)->instruction_ptr);

arg:
    sp_value->type = NUM;
    (sp_value++)->i = *(sp_arg-1);
    goto *((++pc)->instruction_ptr);

narg:
    sp_value->type = NUM;
    (sp_value++)->i = *(sp_arg - pc->op[0].i);
    goto *((++pc)->instruction_ptr);

minus2:
    (sp_value - 1)->i -= pc->op[0].i;
    goto *((++pc)->instruction_ptr);

mul2:
    (sp_value - 1)->i *= pc->op[0].i;
    goto *((++pc)->instruction_ptr);

div2:
    (sp_value - 1)->i /= pc->op[0].i;
    goto *((++pc)->instruction_ptr);

gte2:
    a_ptr = (sp_value - 1);
    a_ptr->type = ( pc->op[0].i >= a_ptr->i && a_ptr->type != nil) ? T : nil; 
    goto *((++pc)->instruction_ptr);

lt2:
    a_ptr = (sp_value - 1);
    a_ptr->type = ( pc->op[0].i < a_ptr->i && a_ptr->type != nil) ? T : nil; 
    goto *((++pc)->instruction_ptr);

lte2:
    a_ptr = (sp_value - 1);
    a_ptr->type = ( pc->op[0].i <= a_ptr->i && a_ptr->type != nil) ? T : nil; 
    goto *((++pc)->instruction_ptr);

eq2:
    a_ptr = (sp_value - 1);
    a_ptr->type = ( pc->op[0].i == a_ptr->i && a_ptr->type != nil) ? T : nil; 
    a_ptr->i = pc->op[0].i;
    goto *((++pc)->instruction_ptr);

setq:
    ((Variable_Data_t*)pc->op[0].adr)->value = (--sp_value)->i;
    goto *((++pc)->instruction_ptr);

gte:
    ret_ptr = (--sp_value);
    ret_ptr->type = ( ret_ptr->i >= (--sp_value)->i && ret_ptr->type != nil) ? T : nil; 
    *(sp_value++) = *ret_ptr;
    goto *((++pc)->instruction_ptr);

lt:
    ret_ptr = (--sp_value);
    ret_ptr->type = ( ret_ptr->i < (--sp_value)->i && ret_ptr->type != nil) ? T : nil;
    *(sp_value++) = *ret_ptr;
    goto *((++pc)->instruction_ptr);

}


