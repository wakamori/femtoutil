/**
 * type of cons cell
 */
enum ctype {
	T_START,
	T_ADD,
	T_SUB,
	T_MUL,
	T_DIV,
	T_INT,
	T_STRING,
	T_FLOAT
};

/**
 * state
 */
typedef enum {
	S_OUT,
	S_HEAD,
	S_BODY
} state_t;

/**
 * opcode
 */
typedef enum {
	RET,
	PUSH,
	PUSH0,
	PUSH1,
	PUSH2,
	LDA,
	DEC,
	INC,
	ADD,
	SUB,
	MUL,
	DIV,
	NOP
} op_t;

/**
 * cons cell
 */
typedef struct cons_t {
	enum ctype type;
	union {
		int ival;
		float fval;
		char *str;
		struct cons_t *car;
	};
	struct cons_t *cdr;
} cons_t;

/**
 * code
 */
typedef struct {
	union {
		op_t op;
		const void *adr;
	};
	int opr;
} code_t;

/**
 * VM register
 */
typedef struct {
	struct {
		int a;
	} reg;
	int stack[32];
	int sp;
} vm_t;
