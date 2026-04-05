# Init 시스템 전환 및 네트워크 설정 (systemd 기반)

## 1. 목표
- 기존 SysVinit(init.d) 방식에서 systemd 기반으로 전환하고,  
- Yocto 환경에서 정적 IP 네트워크 설정을 적용

---

## 2. 수행내용
### 1) SysVinit vs systemd (핵심 차이)

| 항목 | SysVinit | systemd  |
|------|----------|----------|
| 실행 | 순차 실행 | 병렬 실행 |
| 설정 | 쉘스크립트| unit 파일 |
| 부팅 속도 | 느림 | 빠름      |

👉 systemd는 병렬 실행 및 의존성 관리로 부팅 속도와 안정성이 향상됨

### 2) systemd 선택 이유
- 부팅 속도 개선 (병렬 실행)
- 서비스 자동 재시작 기능 지원
- Yocto 최신 환경과의 호환성
- sysinit으로 할 시 rc.local에서 10초 슬립을 해야 붙는경우 생김
- SysVinit 환경에서 rc.local의 역할과 실행 순서(S99), 그리고 하드웨어와 소프트웨어 간의 타이밍 이슈 이해함.

### 3) systemd 서비스 구성

#### 서비스 파일 예시
```bash
[Match]
Name=eth0

[Network]
Address=192.168.xxx.xxx/24
Gateway=192.168.xxx.xxx
```
