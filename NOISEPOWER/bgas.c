//
//  How to access GPIO registers from C-code on the Raspberry-Pi
//  Example program
//  15-January-2012
//  Dom and Gert
//  Played with by Paschal noisepower 30 Oct 2012
//  29th Nov 2012 modified by out as a silly string the ON command
//  will be compiled to an exe called ON 
//  Modified tng@chegwin.org 30-Jul-2013
//  Sniffed the remote codes and replicated inside the C.
//  Modified tng@chegwin.org 26-Sep-2013
//  Boiler on/off for Drayton/British Gas boilers
//  Each boiler is individual and needs correctly sniffing


// Access from ARM Running Linux

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

int  mem_fd;
char *gpio_mem, *gpio_map;
char *spi0_mem, *spi0_map;
int sampletime;
int standard_unit;

#define STANDARD_UNIT 22 //Units are 23/46 sampletimes long
#define SAMPLE_TIME 20 //22.67 microseconds
#define UNIT_INTERVAL (STANDARD_UNIT*SAMPLE_TIME)

// I/O access
volatile unsigned *gpio;


// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |=  (1<<(((g)%10)*3))
#define SET_GPIO_ALT(g,a) *(gpio+(((g)/10))) |= (((a)<=3?(a)+4:(a)==4?3:2)<<(((g)%10)*3))

#define GPIO_SET *(gpio+7)  // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR *(gpio+10) // clears bits which are 1 ignores bits which are 0

void setup_io();
void short_on();
void short_off();
void long_on();
void long_off();

int main(int argc, char *argv[])
{ 
  int g,rep;
  int plug, set;
  int receiver;
  char *on_or_off = NULL;
 
  // Set up gpi pointer for direct register access
  setup_io();

  on_or_off = argv[1];  

if (!(on_or_off))
   {
	printf("Usage %s on|off \n",argv[0]);
        exit(2);	
   }


//test suitable
 if (((strcmp(on_or_off,"on")==0) || (strcmp(on_or_off,"off")==0)))
  {
//    printf("We have requested switching %s  ",on_or_off); 
  }
 else
  {
	printf("Usage %s on|off \n",argv[0]);
     exit(2);
  } 

  // Switch GPIO 7..11 to output mode

 /************************************************************************\
  * You are about to change the GPIO settings of your computer.          *
  * Mess this up and it will stop working!                               *
  * It might be a good idea to 'sync' before running this program        *
  * so at least you still have your code changes written to the SD-card! *
 \************************************************************************/

  // Set GPIO pin 18 to output
      INP_GPIO(18);
      OUT_GPIO(18);
 

  for (rep=0; rep<3; rep++)
  //recall INVERSION out a 1 to ge a zero!
  {
//	printf("Hello");
        g=18;
        GPIO_SET = 1<<g;
        bcm2835_delay(10);
//OFF
//1 1 1 1 1 2  1  2 1  1 1 1 1 1 1 2  1 1 1 1 1 1 1 1 2  1 1 2 1 1
//Received 376 bytes  Taken from https://github.com/jimstudt/ook-decoder
//0000207.445396s ###  30 pulses
//num high   low      freq
//  1  464uS    464uS  -10.776kHz
//  2  564uS    456uS  -13.741kHz
//  3  584uS    444uS  -17.123kHz
//  4  552uS    464uS  -13.134kHz
//  5  560uS    464uS  -12.500kHz
//  6 1080uS    968uS  -13.194kHz
//  7  560uS    972uS  -10.268kHz
//  8 1076uS    460uS  -11.385kHz
//  9  564uS    964uS  -14.184kHz
// 10  564uS    484uS  -10.638kHz
// 11  536uS    468uS  -11.194kHz
// 12  552uS    464uS  -11.775kHz
// 13  568uS    456uS  -13.204kHz
// 14  560uS    468uS  -12.054kHz
// 15  552uS    468uS  -11.775kHz
// 16 1076uS    968uS  -13.708kHz
// 17  560uS    464uS   -6.696kHz
// 18  556uS    464uS  -15.288kHz
// 19  564uS    460uS  -17.287kHz
// 20  560uS    464uS  -10.714kHz
// 21  568uS    456uS  -10.563kHz
// 22  560uS    460uS  -13.839kHz
// 23  560uS    464uS  -13.839kHz
// 24  560uS    460uS  -11.607kHz
// 25 1084uS    964uS  -12.223kHz
// 26  556uS    464uS   -7.644kHz
// 27  564uS    472uS  -13.741kHz
// 28 1060uS    464uS  -12.736kHz
// 29  576uS    448uS  -15.191kHz
// 30  568uS   8004uS  -11.444kHz
//ON
//1 1 1 1 1 2  1  2 1  1 1 1 1 1 1 2  1 1 1 2  1 1 1 1 2  1 2 1 1
	short_on();
	short_off();
	short_on();
	short_off();
	short_on();
	short_off();
	short_on();
	short_off();
	short_on();
	short_off();
	long_on();
	long_off();
	short_on();
	long_off();
	long_on();
	short_off();
	short_on();
	long_off();
	short_on();
	short_off();
	short_on();
	short_off();
	short_on();
	short_off();
	short_on();
	short_off();
	short_on();
	short_off();
	short_on();
	short_off();
	long_on();
	long_off();
	short_on();
	short_off();
	short_on();
	short_off();
	short_on();
	short_off();
// End of common code
  //above is gap between burst
	if (strcmp(on_or_off,"off")==0){	
//OFF
//1 1 1 1 1 2  1 1 2 1 1
		short_on();
		short_off();
		short_on();
		short_off();
		short_on();
		short_off();
		short_on();
		short_off();
		short_on();
		short_off();
		long_on();
		long_off();
		short_on();
		short_off();
		short_on();
		short_off();
		long_on();
		short_off();
		short_on();
		short_off();
		short_on();
		short_off();
	} else if (strcmp(on_or_off,"on")==0) {
//ON
//2  1 1 1 1 2  1 2 1 1
		long_on();
		long_off();
		short_on();
		short_off();
		short_on();
		short_off();
		short_on();
		short_off();
		short_on();
		short_off();
		long_on();
		long_off();
		short_on();
		short_off();
		long_on();
		short_off();
		short_on();
		short_off();
		short_on();
		short_off();
	} else {
		long_on();
		long_off();
	}
	
        usleep(500000);
  }

  return 0;

} // main


