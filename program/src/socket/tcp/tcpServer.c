#include "tcp.h"

int main(){

    int sockfd, client_sockfd;
    int buffer_len;
    char message[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t server_addr_len = sizeof(server_addr);
    socklen_t client_addr_len = sizeof(client_addr);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&server_addr, 0, server_addr_len);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("192.168.35.126");
    server_addr.sin_port = htons(12345);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(sockfd, (struct sockaddr *)&server_addr, server_addr_len);
    listen(sockfd, 3);
    client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
    printf("서버 IP, 포트 번호 : %s :%d\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));
    while(1){
        buffer_len = recv(client_sockfd, message, BUFFER_SIZE, 0);
        if (buffer_len > 0) {
            message[buffer_len] = '\0';  // 받은 데이터 바로 뒷자리에 null 문자 추가
            printf("클라이언트한테 받은 메시지: %s\n", message);
        }
        send(client_sockfd, message, buffer_len, 0);
        printf("클라이언트한테 메시지 보냄: %s\n", message);
    }
    close(sockfd);
    return 0;
}