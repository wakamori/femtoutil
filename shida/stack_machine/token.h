#ifndef TOKEN
#define TOKEN
int read_Token(char* );
void analize_Expression(char* );
int read_Expression(int,int);
void skip_Expression(void);

void plus( void );
void minus( void );
void mul( void );
void d_iv( void );
void gt( void );
void gte( void );
void lt( void );
void lte( void );
void eq( void );
char str[1000];


#endif /*TOKEN*/
