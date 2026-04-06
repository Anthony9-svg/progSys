#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    int desc=atoi(getenv("DESC"));
    char message[20];
    read(desc,message,sizeof(message));
    printf("Voici le message de mon pere: %s\n",message);
    printf("\n");
    return 0;
}