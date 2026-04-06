#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#define PORT 12345
#define MAX_LIGNE 1024
#define SAVE_DIR "/tmp/serveur/"

void creer_repertoire(const char *dir) {
    struct stat st = {0};
    if (stat(dir, &st) == -1) {
        mkdir(dir, 0700);
    }
}

int main() {
    creer_repertoire(SAVE_DIR);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("socket"); return 1; }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind"); return 1;
    }

    listen(sockfd, 5);
    printf("Serveur en écoute sur le port %d...\n", PORT);

    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);
        int client = accept(sockfd, (struct sockaddr*)&client_addr, &addrlen);
        if (client < 0) { perror("accept"); continue; }

        char ip_client[50];
        inet_ntop(AF_INET, &client_addr.sin_addr, ip_client, sizeof(ip_client));
        printf("Connexion de %s\n", ip_client);
        char archive_path[256];
        snprintf(archive_path, sizeof(archive_path), "%srep.tgz", SAVE_DIR);

        FILE *fp = fopen(archive_path, "wb");
        if (!fp) { perror("fopen"); close(client); continue; }

        char buffer[MAX_LIGNE];
        ssize_t n;
        while ((n = read(client, buffer, MAX_LIGNE)) > 0) {
            fwrite(buffer, 1, n, fp);
        }

        fclose(fp);
        close(client);
        char cmd[512];
        snprintf(cmd, sizeof(cmd), "cd %s && tar zxvf rep.tgz && rm rep.tgz", SAVE_DIR);
        system(cmd);

        printf("Archive reçue et extraite dans %s\n", SAVE_DIR);
    }

    close(sockfd);
    return 0;
}