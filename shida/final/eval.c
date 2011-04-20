#include <stdio.h>
#include"main.h"
#include"eval.h"

void (*inst [])() = {
    push_i,
    push_pc,
    push_bool,
    plus,
    minus,
    mul,
    div,
    end,
    gt,
    gte,
    lt,
    lte,
    eq
};

void eval( void )
{
    value_t* ret;
    while( pc->instruction != END ){
        inst[ pc->instruction ]();
        pc++;
    }
    ret = pop();
    switch( ret->type ){
        case NUM:
            printf( "%d\n", ret->u.i );
            break;

        case T:
            printf( "T\n" );
            break;

        case nil:
            printf( "nil\n" );
            break;

    }
    pc++;
}


void push_value_t( value_t* t )
{
    *(sp_value++) = *t;
}

void push_i( int i)
{
    (sp_value++)->u.i = i;
}

void push_pc( void )
{
    (sp_value)->type = NUM;
    (sp_value++)->u.i = pc->op1.i;
}

void push_bool( enum eTYPE t )
{
    (sp_value++)->type = t;
}
value_t* pop( void )
{
    return (--sp_value);
}

void plus( void )
{
    int ret = pop()->u.i + pop()->u.i;
    push_i( ret );
}

void minus( void )
{
    int a = pop()->u.i;
    int ret = pop()->u.i - a;
    push_i( ret );
}

void mul( void )
{
    int ret = pop()->u.i * pop()->u.i;
    push_i( ret );
}

void div( void )
{
    int a = pop()->u.i;
    int ret = pop()->u.i / a;
    push_i( ret );
}

void gt( void )
{
    value_t* ret = pop();
    value_t* a = pop();
    ret->type = ( ret->u.i > a->u.i && a->type != nil) ? T : nil;
    push_value_t( ret );
}

void gte( void )
{
    value_t* ret = pop();
    value_t* a = pop();
    ret->type = ( ret->u.i >= a->u.i && a->type != nil) ? T : nil; 
    push_value_t( ret );
}

void lt( void )
{
    value_t* ret = pop();
    value_t* a = pop();
    ret->type = ( ret->u.i < a->u.i && a->type != nil) ? T : nil;
    push_value_t( ret );
}

void lte( void )
{
    value_t* ret = pop();
    value_t* a = pop();
    ret->type = ( ret->u.i <= a->u.i && a->type != nil) ? T : nil;
    push_value_t( ret );
}

void eq( void )
{
    value_t* ret = pop();
    ret->type = ( ret->u.i == pop()->u.i ) ? T : nil;
    push_value_t( ret );
}
