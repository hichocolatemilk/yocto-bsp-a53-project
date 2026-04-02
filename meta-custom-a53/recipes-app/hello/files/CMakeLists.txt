cmake_minimum_required(VERSION 3.10)
project(hello)

# 1. 실행 파일 생성 (hello.c를 빌드해서 hello 실행 파일 만들기)
add_executable(hello hello.c)

# 2. 설치 경로 지정 (Yocto의 bin 디렉토리에 설치)
# CMAKE_INSTALL_BINDIR는 Yocto가 자동으로 /usr/bin으로 설정해줍니다.
include(GNUInstallDirs)
install(TARGETS hello DESTINATION ${CMAKE_INSTALL_BINDIR})
