#ifndef EVAL
#define EVAL
value_t* pop( void );
void push_i( int );
void push_pc( void );
void push_bool( enum eTYPE );
void push_value_t( value_t* );
void plus( void );
void minus( void );
void mul( void );
void div( void );
void end( void );
void gt( void );
void gte( void );
void lt( void );
void lte( void );
void eq( void );
void jmp( void );


#endif /*EVAL*/
