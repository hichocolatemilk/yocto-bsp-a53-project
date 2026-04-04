cmake_minimum_required(VERSION 3.10)
project(openSSLProject)

# 1. 헤더 위치 알려주기
include_directories(include)

# 2. OpenSSL 라이브러리 연결 준비
find_package(OpenSSL REQUIRED)

# 3. 실행 파일 만들기 (필요한 파일들만 골라넣기)
add_executable(tcp_secure_client 
    src/socket/tcp/tcpClientThread.c 
    src/openssl/AES256-CBC_TEST.c
)

# 4. 필요한 라이브러리(pthread, crypto) 링크
target_link_libraries(tcp_secure_client 
    PRIVATE 
    OpenSSL::Crypto 
    pthread
)
