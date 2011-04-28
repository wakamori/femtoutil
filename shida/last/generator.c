#include <stdio.h>
#include"main.h"
void generate (AST* ast)
{
    switch(ast->type){
        case tok_plus:
        case tok_minus:
        case tok_mul:
        case tok_div:
        case tok_gt:
        case tok_gte:
        case tok_lt:
        case tok_lte:
        case tok_eq:
            generate(ast->LHS);
            generate(ast->RHS);
    }
}
