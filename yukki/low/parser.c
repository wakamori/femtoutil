#include "lisp.h"
#include <string.h>
#include <ctype.h>

int next_token(Token *token){
	char *in = token->input;
	while(isspace(*in)) in++;

	if(*in == '\0'){
		token->input = in;
		return 0;
	}else if(isdigit(*in)){
		int n=0;
		do{
			n *= 10;
			n += *in - '0';
			in++;
		}while(isdigit(*in));
		token->type = TOKEN_INT;
		token->num = n;
	}else if(*in == '-' && isdigit(*(in+1))){
		in++;
		token->input = in;
		next_token(token);
		token->num = -token->num;
		return 1;
	}else if(isalpha(*in)){
		token->type = TOKEN_STR;
		char *s = token->str;
		while(isalpha(*in) || isdigit(*in)){
			*s++ = tolower(*in++);
		}
		*s = '\0';
	}else{
		token->type = *in++;
	}
	token->input = in;
	return 1;
}

// add last
cons_t *add_list(cons_t *l, int type, consvalue_t v){
	cons_t *top;
	if(l == NULL){
		l = (cons_t *)low_malloc(sizeof(cons_t));
		top = l;
	}else{
		top = l;
		while(l->cdr != NULL) l = l->cdr;
		l->cdr = (cons_t *)low_malloc(sizeof(cons_t));
		l = l->cdr;
	}
	l->type = type;
	l->v = v;
	l->cdr = NULL;
	return top;
}

int depth = 0;

cons_t *create_list(Token *token){
	cons_t *list = NULL;
	consvalue_t value;

	depth++;

	while(next_token(token)){
		switch(token->type){
		case '(':
			value.car = create_list(token);
			list = add_list(list, TYPE_CAR, value);
			break;

		case TOKEN_INT:
			value.i = token->num;
			list = add_list(list, TYPE_INT, value);
			break;

		case TOKEN_STR:
			if(strcmp(token->str, "if") == 0){
				list = add_list(list, TYPE_IF, value);
			}else if(strcmp(token->str, "t") == 0){
				list = add_list(list, TYPE_T, value);
			}else if(strcmp(token->str, "nil") == 0){
				list = add_list(list, TYPE_NIL, value);
			}else if(strcmp(token->str, "setq") == 0){
				list = add_list(list, TYPE_SETQ, value);
			}else if(strcmp(token->str, "defun") == 0){
				list = add_list(list, TYPE_DEFUN, value);		
			}else{
				// variable or function ?
				int length = strlen(token->str);
				value.str = (char *)low_malloc(length+1);
				strcpy(value.str, token->str);
				list = add_list(list, TYPE_STR, value);
			}
			break;

		case ')':
			goto end;

		case '+':
		case '-':
		case '*':
		case '/':
		case '=':
			value.i = token->type;
			list = add_list(list, TYPE_OPERATE, value);
			break;

		case '<':
		case '>':
			// cannot parse <=, >= !!!!
			value.i = token->type;
			list = add_list(list, TYPE_OPERATE, value);
			break;

		default:
			printf("PARSER ERROR!!\n");
			//exit(0);
		}
		if(depth == 1) break;
	}
end:
	depth--;
	return list;
}

