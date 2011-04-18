#include"../include/clispy.h"

#define matchToken(t, c) (!strncmp(t, c, strlen(t)))
#define P(p) printf("line: %s, %p, %d\n", p, p, __LINE__)

Cell_t* new_cell()
{
	Cell_t* c = (Cell_t*)malloc(sizeof(Cell_t));
	c->type = OP_NULL;
	//c->car = NULL;
	c->inum = 0;
	c->cdr = NULL;
	return c;
}

int isSpace(char c)
{
	if (c == ' ' || c == '\t')
		return 1;
	return 0;
}

char* addString(char** tmp, size_t size)
{
	char* ret = malloc(sizeof(char) * (size + 1));
	strncpy(ret, *tmp, size);
	ret[size] = '\0';
	*tmp += size;
	return ret;
}

int isBlank(char c)
{
	if (c == ' ' || c == '\t'){
		return 1;
	}
	return 0;
}

char* getToken(char** line)
{
	char c = *line[0];
	char* tmp = *line;
	int size = 0;

	while (c != '\0'){
		c = tmp[0];
		if (c == ' '){
			char* ret = addString(line, size);
			(*line)++;
			return ret;
		}
		else if (isBlank(c)){
			return addString(line, size);
		}
		else if (c == '(' || c == ')'){
			if (*line[0] != c){
				return addString(line, size);
			}
			if (isBlank(tmp[1])){
				char* ret = addString(line, size+1);
				(*line)++;
				return ret;
			}
			return addString(line, size+1);
		}
		tmp++; size++;
	}
	return '\0';
}

//char** split(char* line)
//{
//	char** ret = malloc(sizeof(char) * 8 * 32);
//	char* tmp = line;
//	char c = line[0];
//	int i = 0;
//	int size = 0;
//	while (c != '\0') {
//		c = line[0];
//		if (c == ' ' || c == '\0'){ // split line by space and EOL
//			ret[i] = addString(tmp, size);
//			i++;
//			tmp = ++line;
//			size = 0;
//		}
//		else if (c == '(' || c == ')'){ // split line by parenths
//			if (tmp[0] != c){
//				ret[i] = addString(tmp, size);
//				i++;
//				tmp = line;
//				size = 0;
//			}
//			ret[i] = addString(tmp, ++size);
//			i++;
//			tmp = ++line;
//			size = 0;
//		}
//		else {
//			size++; line++;
//		}
//	}
//	return ret;
//}

/*-------------------------------------------------*/
//                 parse functions 
/*-------------------------------------------------*/

Opt_t getOpt(char* token){
	if (!strncmp(token, "+", 1)){
		return OP_ADD;
	}
	else if (!strncmp(token, "-", 1)){
		return OP_SUB;
	}
	else if (!strncmp(token, "*", 1)){
		return OP_MUL;
	}
	else if (!strncmp(token, "/", 1)){
		return OP_DIV;
	}
	else if (!strncmp(token, ">", 1)){
		return OP_GT;
	}
	else if (!strncmp(token, "<", 1)){
		return OP_LT;
	}
	else if (!strncmp(token, ">=", 2)){
		return OP_GTE;
	}
	else if (!strncmp(token, "<=", 2)){
		return OP_LTE;
	}
	else if (!strncmp(token, "if", 2)){
		return OP_IF;
	}
	else if (!strncmp(token, "setq", 4)){
		return OP_SETQ;
	}
	else if (!strncmp(token, "defun", 5)){
		return OP_DEFUN;
	}
	else {
		return OP_NULL;
	}
}

bool isDigit(char* token){
	int tmp = atoi(token);
	if (tmp != 0) return TRUE;
	else {
		if (strncmp(token, "0", 1)){
			return FALSE;
		}
		return TRUE;
	}
}

bool isFunc(char* token)
{
	printf("%s\n", token);
	return FALSE;
}

bool isVariable(char* token)
{
	printf("%s\n", token);
	return FALSE;
}

bool isOpt(char* token)
{
	Opt_t tmp = getOpt(token);
	if (tmp == T_NULL) return FALSE;
	return TRUE;
}


void parse(Cell_t* head, char** line)
{
	char* token;
	Cell_t* tmpHead = head;
	while ((token = getToken(line)) != '\0'){
		if (matchToken(token, "(")){
			printf("T_OP_PARENTHS\n");
			CDR(head) = new_cell();
			head = CDR(head);
			TYPE(head) = T_OP_PARENTHS;
			parse(head, line);
		}
		else if (matchToken(token, ")")){
			printf("T_CL_PARENTHS\n");
			CDR(head) = new_cell();
			head = CDR(head);
			TYPE(head) = T_CL_PARENTHS;
			CDR(head) = tmpHead;
			return;
		}
		else if (isDigit(token)){
			printf("T_INT: %s\n", token);
			CDR(head) = new_cell();
			head = CDR(head);
			TYPE(head) = T_INT;
			INUM(head) = atoi(token);
		}
		else if (isOpt(token)){
			printf("T_Opt:%s\n", token);
			CAR(head) = new_cell();
			head = CAR(head);
			TYPE(head) = T_OPR;
			OPR(head) = getOpt(token);
		}
		else if (isVariable(token)){
			printf("T_VAR\n");
		}
		else if (isFunc(token)){
			printf("T_FUNC\n");
		}
		else {
		}
	}
}

