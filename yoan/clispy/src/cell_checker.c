#include"../include/clispy.h"

void check_opr(Opt_t opr)
{
	printf("opr: ");
	switch (opr){
		case OP_NULL:
			printf("OP_NULL\n");
			break;
		case OP_ADD:
			printf("OP_ADD\n");
			break;
		case OP_SUB:
			printf("OP_SUB\n");
			break;
		case OP_MUL:
			printf("OP_MUL\n");
			break;
		case OP_DIV:
			printf("OP_DIV\n");
			break;
		case OP_GT:
			printf("OP_GT\n");
			break;
		case OP_LT:
			printf("OP_LT\n");
			break;
		case OP_GTE:
			printf("OP_GTE\n");
			break;
		case OP_LTE:
			printf("OP_LTE\n");
			break;
		case OP_IF:
			printf("OP_IF\n");
			break;
		case OP_SETQ:
			printf("OP_SETQ\n");
			break;
		case OP_DEFUN:
			printf("OP_DEFUN\n");
			break;
		case OP_MOD:
			printf("OP_MOD\n");
			break;
	}
}

void check_cell(Cell_t *p)
{
	printf("type: ");
	switch(p->type){
		case T_NULL:
			printf("T_NULL\n");
			break;
		case T_OP_PARENTHS:
			printf("T_OP_PARENTHS\n");
			break;
		case T_CL_PARENTHS:
			printf("T_CL_PARENTHS\n");
			break;
		case T_INT:
			printf("inum: %d\n",p->inum);
			break;
		case T_FLOAT:
			printf("float: %lf\n",p->fnum);
			break;
		case T_STR:
			printf("str: %s\n",p->str);
			break;
		case T_BOOL:
			printf("data: ");
			if(p->type == TRUE)
				printf("true\n");
			else
				printf("false\n");
			break;
		case T_OPR:
			printf("T_OPR\n");
			check_opr(p->opr);
			break;
		case T_FUNC:
			printf("func: %s\n",p->str);
			break;
		default:
			printf("type error. unknown type\n");
	}
}


