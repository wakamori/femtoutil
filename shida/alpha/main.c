#include<stdio.h>
#include <string.h>
#include"main.h"
#include"token.h"
#include"tree.h"

void print_cons(cons_t* cons);

int main(int argc,char* argv[]){
    int i;
    int file_flag=1;
    first=NULL;
    last=NULL;
    FILE *file;
    int firstChar;
    int flag=0;
    char str[1000];
    //memset(variable_Data,0,sizeof(variable_Data_t)*100);
    if(argc == 2){
        file = fopen(argv[1],"r");
    }
    for (i = 0; i < 100; i++){
       variable_Data[i].name[0]='\0';
       function_Data[i].name[0]='\0';
    }       
    while(1){
    if(flag==0){
	printf(">>>");
    }
    if(argc == 2 && file_flag==1){
	if(fgets(str,sizeof(str),file)==NULL){
	    file_flag=0;
	    fclose(file);
	    flag=1;
	} else {
	    flag=read_Token(str);
	}
    }else{
        fgets(str,sizeof(str),stdin);
        flag=read_Token(str);
    }
    firstChar=0;
    if(flag == 0 ){
	print_cons(read_Expression(CONTINUE,0));
    }
    }

}

void print_cons(cons_t* cons){
    if(cons->type==NUM){
	printf("%d\n",cons->u.i);
	exit(0);
    }else if(cons->type==T){
	printf("T\n");
    }else if(cons->type==nil){
	printf("nil\n");
    } else if (cons->type == DEFUN){
        printf( "define function\n" );
    }else if(cons->type==STR){
	printf("STR%s",cons->u.c);
    }else{
	printf("\n");
    }
    first=NULL;
    last=NULL;
}
