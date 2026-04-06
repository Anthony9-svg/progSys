#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int n;

    printf("Nombre d'elements : ");
    scanf("%d", &n);

    int *tab = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        printf("tab[%d] = ", i);
        scanf("%d", &tab[i]);
    }

    int fd = open("tableau.bin",
                  O_WRONLY | O_CREAT | O_TRUNC,
                  0624);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    write(fd, tab, n * sizeof(int));

    close(fd);
    free(tab);

    return 0;
}