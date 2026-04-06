#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int n = atoi(argv[1]);
    int k = atoi(argv[2]);

    if (k == 0 || k == n)
    {
        printf("1\n");
        return 0;
    }

    pid_t p1 = fork();

    if (p1 == 0)
    {
        freopen("/tmp/bin1.txt", "w", stdout);

        char n1[10], k1[10];
        sprintf(n1, "%d", n - 1);
        sprintf(k1, "%d", k - 1);

        execl("./a.out", "a.out", n1, k1, NULL);
    }

    pid_t p2 = fork();

    if (p2 == 0)
    {
        freopen("/tmp/bin2.txt", "w", stdout);

        char n2[10], k2[10];
        sprintf(n2, "%d", n - 1);
        sprintf(k2, "%d", k);

        execl("./a.out", "a.out", n2, k2, NULL);
    }

    wait(NULL);
    wait(NULL);

    FILE *f1 = fopen("/tmp/bin1.txt", "r");
    FILE *f2 = fopen("/tmp/bin2.txt", "r");

    int a, b;
    fscanf(f1, "%d", &a);
    fscanf(f2, "%d", &b);

    fclose(f1);
    fclose(f2);

    printf("%d\n", a + b);

    return 0;
}