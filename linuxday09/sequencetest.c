#include <my_header.h>

/* Usage:  */
int main(int argc, char *argv[]){                                  
    int a= 0x12345678;
    char *p = (char *)&a;

    printf("a first is %x\n", *p);    
    return 0;
}

