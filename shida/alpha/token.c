#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"main.h"
#include"token.h"
#include"tree.h"
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
                //		printf("num%d\n",j);
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
            //	    printf("num%d\n",j);
            if(j!=0)
                index+=(int)log10((double)j);
            cons->u.i=j;
            enq(cons);
        }else if(c=='('){
            cons=(cons_t*)malloc_original(sizeof(cons_t));
            cons->type=OPEN;
            //printf("kakko\n");
            enq(cons);
            //analize_Expression(&str[index+1]);
            //for(j = index+1; str[j] != '\0'; j++){
            //	if(str[j]=='('){
            //	    count++;
            //	}else if(str[j]==')'){
            //	    count--;
            //	    if(count<0){
            //		index=j+1;
            //		break;
            //	    }
            //	}
            //    }
        }else if(c==')'){
            cons=(cons_t*)malloc_original(sizeof(cons_t));
            //	    printf("close\n");
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

cons_t* read_Expression(int mode,int argument){
    cons_t* cons[3];
    cons_t* cons_copy;
    int depth=0;
    while(first!=NULL){
        cons[depth]=deq();
        if(depth==0&&(cons[depth]->type==NUM)){
            return cons[depth];
        }
        //printf("deq\n");
        if(cons[depth]->type==OPEN){
            if(mode==ONCE){
                return read_Expression(CONTINUE,argument);
            }else{
                cons[depth]=read_Expression(CONTINUE,argument);
            }
        }else if(cons[depth]->type==CLOSE){
            return cons[depth-1];
            //return cons_return;
        }else if(cons[depth]->type==ARG){
            //cons[depth]->type=NUM;
            cons[depth]->u.i=argument;
            if(depth == 0){
                return cons[depth];
            }
        }else if(cons[depth]->type==DEFUN){
            setf();
        }else if(cons[depth]->type == STR && cons[0]->type != SETQ ){
            cons_copy=getf(cons[depth]->u.c,argument);
            if(cons_copy==NULL){
                cons[depth]->type=NUM;
                cons[depth]->u.i=getq(cons[depth]->u.c);
            }else{
                cons[depth]=cons_copy;
            }
        }else if(cons[depth]->type==IF){
            /*printf("if\n");
              if(deq()->type==OPEN){
              if(read_Expression(CONTINUE,argument)->boolean==nil){
              printf("false\n");
              if(deq()->type==OPEN){
              skip_Expression();
              return read_Expression(CONTINUE,argument);
              }else{
              return read_Expression(CONTINUE,argument);
              }
              }else{
              printf("true\n");
              cons[depth]=deq();
              if(cons[depth]->type==OPEN){
            //cons[depth]=read_Expression(CONTINUE,argument);
            return read_Expression(CONTINUE,argument);
            if(deq()->type==OPEN){
            skip_Expression();
            }
            return cons[depth];
            }else{
            if(deq()->type==OPEN){
            skip_Expression();
            }
            return cons[depth];
            }
            }
            }else{
            first=NULL;
            last=NULL;
            }*/
            if(read_Expression(ONCE,argument)->boolean == nil){
                //printf("false\n");
                skip_Expression();
                return read_Expression(ONCE,argument);
            }else{
                //printf("true\n");
                cons[depth]= read_Expression(ONCE,argument);
                skip_Expression();
                return cons[depth];
            }
        }

        depth++;
        if(depth==3){
            switch(cons[0]->type){

                case PLUS:
                    cons[1]->u.i=cons[1]->u.i+cons[2]->u.i;
                    break;

                case MINUS:
                    cons[1]->u.i=cons[1]->u.i-cons[2]->u.i;
                    break;

                case MUL:
                    cons[1]->u.i=cons[1]->u.i*cons[2]->u.i;
                    break;

                case DIV:
                    cons[1]->u.i=cons[1]->u.i/cons[2]->u.i;
                    break;

                case GT:
                    if(cons[1]->u.i < cons[2]->u.i){
                        cons[1]->boolean=T;
                    }else{
                        cons[1]->boolean=nil;
                    }
                    break;

                case GTE:
                    if(cons[1]->u.i <= cons[2]->u.i){
                        cons[1]->boolean=T;
                    }else{
                        cons[1]->boolean=nil;
                    }
                    break;

                case LT:
                    if(cons[1]->u.i > cons[2]->u.i){
                        cons[1]->boolean=T;
                    }else{
                        cons[1]->boolean=nil;
                    }
                    break;

                case LTE:
                    if(cons[1]->u.i >= cons[2]->u.i){
                        cons[1]->boolean=T;
                    }else{
                        cons[1]->boolean=nil;
                    }
                    break;

                case EQ:
                    if(cons[1]->u.i==cons[2]->u.i){
                        cons[1]->boolean=T;
                    }else{
                        cons[1]->boolean=nil;
                    }
                    break;

                case SETQ:
                    cons[1]->type=SETQ;
                    setq(cons[1],cons[2]);
                    break;


            }
            depth=2;
        }
    }
    return cons[depth-1];
}

/*void setq(cons_t* cons1, cons_t* cons2){
  int hashNum;
  struct variable_Data_t* p;
  struct variable_Data_t* next_p;
  hashNum=((int)(cons1->u.c[0]) * (int)(cons1->u.c[1])) % (sizeof(variable_Data)/sizeof(variable_Data[0]));
  p=&variable_Data[hashNum];
  while(1){
  if(p->name[0] == '\0'){
  strcpy(p->name,cons1->u.c);
  p->value = cons2->u.i;
  break;
  }else if(p->name == cons1->u.c){
  break;
  }else if(p->next == NULL){
  printf("null");
  next_p = (struct variable_Data_t*)malloc(sizeof(struct variable_Data_t));
  memset( next_p, 0, sizeof(variable_Data_t));
  p->next = next_p;
  p=next_p;
  }else{
  p=p->next;
  }
  }
  }
  int getValue(char* str){
  int hashNum;
  struct variable_Data_t* p;
  hashNum = ((int)str[0] * (int)str[1]) % (sizeof(variable_Data)/sizeof(variable_Data[0]));
  p = &variable_Data[hashNum];
  while(1){
  if(strcmp(p->name,str) == 0){
  return p->value;
  }else if(p->next != NULL){
  p=p->next;
  }else{
  return -1;
  }
  }
  }*/
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

