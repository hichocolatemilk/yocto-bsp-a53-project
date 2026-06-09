#include "udp.h"



void cpuStatus(char *result_buf)
{
    FILE *fp;
    char buf[256];

    static unsigned long long prev_total = 0;
    static unsigned long long prev_idle = 0;

    // /proc/stat에서 읽어올 현재 틱 변수들
    unsigned long long user, nice, system, idle, iowait, irq, softirq;

    fp = fopen("/proc/stat", "r");
    if (fp == NULL) {
        return;
    }

    if (fgets(buf, sizeof(buf), fp) == NULL) {
        fclose(fp);
        sprintf(result_buf, "0.0");
        return;
    }
    fclose(fp);

    // 컴파일러 경고를 없애기 위해 sscanf가 7개 항목을 잘 읽었는지(== 7) 검사합니다.
    if (sscanf(buf, "cpu %llu %llu %llu %llu %llu %llu %llu", 
                &user, &nice, &system, &idle, &iowait, &irq, &softirq) != 7) {
        sprintf(result_buf, "0.0");
        return;
    }

    // 현재 시점의 총합(Total)과 쉰 합(Idle) 계산
    unsigned long long current_total = user + nice + system + idle + iowait + irq + softirq;
    unsigned long long current_idle = idle + iowait; // iowait도 노는 시간에 포함합니다.

    // [예외 처리] 제일 처음 함수가 실행됐을 때는 '1초 전 과거 값'이 없으므로 계산 불가
    if (prev_total == 0) {
        prev_total = current_total;
        prev_idle = current_idle;
        sprintf(result_buf, "0.0"); // 첫 초에는 0.0%로 넘김
        return;
    }

    // 1초 전 누적치와 현재 누적치의 '차이(Delta)' 구하기
    unsigned long long diff_total = current_total - prev_total;
    unsigned long long diff_idle = current_idle - prev_idle;

    // 백분율 계산
    double cpu_usage = (1.0 - ((double)diff_idle / diff_total)) * 100.0;

    //다음 루프(1초 뒤)를 위해 현재 값을 과거 저장소에 백업
    prev_total = current_total;
    prev_idle = current_idle;
    
    sprintf(result_buf, "CPU=%.1f", cpu_usage);
}

void memoryStatus(char *result_buf)
{
    FILE *fp;
    int total = 0, available = 0, percent = 0;
    char buf[100];

    fp = fopen("/proc/meminfo", "r");
    if(fp == NULL) return;

    while(fgets(buf, sizeof(buf), fp) != NULL)
    {
        if(strncmp(buf, "MemTotal:", 9) == 0)
        {
            sscanf(buf, "MemTotal: %d", &total);
        }
        else if(strncmp(buf, "MemAvailable:", 13) == 0)
        {
            sscanf(buf, "MemAvailable: %d", &available);
            break;
        }
    }
    percent = ((total - available) * 100) / total;
    sprintf(result_buf, "MEM=%d", percent);
    fclose(fp);
}


void uptime(char *result_buf)
{
    FILE *fp;
    double time;
    fp = fopen("/proc/uptime", "r");
    if (fp == NULL) return;
    if(fscanf(fp, "%lf", &time)!= 1)
    {
        fclose(fp);
        return;
    }
    int total_second = (int)time;
    int h = total_second /3600;
    int m = total_second / 3600 / 60;
    int s = total_second % 60;
    sprintf(result_buf, "UPTIME=%02d:%02d:%02d", h, m, s);
    fclose(fp);
}


int main()
{

    int sockfd;
    char cpu_data[100];
    char mem_data[100];
    char uptime_data[30];
    char total_data[300];
    struct sockaddr_in server_addr, client_addr;
    socklen_t server_addr_len = sizeof(server_addr);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&server_addr, 0, server_addr_len);
    memset(total_data, 0, sizeof(total_data));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("192.168.35.125");
    server_addr.sin_port = htons(12345);

    while(1){
        cpuStatus(cpu_data);
        memoryStatus(mem_data);
        uptime(uptime_data);
        sprintf(total_data, "%s;%s;%s", cpu_data, mem_data, uptime_data);
        sendto(sockfd, total_data , sizeof(total_data), 0, (struct sockaddr *)&server_addr, server_addr_len);
        sleep(1);
    }
    return 0;
}