#include "tcp.h"

int main(){

    int sockfd;
    int buffer_len;
    char message[BUFFER_SIZE];
    struct sockaddr_in server_addr;
    socklen_t server_addr_len = sizeof(server_addr);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&server_addr, 0, server_addr_len);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("192.168.35.125");
    server_addr.sin_port = htons(12345);
    connect(sockfd, (struct sockaddr *)&server_addr, server_addr_len);
    while(1){
        printf("서버로 보낼 메시지 입력:");
        fgets(message, BUFFER_SIZE, stdin);
        send(sockfd, message, strlen(message), 0);
        buffer_len = recv(sockfd, message, BUFFER_SIZE, 0);
        if (buffer_len > 0) {
            message[buffer_len] = '\0';  // 받은 데이터 바로 뒷자리에 null 문자 추가
            printf("서버로부터 받은 메시지: %s\n", message);
        }
    }
    close(sockfd);
    return 0;
}