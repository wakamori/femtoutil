#include <stdio.h>
#include"stack_data.h"

int data[MAX];
int* sp=data;

void push ( int e ) {
    if( sp == &data[MAX] ){
        printf("over flow\n");
        exit(1);
    }
    *(sp++) = e;
}
int pop ( void ){
    return *(--sp);
}
