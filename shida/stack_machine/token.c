#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"main.h"
#include"token.h"
#include"data_structure.h"

int obj;
struct function_Data_t* ptr;

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


void analize_Expression(char* str){

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
            cons->type=STR;
            enq(cons);
        }
    }


}

int read_Expression(int mode,int argument){
    cons_t* cons[3];
    int depth=0;
    while(first!=NULL){
        cons[depth]=deq();
        switch( cons[depth]->type ){

            case OPEN:
                if ( mode == ONCE ){
                    return read_Expression( CONTINUE, argument );
                }else{
                    push ( read_Expression( CONTINUE, argument ) );
                }
                break;

            case CLOSE:
                return pop();
                break;

            case NUM:
                if ( depth == 0 ) {
                    return cons[depth]->u.i;
                } else {
                    push( cons[depth]->u.i );
                }
                break;

            case STR:
                ptr = searchf( cons[depth]->u.c );
                if ( ptr == NULL ){
                    if ( depth == 0 ){
                        return getq( cons[depth]->u.c );
                    } else {
                        push( getq( cons[depth]->u.c ) );
                    }
                } else {
                    push( getf( cons[depth]->u.c , argument , ptr ) );
                }
                break;

            case ARG:
                if(depth == 0){
                    return argument;
                } else {
                    push( argument );
                }
                break;

            case DEFUN:
                setf();
                break;

            case IF:
                if(read_Expression(ONCE,argument) == 0){
                    skip_Expression();
                    return read_Expression(ONCE,argument);
                }else{
                    return read_Expression(ONCE,argument);
                }
                break;
        }

        depth++;
        if(depth==3){
            switch(cons[0]->type){

                case PLUS:
                    push ( pop() + pop() );
                    break;

                case MINUS:
                    obj= -1 * pop();
                    push( obj + pop() );
                    break;

                case MUL:
                    push ( pop() * pop() );
                    break;

                case DIV:
                    push( (int)( ( 1.0 / (float)pop() ) * (float)pop() ) );
                    break;

                case GT:
                    if( pop() > pop() ){
                        push( 1 );
                    }else{
                        push( 0 );
                    }
                    break;

                case GTE:
                    if( pop() >= pop() ){
                        push( 1 );
                    }else{
                        push( 0 );
                    }
                    break;

                case LT:
                    if( pop() < pop() ){
                        push( 1 );
                    }else{
                        push( 0 );
                    }
                    break;

                case LTE:
                    if( pop() <= pop() ){
                        push( 1 );
                    }else{
                        push( 0 );
                    }
                    break;

                case EQ:
                    if( pop() == pop() ){
                        push( 1 );
                    }else{
                        push( 0 );
                    }
                    break;

                case SETQ:
                    setq(cons[1],cons[2]);
                    break;


            }
            depth=2;
        }
    }
    return pop();
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
