#include <fcntl.h>  
#include <errno.h>  
#include <sys/ioctl.h>  
#include <stdio.h>  

/***
GSENSOR_IOCTL_START  -->  打开循环上报功能
GSENSOR_IOCTL_CLOSE  -->  close
GSENSOR_IOCTL_GETDATA  --->  获取上报值 x,y,z
***/
#define FILE_PATH "/dev/mma8452_daemon"  
#define GSENSOR_IOCTL_MAGIC         'a'
#define GBUFF_SIZE              12  /* Rx buffer size */
#define GSENSOR_IOCTL_START             _IO(GSENSOR_IOCTL_MAGIC, 0x03)
#define GSENSOR_IOCTL_CLOSE             _IO(GSENSOR_IOCTL_MAGIC, 0x02)
#define GSENSOR_IOCTL_GETDATA _IOR(GSENSOR_IOCTL_MAGIC, 0x08,char[GBUFF_SIZE+1])

struct sensor_axis {
    int x;
    int y;
    int z;
}; 

int main()
{
    int ret;
    struct sensor_axis acc = {0, 0, 0};

    int fd = open(FILE_PATH, O_RDWR);  
    if ( -1 == fd){  
        printf("open led_flash node failed!\n");  
        return -1;  
    }  

    ret = ioctl(fd, GSENSOR_IOCTL_START);
    sleep(1);
    ret = ioctl(fd, GSENSOR_IOCTL_GETDATA, &acc);
    printf("x = %d, y = %d, z = %d.\n", acc.x, acc.y, acc.z);
    sleep(1);
    ret = ioctl(fd, GSENSOR_IOCTL_CLOSE);

    return 0;
}
