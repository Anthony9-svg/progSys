#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX_LIGNE 1024

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <archive_tgz> <serveur_ip> <port>\n", argv[0]);
        return 1;
    }

    char *archive = argv[1];      // Exemple: /tmp/rep.tgz
    char *serveur_ip = argv[2];
    int port = atoi(argv[3]);

    FILE *fp = fopen(archive, "rb");
    if (!fp) { perror("fopen"); return 1; }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) { perror("socket"); return 1; }

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, serveur_ip, &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        return 1;
    }

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        return 1;
    }

    char buffer[MAX_LIGNE];
    size_t n;
    while ((n = fread(buffer, 1, MAX_LIGNE, fp)) > 0) {
        if (write(sockfd, buffer, n) != n) {
            perror("write");
            break;
        }
    }

    printf("Archive envoyée avec succès.\n");

    fclose(fp);
    close(sockfd);
    return 0;
}