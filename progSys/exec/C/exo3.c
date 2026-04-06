#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
int main ()
{
    char* arg=malloc(10);
    printf("Donnez le nom du fichier: ");
    scanf("%s",arg);
    execl("/usr/bin/nano","nano",arg,NULL);
}