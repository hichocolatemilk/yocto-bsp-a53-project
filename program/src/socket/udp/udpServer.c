#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_SIZE 1024

int main(){

    int sockfd;
    char buffer[BUFFER_SIZE];
    int buffer_len;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    socklen_t server_addr_len = sizeof(server_addr);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("192.168.35.126");
    server_addr.sin_port = htons(12345);
    bind(sockfd, (struct sockaddr *)&server_addr, server_addr_len);
    while(1){
        buffer_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_addr_len);
        buffer[buffer_len] = '\0';
        printf("클라이언트로부터 받은 메시지: %s\n", buffer);
        sendto(sockfd, buffer, buffer_len, 0, (struct sockaddr *)&client_addr, client_addr_len);
        printf("클라이언트로 메시지 보냄: %s\n", buffer);
    }
    close(sockfd);
    return 0;
}