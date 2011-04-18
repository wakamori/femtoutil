#include "../include/lispy.h"

int getParenth(char* line)
{
	int ret = 0, i = 0;
	char c = line[i++];

	while (c != '\0'){
		if (c == '('){
			ret++;
		}
		else if (c == ')'){
			ret--;
		}
		else {}
		c = line[i++];
	}
	return ret;
}

void readFile(char* str)
{
	FILE *fp;
	char s[64], tmp[256];
	char* line = tmp;
	memset(s, 0, 64);
	memset(line, 0, 256);
	int parenth = 0;
	if ((fp = fopen(str, "r")) == NULL){
		fprintf(stderr, "ERROR: cannot open file %s\n", str);
		exit(0);
	}

	while (fgets(s, 64, fp) != NULL){
		strcat(line, s);
		Cell_t* head = new_cell();
		parenth += getParenth(s);
		if (parenth == 0){
			parse(head, &line);
			eval(head);
			memset(line, 0, 256);
		}
	}
	if (parenth != 0){
		fprintf(stderr, "ERROR: sytax error\n");
		exit(0);
	}
	fclose(fp);
}

int main(int argc, char** argv)
{
	if (argc == 1){
		// interactive mode
	}
	else if (argc == 2){
		// file mode
		readFile(argv[1]);
	}
	else {
		fprintf(stderr, "ERROR: lispy need more arguments\n");
		exit(0);
	}
	return 0;
}