//
// Set up a memory regions to access GPIO
//
void setup_io()
{
   /* open /dev/mem */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("can't open /dev/mem \n");
      exit (-1);
   }

   /* mmap GPIO */

   // Allocate MAP block
   if ((gpio_mem = malloc(BLOCK_SIZE + (PAGE_SIZE-1))) == NULL) {
      printf("allocation error \n");
      exit (-1);
   }

   // Make sure pointer is on 4K boundary
   if ((unsigned long)gpio_mem % PAGE_SIZE)
     gpio_mem += PAGE_SIZE - ((unsigned long)gpio_mem % PAGE_SIZE);

   // Now map it
   gpio_map = (unsigned char *)mmap(
      (caddr_t)gpio_mem,
      BLOCK_SIZE,
      PROT_READ|PROT_WRITE,
      MAP_SHARED|MAP_FIXED,
      mem_fd,
      GPIO_BASE
   );

   if ((long)gpio_map < 0) {
      printf("mmap error %d\n", (int)gpio_map);
      exit (-1);
   }

   // Always use volatile pointer!
   gpio = (volatile unsigned *)gpio_map;


} // setup_io

void short_on ()
{
	int g;
        g=18;
        GPIO_CLR = 1<<g;	
        usleep(UNIT_INTERVAL);
//	printf("1");
}

void short_off ()
{
	int g;
        g=18;
        GPIO_SET = 1<<g;
        usleep(UNIT_INTERVAL);
//	printf(" ");
}
void long_on ()
{
	int g;
        g=18;
        GPIO_CLR = 1<<g;
        usleep(2*UNIT_INTERVAL);
//	printf("2");

}
void long_off ()
{
	int g;
        g=18;
        GPIO_SET = 1<<g;
        usleep(2*UNIT_INTERVAL);
//	printf("  ");

}
