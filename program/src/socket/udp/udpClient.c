#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define BUFFER_SIZE 1024

int main()
{
    int sockfd;
    int buffer_len;
    char message[BUFFER_SIZE];
    struct sockaddr_in server_addr, client_addr;
    socklen_t server_addr_len = sizeof(server_addr);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&server_addr, 0, server_addr_len);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("192.168.35.125");
    server_addr.sin_port = htons(12345);

    while(1){
        printf("서버로 보낼 메시지 입력:");
        fgets(message, BUFFER_SIZE, stdin);
        message[strcspn(message, "\n")] = '\0';
        sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)&server_addr, server_addr_len);
        buffer_len = recvfrom(sockfd, message, BUFFER_SIZE, 0, (struct sockaddr *)&server_addr, &server_addr_len);
        if (buffer_len > 0) {
            message[buffer_len] = '\0';  // 받은 데이터 바로 뒷자리에 널 문자를 넣어서 문을 닫음!
            printf("서버로부터 받은 메시지: %s\n", message);
        }
    }
    close(sockfd);
    return 0;
}