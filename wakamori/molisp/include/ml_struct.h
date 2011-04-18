enum ctype {
	T_INT,
	T_STRING,
	T_FLOAT
};

typedef struct cons_t {
	enum ctype ct;
	union {
		int ival;
		float fval;
		char *str;
		struct cons_t *branch;
	};
	struct cons_t *next;
} cons_t;
