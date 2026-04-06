#include <unistd.h>

int main(int argc,char** argv)
{
    execl("/usr/bin/cp","cp","-r",argv[1],".",NULL);
}