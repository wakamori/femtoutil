#include<stdio.h>
#include <string.h>
#include"main.h"
#include"token.h"
#include"tree.h"

void print_cons(cons_t* cons);

int main(){
    int i;
    first=NULL;
    last=NULL;
    int firstChar;
    int flag=0;
    char str[1000];
    //memset(variable_Data,0,sizeof(variable_Data_t)*100);
    printf("size%d\n",sizeof(cons_t));
    for (i = 0; i < 100; i++){
       variable_Data[i].name[0]='\0';
       function_Data[i].name[0]='\0';
    }       
    read_Token("defun fib (n) (if (< n 3) 1 (+ ( fib (- n 1)) ( fib (- n 2))))");
    print_cons(read_Expression(CONTINUE,0));
    while(1){
    if(flag==0){
	printf(">>>");
    }
    fgets(str,sizeof(str),stdin);
    flag=read_Token(str);
    firstChar=0;
    if(flag == 0 ){
	print_cons(read_Expression(CONTINUE,0));
    }
    }
}

void print_cons(cons_t* cons){
    if(cons->type==NUM){
	printf("%d\n",cons->u.i);
    }else if(cons->type==T){
	printf("T\n");
    }else if(cons->type==nil){
	printf("nil\n");
    }else if(cons->type==STR){
	printf("STR%s",cons->u.c);
    }else{
	//printf("error%d\n%d\n",cons->type,cons->u.i);
    }
    first=NULL;
    last=NULL;
}
