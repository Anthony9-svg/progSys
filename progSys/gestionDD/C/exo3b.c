#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main()
{
    struct stat st;

    if (stat("tableau.bin", &st) == -1)
    {
        perror("stat");
        return 1;
    }

    int n = st.st_size / sizeof(int);

    int *tab = malloc(n * sizeof(int));

    int fd = open("tableau.bin", O_RDONLY);

    if (fd == -1)
    {
        perror("open");
        return 1;
    }

    read(fd, tab, n * sizeof(int));

    close(fd);

    printf("Contenu du tableau :\n");

    for (int i = 0; i < n; i++)
    {
        printf("%d ", tab[i]);
    }

    printf("\n");

    free(tab);

    return 0;
}