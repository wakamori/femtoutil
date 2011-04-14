#include "lisp.h"

void next_token(Token *token){
	char *in = token->input;
	while(*in == ' ') in++;

	if(*in == '\0'){
		token->type = TOKEN_NULL;
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
}

// add last
Cons *add_list(Cons *l, int type, Data data){
	Cons *top;
	if(l == NULL){
		l = (Cons *)malloc(sizeof(Cons));
		top = l;
	}else{
		top = l;
		while(l->cdr != NULL) l = l->cdr;
		l->cdr = (Cons *)malloc(sizeof(Cons));
		l = l->cdr;
	}
	l->type = type;
	l->data = data;
	l->cdr = NULL;
	return top;
}

Cons *create_list(Token *token){
	Cons *list = NULL;
	Data data;

	while(1){
		next_token(token);
		switch(token->type){
		case '(':
			data.car = create_list(token);
			list = add_list(list, TYPE_LIST, data);
			break;

		case TYPE_NULL:
			printf("PARSER ERROR NULL!!\n");
			break;

		case TYPE_INT:
			data.i = token->num;
			list = add_list(list, TYPE_INT, data);
			break;
			
		case ')':
			goto end;

		case '+':
			list = add_list(list, TYPE_PLUS, data);
			break;
		case '-':
			list = add_list(list, TYPE_MINUS, data);
			break;
		case '*':
			list = add_list(list, TYPE_MULTI, data);
			break;
		case '/':
			list = add_list(list, TYPE_DIV, data);
			break;

		default:
			printf("PARSER ERROR!!\n");
			//exit(0);
		}
	}
end:
	return list;
}

Cons *compile(char *input){
	Token token;
	token.input = input;

	next_token(&token);
	return create_list(&token);
}


