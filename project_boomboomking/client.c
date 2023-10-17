#include "boomboomking.h"
#include "sockop.h"
#include <pthread.h>

int dev_fd, connfd;


void *control()
{

    unsigned short key;
    int ret;
    char moving;
    moving = '\0';
    // memset(&moving, '\0', 1);
    ioctl(dev_fd, KEY_IOCTL_CLEAR);

    while (1)
    {
        ret = ioctl(dev_fd, KEY_IOCTL_WAIT_CHAR, &key);
        // printf("key %c %d\n", key, ret);

        if (!ret)
        {
            if ((key & 0xff) == '2')
            {

                moving = 'w';
                // printf("key %c\n", moving);
                send(connfd, &moving, 1, 0);
            }
            else if ((key & 0xff) == '5')
            {
                moving = 's';
                send(connfd, &moving, 1, 0);
            }
            else if ((key & 0xff) == '4')
            {
                moving = 'a';
                send(connfd, &moving, 1, 0);
            }
            else if ((key & 0xff) == '6')
            {
                moving = 'd';
                send(connfd, &moving, 1, 0);
            }
            else if ((key & 0xff) == '*') // C
            {
                moving = 'c';
                // printf("key %c\n", moving);
                send(connfd, &moving, 1, 0);
            }
            else if ((key & 0xff) == 'D') // D
            {
                moving = 't';
                send(connfd, &moving, 1, 0);
            }
            else if ((key & 0xff) == '0') // D
            {
                moving = ' ';
                send(connfd, &moving, 1, 0);
            }
            else
                continue;
        }
    }
}

