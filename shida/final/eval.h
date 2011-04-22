#ifndef EVAL
#define EVAL
static inline value_t* pop( void );
value_t* pop_ans( void );
static inline void push_i( int );
static inline void push_bool( enum eTYPE );
static inline void push_value_t( value_t*);



#endif /*EVAL*/
