#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"main.h"
#include"token.h"
#include"data_structure.h"

void (*operation[])( void ) = {
    plus,
    minus,
    mul,
    d_iv,
    gt,
    gte,
    lt,
    lte,
    eq,
};

//int obj,a,b;
cons_t* ptr;

int read_Token(char* s){
    int level=0,count=0;
    int i,j=0;

    count=0;
    strcat(str,s);
    for (i = 0; i < strlen(str); i++) {
        if(str[i]=='('){
            count++;
        }else if(str[i]==')'){
            count--;
        }
    }
    if(count>0){
        return 1;
    }

    analize_Expression(str);

    str[0]='\0';
    return 0;
}


int analize_Expression(char* str){

    int count=0;
    int i,j;
    char c;
    char*c_ptr;
    char token[40];
    int index;
    for(index = 0; str[index]!='\0'; index++){
        c=str[index];
        if(c == '+'){
            cons=(cons_t*)malloc_original(sizeof(cons_t));
            cons->type=PLUS;
            enq(cons);
        }else if (c == '-'){
            if(str[index+1] <= 57 && str[index+1] >= 48){
                cons=(cons_t*)malloc_original(sizeof(cons_t));
                cons->type=NUM;
                j=atoi(&str[index]);
                index+=(int)log10((double)-j)+1;
                cons->u.i=j;
                enq(cons);
            }else{
                cons=(cons_t*)malloc_original(sizeof(cons_t));
                cons->type=MINUS;
                enq(cons);
            }
        }else if(c == '*'){
            cons=(cons_t*)malloc_original(sizeof(cons_t));
            cons->type=MUL;
            enq(cons);
        }else if(c == '/'){
            cons=(cons_t*)malloc_original(sizeof(cons_t));
            cons->type=DIV;
            enq(cons);
        }else if(c==' '){

        }else if(c<=57&&c>=48){
            cons=(cons_t*)malloc_original(sizeof(cons_t));
            cons->type=NUM;
            j=atoi(&str[index]);
            if(j!=0)
                index+=(int)log10((double)j);
            cons->u.i=j;
            enq(cons);
        }else if(c=='('){
            cons=(cons_t*)malloc_original(sizeof(cons_t));
            cons->type=OPEN;
            //printf("kakko\n");
            enq(cons);
        }else if(c==')'){
            cons=(cons_t*)malloc_original(sizeof(cons_t));
            // printf("close\n");
            cons->type=CLOSE;
            enq(cons);
        }else if(c=='>'){
            if(str[index+1]=='='){
                cons=(cons_t*)malloc_original(sizeof(cons_t));
                cons->type=LTE;
                enq(cons);
            }else{
                cons=(cons_t*)malloc_original(sizeof(cons_t));
                cons->type=LT;
                enq(cons);
            }
        }else if(c=='<'){
            if(str[index+1]=='='){
                cons=(cons_t*)malloc_original(sizeof(cons_t));
                cons->type=GTE;
                enq(cons);
            }else{
                cons=(cons_t*)malloc_original(sizeof(cons_t));
                cons->type=GT;
                enq(cons);
            }
        }else if(str[index] == '=' && str[index+1] == '='){
            cons=(cons_t*)malloc_original(sizeof(cons_t));
            cons->type=EQ;
            enq(cons);
        }else if(c == '\0'){

        }else if(str[index] == 'i' && str[index+1] == 'f' && str[index+2] == ' '){
            cons=(cons_t*)malloc_original(sizeof(cons_t));
            cons->type=IF;
            cons->f = NULL;
            enq(cons);
            index++;
        }else if(str[index] == 's' && str[index+1] == 'e' && str[index+2] == 't' && str[index+3] == 'q' &&str[index+4] == ' '){
            cons=(cons_t*)malloc_original(sizeof(cons_t));
            cons->type=SETQ;
            enq(cons);
            index+=3;
        }else if(str[index] == 'd' && str[index+1] == 'e' && str[index+2] == 'f' && str[index+3] == 'u' &&str[index+4] == 'n' && str[index+5] == ' '){
            cons=(cons_t*)malloc_original(sizeof(cons_t));
            cons->type=DEFUN;
            enq(cons);
            index+=4;
        }else if(c >= 'a' && c <= 'z'){
            count=0;
            while(1){
                token[count]=str[index+count];
                if(str[index+count+1]==' ' || str[index+count+1]==')' || str[index+count+1]=='\0' || str[index+count+1]=='\n'){
                    break;
                }
                count++;
            }
            index+=count;
            token[count+1]='\0';
            cons=(cons_t*)malloc_original(sizeof(cons_t));
            cons->u.c=(char*)malloc_original(sizeof(char)*(count+2));
            strcpy(cons->u.c,token);
            ptr = searchf( cons->u.c );
            if( ptr != NULL ){
                cons->type = FUNC;
                cons->f = ptr;
            } else if ( getq( cons->u.c ) != NULL ){
                cons->type = NUM;
                cons->u.i = *getq( cons->u.c );
            } else {
                cons->type = STR;
            }
            enq(cons);
        }
    }


}

void read_Expression(int mode){
    cons_t* cons[3];
    int depth=0;
    while(first!=NULL){
        cons[depth]=deq();
        switch( cons[depth]->type ){

            case OPEN:
                if ( mode == ONCE ){
                    read_Expression( CONTINUE );
                    return;
                }else{
                    read_Expression( CONTINUE ) ;
                }
                break;

            case CLOSE:
                return ;

            case FUNC:
                getf( cons[depth]->f );
                break;

            case NUM:
                push( cons[depth]->u.i );
                if(depth == 0){
                    return;
                }
                break;

            case STR:
                printf("variable or function undefined\n");
                if(depth == 0){
                    return;
                }
                break;

            case ARG:
                push( get_arg() );
                if( depth == 0){
                    return;
                }
                break;

            case DEFUN:
                setf();
                break;

            case IF:
                read_Expression( ONCE );
                if(pop() == 0){
                    skip_Expression();
                    read_Expression( ONCE );
                    return;
                }else{
                    read_Expression( ONCE );
                    return;
                }
                break;

            case SETQ:
                setq( deq(), deq() );
                break;
        }
        depth++;
        if(depth==3){
            operation[ cons[ 0 ]->type ]();
            depth=2;
            continue;
        }
    }
}


void skip_Expression(){
    int skipCount=-1;
    cons_t* cons;
    while(first!=NULL){
        cons=deq();
        if(cons->type==OPEN){
            skipCount++;
        }else if(cons->type==CLOSE){
            skipCount--;
        }
        if(skipCount<0){
            break;
        }
    }
}

inline void plus( void )
{
    push( pop() + pop() );
}

inline void minus( void )
{
    int a = pop();
    int b = pop();
    push( b-a );
}

inline void mul( void )
{
    push( pop() * pop() );
}

inline void d_iv( void )
{
    int a = pop();
    int b = pop();
    push( b / a );
    //push( (int)( ( 1.0 / (float)pop() ) * (float)pop() ) );
}

inline void gt( void )
{

    int obj = (pop() > pop()) ? 1 : 0;
    push(obj);
}

inline void gte( void )
{
    int obj = (pop() >= pop()) ? 1 : 0;
    push(obj);
}

inline void lt( void )
{
    int ret = (pop() < pop()) ? 1 : 0;
    push(ret);
}

inline void lte( void )
{
    int ret = (pop() <= pop()) ? 1 : 0;
    push(ret);
}

inline void eq( void )
{
    int ret = (pop() == pop()) ? 1 : 0;
    push(ret);
}

