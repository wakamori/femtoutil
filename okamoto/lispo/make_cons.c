#include<stdio.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<string.h>
#include"lispo.h"

void make_cons(cons_t* p, FILE* fp)
{
	int i = 0;
	int x = 0;
	int b_counter = -1;
	int minus_flag = 0;
	int e_b_flag = 0;
	char* line_temp;
	char* line;
	char line1[100];

	if(fp != NULL){
		if((fgets(line1, 100, fp)) == NULL) {
			free(heap);
			fclose(fp);
			exit(0);
		}
		line = strndup(line1, strlen(line1));
	} else {
		line = readline(">>> ");
	}

	if(strncmp(line, "quit", 4) == 0){
		printf("\n");
		free(heap);
		exit(0);
	}
	while(b_counter != 0){
		if(b_counter == -1)
			b_counter++;
		if(line[i] == '\0'){
			if(fp != NULL){
				if((fgets(line1, 100, fp)) == NULL) {
					free(heap);
					if(fp != NULL)
						fclose(fp);
					exit(0);
				}
				line = strndup(line1, strlen(line1));
			} else {
				line = readline("    ");
				i = 0;
			}
			continue;
		}
		else if(line[i] == SPACE){
			if(minus_flag == 1){
				p->type = SUB;
				cons_t* temp = allocate(sizeof(cons_t));
				format_cons_t(temp);
				p->cdr = temp;
				p = temp;
				i++;
				minus_flag = 0;
				continue;
			}
			i++;
			continue;
		}
		else if(line[i] == S_BRACKET){
			p->type = S_BRACKET;
			b_counter++;

			s_b_stack* s_b_s_temp = allocate(sizeof(s_b_stack));
			format_s_b_stack(s_b_s_temp);
			s_b_s_temp->ps = p;
			s_b_s_temp->next = s_b_s_head;
			s_b_s_head = s_b_s_temp;

			cons_t* temp = allocate(sizeof(cons_t));
			format_cons_t(temp);
			p->u.car = temp;
			p = temp;
			i++;
			continue;
		}
		else if(line[i] == E_BRACKET){
			if(b_counter == 0){
				printf("syntax error!!\n");
				free(heap);
				if (fp != NULL)
					fclose(fp);
				exit(0);
			}
			if(e_b_flag == 1){
				line_temp = &line[i + 1];
				line[i] = '\0';
				i = 0;
				p->u.value = line;
				if(strncmp(p->u.value, "if", 2) == 0)
					p->type = IF;
				else if(strncmp(p->u.value, "defun", 5) == 0)
					p->type = DEF;
				else if(strncmp(p->u.value, "setq", 4) == 0)
					p->type = SETQ;
				else
					p->type = STRING;
				line = line_temp;
				cons_t* temp = allocate(sizeof(cons_t));
				format_cons_t(temp);
				p->cdr = temp;
				p = temp;
			}
			p->type = E_BRACKET;
			b_counter--;

			if(b_counter == 0)
				break;

			cons_t* temp = allocate(sizeof(cons_t));
			format_cons_t(temp);
			s_b_s_head->ps->cdr = temp;
			s_b_s_head = s_b_s_head->next;
			p = temp;
			if(e_b_flag != 1)
				i++;
			e_b_flag = 0;
			continue;
		}
		else if(line[i] == ADD){
			if(b_counter == 0){
				printf("syntax error!!\n");
				free(heap);
				if (fp != NULL)
					fclose(fp);
				exit(0);
			}
			p->type = ADD;

			cons_t* temp = allocate(sizeof(cons_t));
			format_cons_t(temp);
			p->cdr = temp;
			p = temp;
			i++;
			continue;
		}
		else if(line[i] == SUB){
			if(b_counter == 0){
				printf("syntax error!!\n");
				free(heap);
				if (fp != NULL)
					fclose(fp);
				exit(0);
			}
			minus_flag = 1;
			i++;
		}
		else if(line[i] == MULT){
			if(b_counter == 0){
				printf("syntax error!!\n");
				free(heap);
				if (fp != NULL)
					fclose(fp);
				exit(0);
			}
			p->type = MULT;

			cons_t* temp = allocate(sizeof(cons_t));
			format_cons_t(temp);
			p->cdr = temp;
			p = temp;
			i++;
			continue;
		}
		else if(line[i] == DEV){
			if(b_counter == 0){
				printf("syntax error!!\n");
				free(heap);
				if (fp != NULL)
					fclose(fp);
				exit(0);
			}
			p->type = DEV;

			cons_t* temp = allocate(sizeof(cons_t));
			format_cons_t(temp);
			p->cdr = temp;
			p = temp;
			i++;
			continue;
		}
		else if(line[i] == L_THAN){
			if(b_counter == 0){
				free(heap);
				if (fp != NULL)
					fclose(fp);
				printf("syntax error!!\n");
				exit(0);
			}
			p->type = L_THAN;

			cons_t* temp = allocate(sizeof(cons_t));
			format_cons_t(temp);
			p->cdr = temp;
			p = temp;
			i++;
			continue;
		}
		else if(line[i] == M_THAN){
			if(b_counter == 0){
				printf("syntax error!!\n");
				free(heap);
				if (fp != NULL)
					fclose(fp);
				exit(0);
			}
			p->type = M_THAN;

			cons_t* temp = allocate(sizeof(cons_t));
			format_cons_t(temp);
			p->cdr = temp;
			p = temp;
			i++;
			continue;
		}
		else if('0' <= line[i] && line[i] <= '9'){
			if(b_counter == 0){
				printf("syntax error!!\n");
				free(heap);
				if (fp != NULL)
					fclose(fp);
				exit(0);
			}
			while('0' <= line[i] && line[i] <= '9'){
				if(minus_flag == 1)
					x = x * 10 - decode(line[i]);
				else
					x = x * 10 + decode(line[i]);
				i++;
			}

			p->type = NUMBER;
			p->u.ivalue = x;
			x = 0;

			cons_t* temp = allocate(sizeof(cons_t));
			format_cons_t(temp);
			p->cdr = temp;
			p = temp;
			continue;
		}
		else{
			if(b_counter == 0){
				printf("syntax error!!\n");
				free(heap);
				if (fp != NULL)
					fclose(fp);
				exit(0);
			}
			line = &line[i];
			i = 0;
			while(line[i] != SPACE && line[i] != COMMA && line[i] != E_BRACKET)
				i++;
			if(line[i] == SPACE || line[i] == COMMA){
				line_temp = &line[i + 1];
				line[i] = '\0';
				p->u.value = line;
				if(strncmp(p->u.value, "if", 2) == 0)
					p->type = IF;
				else if(strncmp(p->u.value, "defun", 5) == 0)
					p->type = DEF;
				else if(strncmp(p->u.value, "setq", 4) == 0)
					p->type = SETQ;
				else
					p->type = STRING;
				line = line_temp;
				i = 0;
			}
			else if(line[i] == E_BRACKET){
				e_b_flag = 1;
				continue;
			}
			cons_t* temp = allocate(sizeof(cons_t));
			format_cons_t(temp);
			p->cdr = temp;
			p = temp;
			continue;
		}
	}
	return;
}

