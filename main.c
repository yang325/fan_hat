#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "lib/Config/DEV_Config.h"
#include "lib/PWM/PCA9685.h"
#include "lib/Device/OLED.h"
#include "lib/GUI/GUI_Paint.h"

#define MAXLINE 1024
int Get_ip(char *buf)
{
    char result_buf[MAXLINE];
    char command[MAXLINE];
    int rc = 0;
    FILE *fp;

    snprintf(command, sizeof(command), "ifconfig -a|grep inet|grep -v 127.0.0.1|grep -v inet6|awk '{print $2}'|tr -d \"addr:\"");

    fp = popen(command, "r");
    if(NULL == fp) {
        perror("popen failed!");
        exit(1);
    }

    while(fgets(result_buf, sizeof(result_buf), fp) != NULL) {
        if('\n' == result_buf[strlen(result_buf)-1]) {
            result_buf[strlen(result_buf)-1] = '\0';
        }
        // printf("%s\r\n", result_buf);
    }

    rc = pclose(fp);
    if(-1 == rc) {
        perror("close failed");
        exit(1);
    }

    strcpy(buf, result_buf);
    // printf("buf = %s\r\n", buf);
    return rc;
}

#define TEMP_PATH "/sys/class/thermal/thermal_zone0/temp"
#define MAX_SIZE 32
static double Get_CPU_Temp(void)
{
    int fd;
    double temp = 0;
    char buf[MAX_SIZE];

    // open /sys/class/thermal/thermal_zone0/temp
    fd = open(TEMP_PATH, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "failed to open thermal_zone0/temp\n");
        return -1;
    }

    // read value
    if (read(fd, buf, MAX_SIZE) < 0) {
        fprintf(stderr, "failed to read temp\n");
        return -1;
    }

    temp = atoi(buf) / 1000.0;
    close(fd);
    return temp;
}

void Handler(int signo)
{
    //System Exit
    printf("\r\nHandler:exit\r\n");
    // DEV_ModuleExit();
    PCA9685_setPWM(0, 0);

    exit(0);
}

int main(void)
{
    printf("fan hat\r\n");

    // Exception handling:ctrl + c
    signal(SIGINT, Handler);
    signal(SIGTERM, Handler);
    DEV_ModuleInit();
    DEV_I2C_Init();

    PCA9685_Init(1000);
    PCA9685_setPWM(0, 0);
    int state = 0;

    printf("oled init\r\n");
    OLED_Init();
    OLED_Clear();

    //Create a new image cache
    UBYTE *Image;
    UWORD Imagesize = SSD1306_WIDTH * SSD1306_HEIGHT;
    printf("image cache need size = %d byte\r\n", Imagesize);
    if((Image = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    Paint_NewImage(Image, SSD1306_WIDTH, SSD1306_HEIGHT, 0, BLACK);
    Paint_SetRotate(ROTATE_180);
    Paint_Clear(BLACK);

    //get ip
    char IP_buf[20];// xxx.xxx.xxx.xxx
    int IP_buf_len;

    //get temp
    double temp = 0;
    char str[5];

    Paint_DrawString_EN(1, 15, IP_buf, &Font12, BLACK, WHITE);
    OLED_Display(Image);
    while(1){
        Paint_Clear(BLACK);

        Get_ip(IP_buf);
        IP_buf_len = strlen(IP_buf);
        for (int i = 0; i < IP_buf_len; ++i) {
            if (isdigit(IP_buf[i]) || IP_buf[i] == '.') {
                continue;
            }
            strcpy(IP_buf, "Loading...");
            break;
        }
        Paint_DrawString_EN(0, 0, "IP:", &Font12, BLACK, WHITE);
        Paint_DrawString_EN(25, 0, IP_buf, &Font12, BLACK, WHITE);

        temp = Get_CPU_Temp();
        sprintf(str, "%.2f", temp);
        Paint_DrawString_EN(0, 15, "Temp:", &Font12, BLACK, WHITE);
        Paint_DrawString_EN(36, 15, str, &Font12, BLACK, WHITE);
        Paint_DrawString_EN(75, 15, "o", &Font8, BLACK, WHITE);
        Paint_DrawString_EN(80, 15, "C", &Font12, BLACK, WHITE);

        OLED_Display(Image);
        if((state == 0) && (temp > 55)){
            PCA9685_setPWM(0, 100);
            state = 1;
        }else if((state != 0) && (temp < 45)){
            PCA9685_setPWM(0, 0);
            state = 0;
        }
        DEV_Delay_ms(1000);
    }

    printf("stop\r\n");

    //System Exit
    DEV_ModuleExit();
    return 0;
}
