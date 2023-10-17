#include <stdio.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include "asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"
//#include "creator_pxa270_lcd.h"
#define DISPLAYWIDTH 16  // lcd width
#define DISPLAYLENGTH 16 // lcd length

struct sock_data 
{
    char map_p[DISPLAYWIDTH][DISPLAYLENGTH];
    char buf[64];
    char time[16];
};
void led_func(int fd, unsigned short retval);
void seven_seg_func(int fd, int retval);
void lcd_func(const char *exp, int fd);
void wait_for_another_player(int fd,char *time);
// unsigned short calculate(char *s);
char *get_input_and_display_lcd(int fd);
void set_lcd(int fd, char *ans_str, lcd_write_info_t *display);
int all_init();
void show_item(char map, int x, int y, unsigned short *data);
void player_plot(unsigned short *data, int row, int col);
void wait_for_choose(int fd);
