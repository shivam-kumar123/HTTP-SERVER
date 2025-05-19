#include<stdio.h>
#include<sys/socket.h>
#include<errno.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main () {
    char buffer[BUFFER_SIZE];
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("web server (socket)");
        return 1;
    }
    struct sockaddr_in bind_addr;
    int bin_addr_len = sizeof(bind_addr);
    bind_addr.sin_family = PF_INET;
    bind_addr.sin_port = htons(PORT);
    bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sockfd, (struct sockaddr*) &bind_addr, bin_addr_len) != 0) {
        perror("web server (bind)");
        return 1;
    }
    if (listen(sockfd, SOMAXCONN) != 0) {
        perror("web server (listen)");
        return 1;
    }
    while (1) {
        int new_sockfd = accept(sockfd, (struct sockaddr*) &bind_addr, (socklen_t*) &bin_addr_len);
        if (new_sockfd == -1) {
            perror("web server (accept)");
            continue;
        }
        int bytes_read = read(new_sockfd, buffer, BUFFER_SIZE);
        if (bytes_read < 0) {
            perror("web server (read)");
            continue;
        }
        close(new_sockfd);
    }
    return 0;
}