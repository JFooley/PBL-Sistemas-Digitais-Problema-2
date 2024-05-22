#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define SET_BACKGROUND _IOW('a', 'a', int32_t*)

int main()
{
    int fd;
    int32_t value[3];
    
    printf("Setting background color\n");
    
    fd = open("/dev/set_background", O_RDWR);
    if (fd < 0) {
        printf("Cannot open device file\n");
        return 0;
    }
    
    printf("Enter Red value: ");
    scanf("%d", &value[0]);
    printf("Enter Green value: ");
    scanf("%d", &value[1]);
    printf("Enter Blue value: ");
    scanf("%d", &value[2]);
    
    ioctl(fd, SET_BACKGROUND, (int32_t*) &value);
    close(fd);
    
    return 0;
}
