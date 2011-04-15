#include "lisugimoto.h"

int main(int argc, char *argv[])
{
	const char *prompt = ">>> ";
	char *line = NULL;
	cons_t *root = NULL;
	cons_t *result = NULL;
	while (1) {
		line = readline(prompt);
		root = sgmt_read(line);
		result = sgmt_eval(root);
		sgmt_print(result);
	}
}

