#include <stdio.h>
#include <stdlib.h>
int main(int argc,char** argv)
{
    printf("%s+%s=%d\n",argv[1],argv[2],atoi(argv[1])+atoi(argv[2]));
    return 0;
}