#include "lisp.h"
#include <string.h>

int next_token(Token *token){
	char *in = token->input;
	while(*in == ' ') in++;

	if(*in == '\0'){
		token->type = TOKEN_NULL;
		return 0;
	}else if(*in >= '0' && *in <= '9'){
		int n=0;
		while(*in >= '0' && *in <= '9'){
			n *= 10;
			n += *in - '0';
			in++;
		}
		token->type = TOKEN_INT;
		token->num = n;
	}else if(*in == '-' && in[1] >= '0' && in[1] <= '9'){
		in++;
		token->input = in;
		next_token(token);
		token->num = -token->num;
		return;
	}else if((*in >= 'a' && *in <= 'z') || (*in >= 'A' && *in <= 'Z')){
		token->type = TOKEN_STR;
		char *s = token->str;
		while(*in != ' ' && *in != '\0'){
			*s++ = *in++;
		}
		*s = '\0';
	}else{
		token->type = *in++;
	}
	token->input = in;
	return 1;
}

// add
cons_t *add_list(cons_t *l, int type, consvalue_t v){
	cons_t *top;
	if(l == NULL){
		l = (cons_t *)malloc(sizeof(cons_t));
		top = l;
	}else{
		top = l;
		while(l->cdr != NULL) l = l->cdr;
		l->cdr = (cons_t *)malloc(sizeof(cons_t));
		l = l->cdr;
	}
	l->type = type;
	l->v = v;
	l->cdr = NULL;
	return top;
}

cons_t *create_list(Token *token){
	cons_t *list = NULL;
	consvalue_t value;
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
			}else{
				printf("token error!! %s\n", token->str);
			}
			break;

		case ')':
			goto end;

		case '+':
		case '-':
		case '*':
		case '/':
		case '=':
		case '<':
		case '>':
			value.i = token->type;
			list = add_list(list, TYPE_OPERATE, value);
			break;

		default:
			printf("PARSER ERROR!!\n");
			//exit(0);
		}
	}
end:
	return list;
}

cons_t *compile(char *input){
	Token token;
	token.input = input;
	next_token(&token);
	return create_list(&token);
}


