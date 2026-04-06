#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main(void)
{
    int tube_fils[2];   // fils -> père
    int tube_pere[2];   // père -> fils
    pid_t pid;
    char mot[100], ligne[256];
    FILE *f;
    int trouve = 0;

    int tty_in = dup(STDIN_FILENO);
    int tty_out = dup(STDOUT_FILENO);

    if (tty_in == -1 || tty_out == -1) {
        perror("dup");
        exit(1);
    }

    if (pipe(tube_fils) == -1 || pipe(tube_pere) == -1) {
        perror("pipe");
        exit(1);
    }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        close(tube_fils[0]);
        close(tube_pere[1]);

        printf("Donnez un mot : ");
        fflush(stdout);

        if (scanf("%99s", mot) != 1) {
            fprintf(stderr, "Erreur lecture mot\n");
            exit(1);
        }

        dup2(tube_fils[1], STDOUT_FILENO);
        close(tube_fils[1]);

        printf("%s\n", mot);
        fflush(stdout);

        dup2(tube_pere[0], STDIN_FILENO);
        close(tube_pere[0]);

        if (scanf("%d", &trouve) != 1) {
            fprintf(stderr, "Erreur lecture résultat\n");
            exit(1);
        }

        dup2(tty_out, STDOUT_FILENO);

        if (trouve)
            printf("Le mot est dans le fichier\n");
        else
            printf("Le mot n'est pas dans le fichier\n");

        close(tty_in);
        close(tty_out);
        exit(0);
    }
    else {
        close(tube_fils[1]);
        close(tube_pere[0]);

        dup2(tube_fils[0], STDIN_FILENO);
        close(tube_fils[0]);

        dup2(tube_pere[1], STDOUT_FILENO);
        close(tube_pere[1]);

        if (scanf("%99s", mot) != 1) {
            fprintf(stderr, "Erreur lecture mot dans le père\n");
            exit(1);
        }

        f = fopen("file", "r");
        if (!f) {
            perror("fopen");
            exit(1);
        }

        while (fgets(ligne, sizeof(ligne), f) != NULL) {
            if (strstr(ligne, mot) != NULL) {
                trouve = 1;
                break;
            }
        }
        fclose(f);

        printf("%d\n", trouve);
        fflush(stdout);

        wait(NULL);

        close(tty_in);
        close(tty_out);
    }

    return 0;
}