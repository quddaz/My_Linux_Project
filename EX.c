#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define GPIO_BASE 0x3F200000
#define GPFSEL0 0x00
#define GPSET0 0x1C
#define GPCLR0 0x28

int Data_in = 4;
int CLK_Lat = 22;
int CLK_Sh = 23;

int main() {
    int fd = open("/dev/mem", O_RDWR|O_SYNC);
    if (fd < 0) {
        printf("Error: can't open /dev/mem \n");
        exit(-1);
    }
    
    char *gpio_mmap = (char *)mmap(0, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, GPIO_BASE);
    if (gpio_mmap == MAP_FAILED) {
        printf("Error in mmap \n");
        exit(-1);
    }

    volatile unsigned int* gpio = (volatile unsigned int*)gpio_mmap;
    
    // 설정된 LED 핀을 출력으로 설정
    gpio[GPFSEL0/4] |= (1<<12); //Data_in 4
    gpio[GPFSEL0/4] |= (1<<15); //CLK_Lat 22
    gpio[GPFSEL0/4] |= (1<<18); //CLK_Sh 23

    int arr[8] = {1,0,1,0,1,0,1,0};
    while(1){
        for (int i = 0; i < 9; i++) {
            if(arr[i] == 0)
                 gpio[GPSET0/4] |= (1<<Data_in);
            else
                gpio[GPCLR0/4] |= (1<<Data_in);
        gpio[GPSET0/4] |= (1<<CLK_Sh);
        sleep(0.5);
        gpio[GPCLR0/4] |= (1<<CLK_Sh);
        sleep(0.5);
        }
        gpio[GPSET0/4] |= (1<<CLK_Lat);
        sleep(0.5);
        gpio[GPCLR0/4] |= (1<<CLK_Lat);
        sleep(0.5);
    }

    munmap(gpio_mmap, 4096);
    return 0;
}
