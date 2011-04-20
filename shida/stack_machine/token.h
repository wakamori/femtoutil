#ifndef TOKEN
#define TOKEN
int read_Token(char* );
int analize_Expression(char* );
void read_Expression(int);
void skip_Expression(void);

inline void plus( void );
inline void minus( void );
inline void mul( void );
inline void d_iv( void );
inline void gt( void );
inline void gte( void );
inline void lt( void );
inline void lte( void );
inline void eq( void );



char str[1000];


#endif /*TOKEN*/
