# 🚀 Yocto Project: Raspberry Pi 3B+ Custom BSP & Secure Networking

본 프로젝트는 **Raspberry Pi 3B+ (Cortex-A53)** 환경에서 **Yocto Project (Scarthgap)**를 활용하여 커스텀 BSP를 구축하고, **OpenSSL** 기반의 보안 통신 어플리케이션을 통합 개발한 임베디드 리눅스 포트폴리오입니다.

---

## 🛠️ 핵심 요약

### 1. 커널 최적화 및 BSP 설계
* **커널 다이어트**: dmesg로 확인하여 불필요한 드라이버(BT, Sound 등)를 제거하여 커널 사이즈를 약 **3.2MB 감량**하고 부팅 속도를 9.7초 -> 7.5초 (2.2초)  개선했습니다.
* **시스템 매니저 전환**: 기존 **SysVinit** 방식의 순차적 부팅 한계를 극복하기 위해 **systemd**로 전환하여 병렬 부팅 및 시스템 관리 효율을 높였습니다.
* **네트워크 안정화**: 하드웨어 준비 지연으로 인한 네트워크 미연결 이슈(Race Condition)를 **지연 실행(Delayed Execution)** 스크립트로 해결했습니다.

### 2. 보안 네트워크 어플리케이션
* **독립적 개발 환경**: `populate_sdk`를 통해 툴체인을 추출하여 타겟 보드에 최적화된 **SDK 크로스 컴파일 환경**을 구축했습니다.
* **보안 통신 구현**: **OpenSSL(AES256-CBC, SHA256)** 라이브러리를 연동하여 데이터 암호화를 확인 후 TCPClientThread에 OpenSSL을 활용하여 소켓통신을 했습니다.
* **빌드 자동화**: **CMake**를 활용하여 복잡한 라이브러리 의존성(pthread, crypto)을 체계적으로 관리하고 빌드 프로세스를 자동화했습니다.

### 3. U-Boot 이해

---

## 📂 프로젝트 구조

* **`meta-custom-a53/`**: 커널 최적화 및 systemd 설정이 담긴 커스텀 요트 레이어
* **`programs/`**: CMake 기반의 보안 TCP/UDP 소켓 통신 소스 코드
* **`docs/`**: WSL 구축부터 네트워크 명령어 실습까지의 기술 리포트 모음

---

## 💡 이슈 경험
### 1
* **Issue**:  sysInit 부팅 직후 `ifconfig` 시 IP가 잡히지 않는 현상 발생.
* **Analysis**: 커널의 네트워크 설정 시점보다 이더넷 칩셋의 준비 완료 시점이 늦어 발생하는 타이밍 이슈 확인.
* **Solution**: `rc.local`을 통해 10초 지연 후 네트워크 서비스를 재시작하도록 설정하여 안정적인 IP 할당 확보 systemd로 교체시 증상 없음 .
### 2
* **Issue**:  devtool modify로 적용 후 다시 modify할 시 바뀐 내용들이 적용이 안됨
* **Analysis**: 
* **Solution**: /meta-custom-a53/receipe-core/linux/linux-raspberrypi/ 의 patch들을 git am으로 하여 역으로 적용 시킴. 그러나 기존의 패치들이 없어짐 백업 후 진행 함.