int main(int argc, char *argv[])
{
    // char map[16][256];

    struct sock_data sock_d; // our data struct for msg
    unsigned short data[0x800] = {0};
    int i, j;
    char win_buf[16];
    int team;
    // int dev_fd, connfd;
    lcd_write_info_t display;

    // connect server section
    connfd = connectsock(argv[1], argv[2], "tcp");
    printf("csock\n %d", __LINE__);
    // init section
    dev_fd = all_init();
    printf("init\n %d", __LINE__);
    // wating another player
    // to do because need to wait server

    pthread_t thread_id;
    while (1)
    {
        // printf("start recv csock\n %d",__LINE__);
        recv(connfd, &sock_d, sizeof(struct sock_data), 0);
        if (strcmp(sock_d.buf, "choose") == 0)
        {
            printf("choose %d \n", __LINE__);
            wait_for_choose(dev_fd);
            fflush(stdout);
            unsigned short key;
            int ret;
            char numb;
            ioctl(dev_fd, KEY_IOCTL_CLEAR);
            ret = ioctl(dev_fd, KEY_IOCTL_WAIT_CHAR, &key);
            if ((key & 0xff) == '2')
            {
                numb = '2';
                send(connfd, &numb, 1, 0);
            }
            else if ((key & 0xff) == '3')
            {
                numb = '3';
                send(connfd, &numb, 1, 0);
            }
            else if ((key & 0xff) == '4')
            {
                numb = '4';
                send(connfd, &numb, 1, 0);
            }
            else if ((key & 0xff) == '6')
            {
                numb = '6';
                send(connfd, &numb, 1, 0);
            }
            else if ((key & 0xff) == '7')
            {
                numb = '7';
                send(connfd, &numb, 1, 0);
            }
            else if ((key & 0xff) == '8')
            {
                numb = '8';
                send(connfd, &numb, 1, 0);
            }
            continue;
        }
        
        if (strcmp(sock_d.buf, "wait") == 0)
        {
            printf("waitting\n %d", __LINE__);
            fflush(stdout);
            char nowtime[16];
            strcpy(nowtime, sock_d.time);
            wait_for_another_player(dev_fd, nowtime);
            printf("waitting\n %d", __LINE__);
            continue;
        }
        else if (strcmp(sock_d.buf, "start") == 0)
        {

            // recv(connfd, &sock_d, sizeof(struct sock_data), 0);
            // printf("start LCD\n %d",__LINE__);
            for (i = 0; i < DISPLAYWIDTH; i++)
            {
                for (j = 0; j < DISPLAYLENGTH; j++)
                {
                    // printf("%c",sock_d.map_p[i][j]);
                    show_item(sock_d.map_p[i][j], i * 8, j, data);
                }
                // printf("\n");
            }
            // printf("show_item \n %d",__LINE__);
            lcd_full_image_info_t *draw_lcd = (lcd_full_image_info_t *)malloc(sizeof(lcd_full_image_info_t));

            memcpy((*draw_lcd).data, data, 0x800 * sizeof(short));

            ioctl(dev_fd, LCD_IOCTL_DRAW_FULL_IMAGE, draw_lcd);

            char str_counttime[16];
            int counttime;
            strcpy(str_counttime, sock_d.time);
            counttime = atoi(str_counttime);
            seven_seg_func(dev_fd, counttime);

            pthread_create(&thread_id, NULL, control, NULL);
            pthread_detach(thread_id);

            while (1)
            {
                recv(connfd, &sock_d, sizeof(struct sock_data), 0);
                // if (strcmp(sock_d.buf, "gameover") == 0)
                // {
                //     // show scoreboard
                //     recv(connfd, &sock_d, sizeof(struct sock_data), 0);
                //printf("%s\n",sock_d.buf);
                if (strstr(sock_d.buf, "win") != NULL)
                {
                    
                    ioctl(dev_fd, LCD_IOCTL_CLEAR, NULL);
                    sscanf(sock_d.buf, "%s %d", win_buf, &team);
                    printf("win end %s %d\n",sock_d.buf,__LINE__);

                    
                    for (j = 0; j < 2048; j++)
                    {
                        data[j] = 0;
                    }
                    memcpy((*draw_lcd).data, data, 0x800 * sizeof(short));
                    ioctl(dev_fd, LCD_IOCTL_DRAW_FULL_IMAGE, draw_lcd);

                    display.CursorX = 0;
                    display.CursorY = 3;
                    ioctl(dev_fd, LCD_IOCTL_CUR_SET, &display);
                    player_plot(data, 8, 2);
                    player_plot(data, 8, 7);
                    player_plot(data, 8, 12);
                    printf("plot done %d\n",__LINE__);
                    memcpy((*draw_lcd).data, data, 0x800 * sizeof(short));
                    ioctl(dev_fd, LCD_IOCTL_DRAW_FULL_IMAGE, draw_lcd);

                    display.CursorX = 0;
                    display.CursorY = 6;
                    ioctl(dev_fd, LCD_IOCTL_CUR_SET, &display);

                    display.Count = sprintf((char *) display.Msg,"***************\n");
                    ioctl(dev_fd, LCD_IOCTL_WRITE, &display);

                    display.Count = sprintf((char *) display.Msg," Player%d win !\n", team);
                    ioctl(dev_fd, LCD_IOCTL_WRITE, &display);

                    display.Count = sprintf((char *) display.Msg,"***************\n");
                    ioctl(dev_fd, LCD_IOCTL_WRITE, &display);

                    display.Count = sprintf((char *) display.Msg,"\n Play Again ?\n\nplayer0 press *\n");
                    ioctl(dev_fd, LCD_IOCTL_WRITE, &display);

                }
                
                if (strcmp(sock_d.buf, "closegame") == 0)
                {
                    for (j = 0; j < 2048; j++)
                    {
                        data[j] = 0;
                    }
                    memcpy((*draw_lcd).data, data, 0x800 * sizeof(short));
                    ioctl(dev_fd, LCD_IOCTL_DRAW_FULL_IMAGE, draw_lcd);

                    display.CursorX = 0;
                    display.CursorY = 4;
                    ioctl(dev_fd, LCD_IOCTL_CUR_SET, &display);

                    display.Count = sprintf((char *) display.Msg,"***************\n");
                    ioctl(dev_fd, LCD_IOCTL_WRITE, &display);

                    display.Count = sprintf((char *) display.Msg,"\n   Game End ! \n");
                    ioctl(dev_fd, LCD_IOCTL_WRITE, &display);

                    display.Count = sprintf((char *) display.Msg,"\n***************\n");
                    ioctl(dev_fd, LCD_IOCTL_WRITE, &display);

                    sleep(3);

                    for (j = 0; j < 2048; j++)
                    {
                        data[j] = 0;
                    }
                    memcpy((*draw_lcd).data, data, 0x800 * sizeof(short));
                    ioctl(dev_fd, LCD_IOCTL_DRAW_FULL_IMAGE, draw_lcd);
                    seven_seg_func(dev_fd, 0);
                    pthread_cancel(thread_id);
                    return 0;
                }
                if (strcmp(sock_d.buf, "continue") == 0)
                {
                    pthread_cancel(thread_id);
                    break;
                }
                if (strcmp(sock_d.buf, "timeout") == 0)
                {
                    for (j = 0; j < 2048; j++)
                    {
                        data[j] = 0;
                    }
                    memcpy((*draw_lcd).data, data, 0x800 * sizeof(short));
                    ioctl(dev_fd, LCD_IOCTL_DRAW_FULL_IMAGE, draw_lcd);

                    display.CursorX = 0;
                    display.CursorY = 3;
                    ioctl(dev_fd, LCD_IOCTL_CUR_SET, &display);

                    display.Count = sprintf((char *) display.Msg,"***************\n");
                    ioctl(dev_fd, LCD_IOCTL_WRITE, &display);

                    display.Count = sprintf((char *) display.Msg,"\n   Timeout ! \n");
                    ioctl(dev_fd, LCD_IOCTL_WRITE, &display);

                    display.Count = sprintf((char *) display.Msg,"\n***************\n");
                    ioctl(dev_fd, LCD_IOCTL_WRITE, &display);

                    display.Count = sprintf((char *) display.Msg,"\n Play Again ?\n\nplayer0 press *\n");
                    ioctl(dev_fd, LCD_IOCTL_WRITE, &display);

                    seven_seg_func(dev_fd, 0);
                    continue;
                }
                //}

                // printf("start LCD %d\n",__LINE__);
                if (strcmp(sock_d.buf, "start") == 0){
                    for (i = 0; i < DISPLAYWIDTH; i++)
                    {
                        for (j = 0; j < DISPLAYLENGTH; j++)
                        {
                            // printf("%c",sock_d.map_p[i][j]);
                            show_item(sock_d.map_p[i][j], i * 8, j, data);
                        }
                        // printf("\n");
                    }

                    memcpy((*draw_lcd).data, data, 0x800 * sizeof(short));

                    ioctl(dev_fd, LCD_IOCTL_DRAW_FULL_IMAGE, draw_lcd);
                    strcpy(str_counttime, sock_d.time);
                    counttime = atoi(str_counttime);
                    seven_seg_func(dev_fd, counttime);
                }
            }
        }
        else
            continue;
    }
    close(dev_fd);
    close(connfd);
    return 0;
}
