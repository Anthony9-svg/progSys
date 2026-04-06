#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 12345
#define MAX_LIGNE 1024

int main() {
    int sockfd;
    struct sockaddr_in serv_addr;
    char buffer[MAX_LIGNE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    while (1) {
        int n = read(sockfd, buffer, MAX_LIGNE - 1);

        if (n <= 0)
            break;

        buffer[n] = '\0';

        printf("%s", buffer);

        if (strstr(buffer, "Saisissez") != NULL) {
            fgets(buffer, MAX_LIGNE, stdin);
            write(sockfd, buffer, strlen(buffer));
        }
    }

    close(sockfd);
    return 0;
}