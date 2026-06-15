# 🚀 Yocto Project: Raspberry Pi 3B+ Custom BSP & Secure Networking

1. Raspberry Pi 3B+ 환경에서 Yocto Project(Scarthgap)를 활용하여 Embedded Linux 빌드 환경을 구축하고, Device Tree 수정, Character Device Driver 통합, RootFS 커스터마이징을 수행한 BSP 학습 프로젝트입니다. 또한 OpenSSL 기반 TCP/UDP 통신 예제를 개발하여 Embedded Linux 환경에서의 빌드, 응용프로그램 개발 과정을 학습. 
2. 간단한 udp로 보드에서 qt로 CPU 사용률, Memory 사용률, 가동 시간을 받는 프로그램 학습.
---

## 🛠️ 사용 기술
* **하드웨어**: 라즈베리파이 3 모델 B+ (ARM Cortex-A53 아키텍처)
* **OS 빌드**: Yocto 프로젝트, WSL2 (우분투 22.04 환경)
* **커널**: 리눅스 커널 6.6.x (라즈베리파이 장기 지원 버전_Scarthgap)
* **사용 언어**: C, C++
* **개발 도구**: CMake, OpenSSL, Git, Qt Creator

---

## 🛠️ 핵심 요약
### 1. 커널 최적화 및 BSP 설계
* **커널 다이어트**: dmesg로 확인하여 불필요한 드라이버(BT, Sound 등)를 제거하여 커널 사이즈를 약 **3.2MB 감량**하고 부팅 속도를 dmesg 기준 eth0 link-up 시점 9.7초 -> 7.5초로 개선
* **Evidence**
- Before: `[    9.596739] lan78xx ... eth0: Link is Up`
- After:  `[    7.508648] lan78xx ... eth0: Link is Up`


* **시스템 매니저 전환**: 기존 SysVinit 환경에서는 서비스가 순차적으로 실행되면서 부팅 시간이 길어지는 문제가 있어, systemd로 전환하여 병렬 실행을 통해 부팅 속도와 관리 효율을 개선했습니다.
* **네트워크 안정화**: SysVinit 환경에서는 부팅 직후 네트워크가 정상적으로 연결되지 않아 일정 시간 지연 후 동작하는 문제가 있었고, 이를 통해 초기화 타이밍 이슈로 판단했습니다.
systemd로 전환한 이후에는 별도의 지연 없이도 네트워크가 정상적으로 동작하는 것을 확인했습니다.

### 2. 보안 네트워크 어플리케이션
* **독립적 개발 환경**: `populate_sdk`를 통해 툴체인을 추출하여 타겟 보드에 최적화된 **SDK 크로스 컴파일 환경**을 구축했습니다.
* **소캣 통신 구현**:
* ### TCP
- pthread 기반 멀티스레드 서버 구현
- OpenSSL AES-256-CBC 적용
- 클라이언트-서버 암호화 통신 구현
* ### UDP
- sendto/recvfrom 기반 통신 구현
- TCP와의 차이 이해 및 실습

* **빌드 자동화**: **CMake**를 활용하여 복잡한 라이브러리 의존성(pthread, crypto)을 체계적으로 관리하고 빌드 프로세스를 자동화했습니다.

### 3. U-Boot 분석
- Yocto 설정을 통해 U-Boot 기반 부팅 환경 구성
- `printenv`를 통해 bootargs, bootcmd 등 주요 환경 변수 분석
- Bootloader → Kernel → RootFS 부팅 흐름 이해
- Raspberry Pi 기본 부팅 구조(config.txt)와의 차이 학습

[2026.05]
### 4. 캐릭터 드라이버 넣기
- hello World 드라이버와 문자열 입력, 출력 드라이버를 추가
- 드라이버 빌드 및 드라이버 동작 학습

[2026.06]
### 5. Qt Creator를 이용한 상태 프로그램 
- udp로 CPU, Memory, 가동시간을 받는 프로그램 추가 (1.0)
---

## 📂 프로젝트 구조

```text
├── doc/                        # 개발 설계 문서 및 시스템 분석 로그
├── meta-custom-a53/            # Cortex-A53 타겟 보드 구동을 위한 Yocto Custom BSP 레이어
└── program/                    # 임베디드 시스템 소프트웨어 영역
	├── udpstatus/              # [타겟 보드] C기반 가벼운 UDP 상태 브로드캐스팅 시스템 데몬
    ├── socket/              	# [타겟 보드] C기반 간단한 소캣프로그래밍
	|	└──  tcpThreadClient    # [타겟 보드] C기반 OpenSSL을 이용한 간단한 소캣 프로그래밍
    └── qt-gui-app/             # [Host PC] 타겟 보드 연동용 Qt6/C++ 기반 실시간 GUI 제어 콘솔
```
---

## 💡 이슈 경험
### 1. sysInit 네트워크 자동 연결 문제
* **Issue**:  sysInit 부팅 직후 `ifconfig` 시 IP가 잡히지 않는 현상 발생.
* **Analysis**: 커널의 네트워크 설정 시점보다 이더넷 칩셋의 준비 완료 시점이 늦어 발생하는 타이밍 이슈 확인.
* **Solution**: `rc.local`을 통해 10초 지연 후 네트워크 서비스를 재시작하도록 설정하여 안정적인 IP 할당 확보 가능 하지만 systemd로 교체시 증상 없음 sleep 10초 안해도 되어 systemd로 결정.

### 2. devtool modify 적용 문제
- **Issue**  
  devtool modify 후 수정 사항이 재적용되지 않는 문제 발생
- **Analysis**  
  기존 patch와 workspace 상태 불일치로 인해 변경 사항 반영 실패
- **Solution**  
  기존 patch를 git am 방식으로 재적용  
  → 변경 사항 정상 반영  
  → 기존 patch 백업 후 진행

### 3. Driver 적용 문제
- **3.1**
- **Issue**
  드라이버 Yocto에 bb추가후 빌드 시 에러 발생
- **Analysis**
  드라이버를 할때 기존 TEST할때는 all,clean만 했었으나 modules_install이 없어서 문제가 됨
- **Solution**
  Makefile에
  modules_install:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) modules_install INSTALL_MOD_PATH=$(DESTDIR)
  추가

- **3.2**
- **Issue**
  char드라이버에 cat /dev/chartest시 커널 경고(Warnning) 발생
- **Analysis**
  kmalloc을 사용안하고 고정배열로 100을 했을때  *off은 커널이 어디까지 읽었다 알려주는 역할인데 이게없이 하여 한꺼번에 다읽어 buffer overflow가 발생
- **Solution**
  read 함수 호출 시 전달되는 offset 변수를 활용하여 현재 읽기 위치를 추적하고, 읽어온 바이트 수만큼 offset을 갱신하도록 로직 수정
