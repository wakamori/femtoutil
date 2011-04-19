#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include"main.h"
#include"token.h"
#include"data_structure.h"

void print_cons ( void );

int main ( int argc,char* argv[] ){
    int i;
    int file_flag = 1;
    first = NULL;
    last = NULL;
    FILE *file;
    int firstChar;
    int flag=0;
    char str[1000];
    //memset(variable_Data,0,sizeof(variable_Data_t)*100);
    if( argc == 2){
        file = fopen(argv[1],"r");
    }
    for (i = 0; i < 100; i++ ){
        variable_Data[i].name[0] = '\0';
        function_Data[i].name[0] = '\0';
    }       
    while(1){
        if ( flag == 0 ){
            printf(">>>");
        }
        if( argc == 2 && file_flag == 1 ){
            if( fgets( str , sizeof(str),file) == NULL ){
                file_flag = 0;
                fclose( file );
                flag = 1;
                exit(0);
            } else {
                flag = read_Token(str);
            }
        }else{
            fgets( str , sizeof(str) , stdin );
            flag = read_Token(str);
        }
        firstChar = 0;
        if(flag == 0 ){
            read_Expression(CONTINUE,0);
            print_cons();
        }
    }
}

void print_cons( void ){
    printf( "%d\n" , pop() );
    first=NULL;
    last=NULL;
}
