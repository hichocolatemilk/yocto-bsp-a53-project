#include "tcp.h"

unsigned char key[32] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                         0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
                         0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
                         0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f};

unsigned char iv[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void *recv_thread(void *args){

    unsigned char decryptedtext[128] = {0};
    unsigned int decryptedtext_len = 0;
    unsigned char cipher_message[BUFFER_SIZE];
    unsigned char decrypted_message[BUFFER_SIZE];
    int serverSockfd = *(int *)args;
    int buffer_len;
    // char message[BUFFER_SIZE];
    while(1){
        buffer_len = recv(serverSockfd, cipher_message, BUFFER_SIZE - 1, 0);
        if (buffer_len > 0) {
            decryptedtext_len = decrypt(cipher_message, buffer_len, key, iv, decrypted_message);
            if(decryptedtext_len > 0){
                decrypted_message[decryptedtext_len] = '\0'; // 널 종료자 추가
                // printf("서버로부터 받은 암호화된 메시지: %s\n", cipher_message);
                printf("복호화된 메시지: %s\n", decrypted_message);
            } else {
                printf("메시지 복호화 실패\n");
            }
        }
    }
    return NULL; // 쓰레드 정상 종료
}

int main(){

    pthread_t recv_thread_id;
    int sockfd;
    int ciphertext_len = 0;
    unsigned char plain_message[BUFFER_SIZE];
    unsigned char cipher_message[BUFFER_SIZE];
    struct sockaddr_in server_addr;
    socklen_t server_addr_len = sizeof(server_addr);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&server_addr, 0, server_addr_len);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("192.168.35.125");
    server_addr.sin_port = htons(12345);
    connect(sockfd, (struct sockaddr *)&server_addr, server_addr_len);
    pthread_create(&recv_thread_id, NULL, recv_thread, &sockfd);
    pthread_detach(recv_thread_id); // 쓰레드 종료 시 자원 자동 해제
    while(1){
        printf("서버로 보낼 메시지 입력:\n");
        fgets(plain_message, BUFFER_SIZE, stdin);
        plain_message[strcspn((char *)plain_message, "\n")] = 0;
        ciphertext_len = encrypt(plain_message, strlen(plain_message), key, iv, cipher_message);
        send(sockfd, cipher_message, ciphertext_len, 0);
    }
    close(sockfd);
    return 0;
}