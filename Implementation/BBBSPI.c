//Header files

#include <stdio.h>                  // printf statements
#include <stdlib.h>                 // exit() statement
#include <stdint.h>                 // uint8_t 
#include <unistd.h>                 // standard symbolic constants and types 
#include <sys/ioctl.h>              // ioctl function call
#include <linux/spi/spidev.h>       // All the functions -- spi read,spi write
#include <linux/types.h>
#include <fcntl.h>
#include <getopt.h>

#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a)[0])

static void pabort(const char *s)
{
    perror(s);
    abort();
}

static const char *device = "/dev/spidev0.1";
static uint8_t mode;
static uint8_t bits = 8;
static uint32_t speed=500000;
static uint16_t delay;


static void Transmitter(int fd)
{
    int ret;
    uint8_t Tx[]= {0x01,};

    struct spi_ioc_transfer tr ={
        .tx_buf = (unsigned long)Tx,
        .len = ARRAY_SIZE(Tx),
        .delay_usecs = delay,
        .speed_hz = speed,
        .bits_per_word = bits,
    };

    ret = ioctl(fd,SPI_IOC_MESSAGE(1),&tr);
    if(ret<1)
    {
        pabort("Cannot send the spi message");
    }
    for(ret=0;ret<ARRAY_SIZE(Tx);ret++)
    {
        if(!(ret % 6))
            puts("");
        printf("%.2X",Tx[ret]);
    }
    puts("");
}


int main(int argc, char *argv[])
{
    int ret=0,fd;
    //Opening the device file for reading and writing
    fd =open(device,O_RDWR);
    if(fd<0)
    {
        pabort("Cannot open the device");
    }

    //Setting write mode --SPI
    ret=ioctl(fd,SPI_IOC_WR_MODE,&mode);
    if(ret==-1)
        pabort("Cannot set SPI Mode");

    //Setting the number of bits per word
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if(ret==-1)
        pabort("Cannot set bits per word");
        
    //Setting the speed
    ret = ioctl(fd,SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if(ret==-1)
        pabort("Cannot set max speed");
        
    printf("SPI Mode: %d\n",mode);
    printf("Bits per word: %d\n",bits);
    printf("Max speed: %d\n KHz",speed/1000);
    printf("Delay: %d\n",delay);

    //Calling the transmitter function which is responsible for sending data to slave
    Transmitter(fd);
    
    close(fd);

    return ret;
}




