#include "lisugimoto.h"

int sgmt_error(char *line,int *pos){
	int blcount = 0,brcount = 0;
	int len = 0,defun =0;
	while(line[*pos + len] != '\0'){
		switch (line[*pos + len] ){
			case '(':
				blcount ++;
				len ++;
				break;
			case ')':
				brcount ++;
				len ++;
				break;
			case 'd':
			if(line[*pos + len + 1] == 'e' && line[*pos + len + 2] == 'f' && line[*pos + len + 3] == 'u' && line[*pos + len + 4] == 'n'){
			defun ++;
			len = len + 5;
			}
			len ++;
			break;
			default :
			len ++;
			break;
		}
	}

	if (blcount != brcount){
		printf ("the number of '(' and ')' is different\n");
		return ERR;
	}
				len =0;

	while(line[*pos + len] != '\0'){
		switch (line[*pos + len]){
			case '(':
			len ++;
			break;
			case '+':
			case '-':
			if(line[*pos + len + 1] == '+' || line[*pos + len + 1] =='-' ){
			printf("such operand or modifier is invalid\n");
			return ERR;
			}
			len ++;
			break;
			default :
			if(line[*pos +len] != ' ' && blcount == 0){
				printf("This program needs '(' and so on\n");
				return ERR;
			}
			len ++;
			break;
		}
	}
	return 0;
}
