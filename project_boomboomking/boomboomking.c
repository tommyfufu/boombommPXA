#include <boomboomking.h>

int all_init()
{
    int fd;
    unsigned short data;
    if ((fd = open("/dev/lcd", O_RDWR)) < 0)
    {
        printf("Open /dev/lcd faild.\n");
        return -1;
    }
    ioctl(fd, LCD_IOCTL_CLEAR, NULL);
    ioctl(fd, _7SEG_IOCTL_ON, NULL);

    data = LED_ALL_ON;
    ioctl(fd, LED_IOCTL_SET, &data);
    ioctl(fd, _7SEG_IOCTL_ON, NULL);
    sleep(1);
    data = LED_ALL_OFF;
    ioctl(fd, LED_IOCTL_SET, &data);
    ioctl(fd, _7SEG_IOCTL_OFF, NULL);
    return fd;
}
// time_t get_sys_time()
// {
//     time_t t = time(0);
// 	struct tm tm1, tm2;
// 	char buf[256];

// 	gmtime_r(&t, &tm1);
// 	localtime_r(&t, &tm2);

// 	char buf1[256], buf2[256], nowtime[10], day[5], month[5], date[5], year[5];

// 	strcpy(buf1, asctime(&tm2));
// 	sscanf(buf1, "%s %s %s %s %s", day, month, date, nowtime, year);
// 	printf("nowtime:%s\n", nowtime);
// 	printf("hour: %c%c", nowtime[0], nowtime[1]);
    
// 	return 0;

// }
void wait_for_another_player(int fd, char* time)
{

    lcd_write_info_t display;
    char waitbuf[30];
    strcpy(waitbuf, "Wait for another player\n");
    display.Count = sprintf((char *)display.Msg, "Wait for another player");
    ioctl(fd, LCD_IOCTL_CLEAR, NULL);
    display.CursorX = 0;
    display.CursorY = 6;
    ioctl(fd, LCD_IOCTL_CUR_SET, &display);

    display.Count = sprintf((char *) display.Msg,"***************\n");
    ioctl(fd, LCD_IOCTL_WRITE, &display);

    display.Count = sprintf((char *) display.Msg," Wait for \n ");
    ioctl(fd, LCD_IOCTL_WRITE, &display);
    display.Count = sprintf((char *) display.Msg," others player\n");
    ioctl(fd, LCD_IOCTL_WRITE, &display);
    display.Count = sprintf((char *) display.Msg,"***************\n");
    ioctl(fd, LCD_IOCTL_WRITE, &display);

    _7seg_info_t sdata;
    ioctl(fd, _7SEG_IOCTL_ON, NULL);
    char hour[3], min[3];
    sscanf(time, "%s %s", hour, min);
    int hourd, mind;
    hourd=atoi(hour);
    mind=atoi(min);
    printf("hour: %d, mind: %d\n", hourd, mind);
    sdata.Mode = _7SEG_MODE_HEX_VALUE;
    sdata.Which = _7SEG_ALL;
    sdata.Value = 0x0000;
    int i;
    for (i = 0; i < 2; i++)
    {
        sdata.Value = sdata.Value | (mind % 10) << (i * 4);
        mind /= 10;
    }
    for (i = 2; i < 4; i++)
    {
        sdata.Value = sdata.Value | (hourd % 10) << (i * 4);
        
        hourd /= 10;
    
    }
    
    
    ioctl(fd, _7SEG_IOCTL_SET, &sdata);
    unsigned short data;
    for (data = 1; data < 9; data++)
    {
        ioctl(fd, LED_IOCTL_SET, 0x00);
        ioctl(fd, LED_IOCTL_BIT_SET, &data);
    }
    

}
void wait_for_choose(int fd)
{

    lcd_write_info_t display;
    char waitbuf[30];
    strcpy(waitbuf, "Choose how many players 2~4\n");
    display.Count = sprintf((char *)display.Msg, "Choose how many players 2~4");
    ioctl(fd, LCD_IOCTL_CLEAR, NULL);
    display.CursorX = 0;
    display.CursorY = 2;
    ioctl(fd, LCD_IOCTL_CUR_SET, &display);

    display.Count = sprintf((char *) display.Msg,"***************\n");
    ioctl(fd, LCD_IOCTL_WRITE, &display);

    display.Count = sprintf((char *) display.Msg," Choose how \n");
    ioctl(fd, LCD_IOCTL_WRITE, &display);
    display.Count = sprintf((char *) display.Msg," many players \n 2~4 \n");
    ioctl(fd, LCD_IOCTL_WRITE, &display);

    display.Count = sprintf((char *) display.Msg,"***************\n");
    ioctl(fd, LCD_IOCTL_WRITE, &display);
    display.Count = sprintf((char *) display.Msg," Press 2~4 :\n map1 \n");
    ioctl(fd, LCD_IOCTL_WRITE, &display);
    display.Count = sprintf((char *) display.Msg," Press 6~8 :\n map2 \n");
    ioctl(fd, LCD_IOCTL_WRITE, &display);
    _7seg_info_t sdata;
    ioctl(fd, _7SEG_IOCTL_ON, NULL);
    sdata.Mode = _7SEG_MODE_HEX_VALUE;
    sdata.Which = _7SEG_ALL;
    sdata.Value = 0x0000;
    ioctl(fd, _7SEG_IOCTL_SET, &sdata);
    unsigned short data;
    for (data = 1; data < 9; data++)
    {
        ioctl(fd, LED_IOCTL_SET, 0x00);
        ioctl(fd, LED_IOCTL_BIT_SET, &data);
    }
}
void led_func(int fd, unsigned short retval)
{
    unsigned short led_bin_data[8];
    unsigned short i = 7, size = 0;
    memset(led_bin_data, 0, 8);
    while (retval > 0 && i >= 0)
    {
        led_bin_data[i] = retval % 2;
        i--;
        retval = retval / 2;
        size++;
    }
    // printf("%u %u", size,i);
    i = LED_ALL_OFF;
    ioctl(fd, LED_IOCTL_SET, &i);
    for (i = 0; i <= size; i++)
        if (led_bin_data[i] == 1)
        {
            // printf("%u %u", size,i);
            ioctl(fd, LED_IOCTL_BIT_SET, &i);
        }
}
void seven_seg_func(int fd, int retval)
{
    _7seg_info_t data;
    int i = 0, ret;
    ioctl(fd, _7SEG_IOCTL_ON, NULL);
    data.Mode = _7SEG_MODE_HEX_VALUE;
    data.Which = _7SEG_ALL;
    data.Value = 0x0000;
    for (i = 0; i < 4; i++)
    {
        data.Value = data.Value | (retval % 10) << (i * 4);
        retval /= 10;
    }
    ioctl(fd, _7SEG_IOCTL_SET, &data);
}

// LCD

void lcd_func(const char *exp, int fd)
{
    lcd_write_info_t display;
    // ioctl(fd, LCD_IOCTL_CLEAR, NULL);
    display.Count = sprintf((char *)display.Msg, exp);
    display.CursorX = 0;
    display.CursorY = 0;
    ioctl(fd, LCD_IOCTL_WRITE, &display);
}

char *get_input_and_display_lcd(int fd)
{
    unsigned short key, exp_index = 0, exp_size = 0;
    int ret;

    char *exp = malloc(1024 * sizeof(char));
    memset(exp, '\0', 1024);
    ioctl(fd, KEY_IOCTL_CLEAR);
    while (1)
    {
        ret = ioctl(fd, KEY_IOCTL_WAIT_CHAR, &key);
        printf("%c %d\n", key, ret);
        printf("str = %s\n", exp);
        if (((key & 0xff) >= '0') && ((key & 0xff) <= '9'))
        {
            exp[exp_index] = ((key & 0xff));
            exp_index++;
        }
        else if ((key & 0xff) == 'A')
        {
            exp[exp_index] = '+';
            exp_index++;
        }
        else if ((key & 0xff) == 'B')
        {
            exp[exp_index] = '-';
            exp_index++;
        }
        else if ((key & 0xff) == 'C')
        {
            exp[exp_index] = '*';
            exp_index++;
        }
        else if ((key & 0xff) == 'D')
        {
            exp[exp_index] = '/';
            exp_index++;
        }
        else if ((key & 0xff) == '*')
        {
            exp_index = 0;
            memset(exp, '\0', 1024);
            ioctl(fd, LCD_IOCTL_CLEAR, NULL);
        }

        else if ((key & 0xff) == '#')
            break;
        else
            continue;
        if (ret == 0)
        {
            ioctl(fd, LCD_IOCTL_CLEAR, NULL);
            lcd_func(exp, fd);
        }
    }
    return exp;
}
void set_lcd(int fd, char *ans_str, lcd_write_info_t *display)
{
    display->Count = sprintf((char *)display->Msg, (const char *)ans_str);
    ioctl(fd, LCD_IOCTL_WRITE, display);
}

void player_plot(unsigned short *data, int row, int col) {

    data [(row)*16+(col+1)] = 0;
    data [(row+1)*16+(col+1)] = 15;
    data [(row+2)*16+(col+1)] = 15;
    data [(row+3)*16+(col+1)] = 14334;
    data [(row+4)*16+(col+1)] = 14351;
    data [(row+5)*16+(col+1)] = 14367;
    data [(row+6)*16+(col+1)] = 14399;
    data [(row+7)*16+(col+1)] = 16383;

    data [row*16+col] = 0;
    data [(row+1)*16+(col)] = 61440;
    data [(row+2)*16+(col)] = 61440;
    data [(row+3)*16+(col)] = 4124;
    data [(row+4)*16+(col)] = 61468;
    data [(row+5)*16+(col)] = 63516;
    data [(row+6)*16+(col)] = 64540;
    data [(row+7)*16+(col)] = 65532;

    data [(row+8)*16+(col+1)] = 16383;
    data [(row+9)*16+(col+1)] = 16383;
    data [(row+10)*16+(col+1)] = 127;
    data [(row+11)*16+(col+1)] = 63;
    data [(row+12)*16+(col+1)] = 28;
    data [(row+13)*16+(col+1)] = 28;
    data [(row+14)*16+(col+1)] = 252;
    data [(row+15)*16+(col+1)] = 252;

    data [(row+8)*16+(col)] = 65532;
    data [(row+9)*16+(col)] = 65532;
    data [(row+10)*16+(col)] = 65024;
    data [(row+11)*16+(col)] = 65024;
    data [(row+12)*16+(col)] = 7168;
    data [(row+13)*16+(col)] = 7168;
    data [(row+14)*16+(col)] = 8128;
    data [(row+15)*16+(col)] = 8128;
}

void show_item(char map, int x, int y, unsigned short *data)
{
    if (map == 'b')
    { //水球
        data[(x)*16 + y] = 960;
        data[(x + 1) * 16 + y] = 4080;
        data[(x + 2) * 16 + y] = 8184;
        data[(x + 3) * 16 + y] = 16380;
        data[(x + 4) * 16 + y] = 8184;
        data[(x + 5) * 16 + y] = 4080;
        data[(x + 6) * 16 + y] = 960;
        data[(x + 7) * 16 + y] = 0;
    }
    else if (map == 'e')
    { //水球炸開
        data[(x + 0) * 16 + y] = 17480;
        data[(x + 1) * 16 + y] = 17480;
        data[(x + 2) * 16 + y] = 8740;
        data[(x + 3) * 16 + y] = 4370;
        data[(x + 4) * 16 + y] = 2185;
        data[(x + 5) * 16 + y] = 4370;
        data[(x + 6) * 16 + y] = 8740;
        data[(x + 7) * 16 + y] = 17480;
    }
    else if (map == '6')
    { //道具1還沒畫
        data[(x + 0) * 16 + y] = 384;
        data[(x + 1) * 16 + y] = 384;
        data[(x + 2) * 16 + y] = 384;
        data[(x + 3) * 16 + y] = 65534;
        data[(x + 4) * 16 + y] = 65534;
        data[(x + 5) * 16 + y] = 384;
        data[(x + 6) * 16 + y] = 384;
        data[(x + 7) * 16 + y] = 384;
    }
    else if (map == '7')
    { //道具2還沒畫
        data[(x + 0) * 16 + y] = 960;
        data[(x + 1) * 16 + y] = 2080;
        data[(x + 2) * 16 + y] = 8208;
        data[(x + 3) * 16 + y] = 8200;
        data[(x + 4) * 16 + y] = 8200;
        data[(x + 5) * 16 + y] = 8208;
        data[(x + 6) * 16 + y] = 2080;
        data[(x + 7) * 16 + y] = 960;
    }
    else if (map == ' ')
    { //空白
        data[(x + 0) * 16 + y] = 0;
        data[(x + 1) * 16 + y] = 0;
        data[(x + 2) * 16 + y] = 0;
        data[(x + 3) * 16 + y] = 0;
        data[(x + 4) * 16 + y] = 0;
        data[(x + 5) * 16 + y] = 0;
        data[(x + 6) * 16 + y] = 0;
        data[(x + 7) * 16 + y] = 0;
    }
    else if (map == 'B')
    { //磚塊
        data[(x + 0) * 16 + y] = 0;
        data[(x + 1) * 16 + y] = 4080;
        data[(x + 2) * 16 + y] = 0;
        data[(x + 3) * 16 + y] = 4080;
        data[(x + 4) * 16 + y] = 4080;
        data[(x + 5) * 16 + y] = 0;
        data[(x + 6) * 16 + y] = 4080;
        data[(x + 7) * 16 + y] = 0;
    }
    else if (map == 'Y')
    { //箱子
        data[(x + 0) * 16 + y] = 0;
        data[(x + 1) * 16 + y] = 32766;
        data[(x + 2) * 16 + y] = 26158;
        data[(x + 3) * 16 + y] = 24966;
        data[(x + 4) * 16 + y] = 24966;
        data[(x + 5) * 16 + y] = 25158;
        data[(x + 6) * 16 + y] = 32766;
        data[(x + 7) * 16 + y] = 0;
    }
    else if (map == 'T')
    { //樹木
        data[(x + 0) * 16 + y] = 384;
        data[(x + 1) * 16 + y] = 960;
        data[(x + 2) * 16 + y] = 2016;
        data[(x + 3) * 16 + y] = 4080;
        data[(x + 4) * 16 + y] = 8184;
        data[(x + 5) * 16 + y] = 384;
        data[(x + 6) * 16 + y] = 384;
        data[(x + 7) * 16 + y] = 384;
    }
    else if (map == 'X')
    { //房子
        data[(x + 0) * 16 + y] = 4080;
        data[(x + 1) * 16 + y] = 4104;
        data[(x + 2) * 16 + y] = 8196;
        data[(x + 3) * 16 + y] = 32766;
        data[(x + 4) * 16 + y] = 8196;
        data[(x + 5) * 16 + y] = 8196;
        data[(x + 6) * 16 + y] = 8196;
        data[(x + 7) * 16 + y] = 16380;
    }
    else if (map == '0')
    { //玩家0
        data[(x + 0) * 16 + y] = 960;
        data[(x + 1) * 16 + y] = 25542;
        data[(x + 2) * 16 + y] = 26598;
        data[(x + 3) * 16 + y] = 32766;
        data[(x + 4) * 16 + y] = 32766;
        data[(x + 5) * 16 + y] = 2016;
        data[(x + 6) * 16 + y] = 1632;
        data[(x + 7) * 16 + y] = 7800;
    }
    else if (map == ';')
    { //玩家0與水球重疊
        data[(x + 0) * 16 + y] = 0;
        data[(x + 1) * 16 + y] = 120;
        data[(x + 2) * 16 + y] = 633;
        data[(x + 3) * 16 + y] = 1023;
        data[(x + 4) * 16 + y] = 13311;
        data[(x + 5) * 16 + y] = 30840;
        data[(x + 6) * 16 + y] = 31182;
        data[(x + 7) * 16 + y] = 12288;
    }
    else if (map == '/')
    { //玩家0與炸紋重疊
        data[(x + 0) * 16 + y] = 32776;
        data[(x + 1) * 16 + y] = 33736;
        data[(x + 2) * 16 + y] = 21452;
        data[(x + 3) * 16 + y] = 16378;
        data[(x + 4) * 16 + y] = 8185;
        data[(x + 5) * 16 + y] = 9154;
        data[(x + 6) * 16 + y] = 20084;
        data[(x + 7) * 16 + y] = 32776;
    }
    else if (map == ')')
    { //玩家0被炸
        data[(x + 0) * 16 + y] = 2632;
        data[(x + 1) * 16 + y] = 9156;
        data[(x + 2) * 16 + y] = 21450;
        data[(x + 3) * 16 + y] = 40953;
        data[(x + 4) * 16 + y] = 24570;
        data[(x + 5) * 16 + y] = 9156;
        data[(x + 6) * 16 + y] = 3696;
        data[(x + 7) * 16 + y] = 2312;
    }
    else if (map == '1')
    { //玩家1
        data[(x + 0) * 16 + y] = 2016;
        data[(x + 1) * 16 + y] = 2016;
        data[(x + 2) * 16 + y] = 384;
        data[(x + 3) * 16 + y] = 15996;
        data[(x + 4) * 16 + y] = 9540;
        data[(x + 5) * 16 + y] = 8836;
        data[(x + 6) * 16 + y] = 16380;
        data[(x + 7) * 16 + y] = 1632;
    }
    else if (map == 'a')
    { //玩家1與水球重疊
        data[(x + 0) * 16 + y] = 0;
        data[(x + 1) * 16 + y] = 60;
        data[(x + 2) * 16 + y] = 60;
        data[(x + 3) * 16 + y] = 12312;
        data[(x + 4) * 16 + y] = 30951;
        data[(x + 5) * 16 + y] = 64665;
        data[(x + 6) * 16 + y] = 30975;
        data[(x + 7) * 16 + y] = 12390;
    }
    else if (map == 'z')
    { //玩家1與炸紋重疊
        data[(x + 0) * 16 + y] = 32776;
        data[(x + 1) * 16 + y] = 33736;
        data[(x + 2) * 16 + y] = 17348;
        data[(x + 3) * 16 + y] = 8578;
        data[(x + 4) * 16 + y] = 7793;
        data[(x + 5) * 16 + y] = 10642;
        data[(x + 6) * 16 + y] = 20468;
        data[(x + 7) * 16 + y] = 34408;
    }
    else if (map == '!')
    { //玩家1被炸
        data[(x + 0) * 16 + y] = 1344;
        data[(x + 1) * 16 + y] = 3024;
        data[(x + 2) * 16 + y] = 5064;
        data[(x + 3) * 16 + y] = 8580;
        data[(x + 4) * 16 + y] = 20082;
        data[(x + 5) * 16 + y] = 35217;
        data[(x + 6) * 16 + y] = 20466;
        data[(x + 7) * 16 + y] = 9828;
    }
    else if (map == '2')
    { //玩家0
        data[(x + 0) * 16 + y] = 960;
        data[(x + 1) * 16 + y] = 25542;
        data[(x + 2) * 16 + y] = 26598;
        data[(x + 3) * 16 + y] = 32766;
        data[(x + 4) * 16 + y] = 32766;
        data[(x + 5) * 16 + y] = 2016;
        data[(x + 6) * 16 + y] = 1632;
        data[(x + 7) * 16 + y] = 7800;
    }
    else if (map == 's')
    { //玩家0與水球重疊
        data[(x + 0) * 16 + y] = 0;
        data[(x + 1) * 16 + y] = 120;
        data[(x + 2) * 16 + y] = 633;
        data[(x + 3) * 16 + y] = 1023;
        data[(x + 4) * 16 + y] = 13311;
        data[(x + 5) * 16 + y] = 30840;
        data[(x + 6) * 16 + y] = 31182;
        data[(x + 7) * 16 + y] = 12288;
    }
    else if (map == 'x')
    { //玩家0與炸紋重疊
        data[(x + 0) * 16 + y] = 32776;
        data[(x + 1) * 16 + y] = 33736;
        data[(x + 2) * 16 + y] = 21452;
        data[(x + 3) * 16 + y] = 16378;
        data[(x + 4) * 16 + y] = 8185;
        data[(x + 5) * 16 + y] = 9154;
        data[(x + 6) * 16 + y] = 20084;
        data[(x + 7) * 16 + y] = 32776;
    }
    else if (map == '@')
    { //玩家0被炸
        data[(x + 0) * 16 + y] = 2632;
        data[(x + 1) * 16 + y] = 9156;
        data[(x + 2) * 16 + y] = 21450;
        data[(x + 3) * 16 + y] = 40953;
        data[(x + 4) * 16 + y] = 24570;
        data[(x + 5) * 16 + y] = 9156;
        data[(x + 6) * 16 + y] = 3696;
        data[(x + 7) * 16 + y] = 2312;
    }
    else if (map == '3')
    { //玩家1
        data[(x + 0) * 16 + y] = 2016;
        data[(x + 1) * 16 + y] = 2016;
        data[(x + 2) * 16 + y] = 384;
        data[(x + 3) * 16 + y] = 15996;
        data[(x + 4) * 16 + y] = 9540;
        data[(x + 5) * 16 + y] = 8836;
        data[(x + 6) * 16 + y] = 16380;
        data[(x + 7) * 16 + y] = 1632;
    }
    else if (map == 'd')
    { //玩家1與水球重疊
        data[(x + 0) * 16 + y] = 0;
        data[(x + 1) * 16 + y] = 60;
        data[(x + 2) * 16 + y] = 60;
        data[(x + 3) * 16 + y] = 12312;
        data[(x + 4) * 16 + y] = 30951;
        data[(x + 5) * 16 + y] = 64665;
        data[(x + 6) * 16 + y] = 30975;
        data[(x + 7) * 16 + y] = 12390;
    }
    else if (map == 'c')
    { //玩家1與炸紋重疊
        data[(x + 0) * 16 + y] = 32776;
        data[(x + 1) * 16 + y] = 33736;
        data[(x + 2) * 16 + y] = 17348;
        data[(x + 3) * 16 + y] = 8578;
        data[(x + 4) * 16 + y] = 7793;
        data[(x + 5) * 16 + y] = 10642;
        data[(x + 6) * 16 + y] = 20468;
        data[(x + 7) * 16 + y] = 34408;
    }
    else if (map == '#')
    { //玩家1被炸
        data[(x + 0) * 16 + y] = 1344;
        data[(x + 1) * 16 + y] = 3024;
        data[(x + 2) * 16 + y] = 5064;
        data[(x + 3) * 16 + y] = 8580;
        data[(x + 4) * 16 + y] = 20082;
        data[(x + 5) * 16 + y] = 35217;
        data[(x + 6) * 16 + y] = 20466;
        data[(x + 7) * 16 + y] = 9828;
    }
    else
    {
        data[(x + 0) * 16 + y] = 0;
        data[(x + 1) * 16 + y] = 0;
        data[(x + 2) * 16 + y] = 0;
        data[(x + 3) * 16 + y] = 0;
        data[(x + 4) * 16 + y] = 0;
        data[(x + 5) * 16 + y] = 0;
        data[(x + 6) * 16 + y] = 0;
        data[(x + 7) * 16 + y] = 0;
    }
}

// void show_item(char **map, int x, int y, unsigned short* data){
//     printf("%d %d %c\n",x,y,map[x][y]);
//     if(map[x][y] == 'b'){ //水球
//         data [(x)*16+y] = 960;
//         data [(x+1)*16+y] = 4080;
//         data [(x+2)*16+y] = 8184;
//         data [(x+3)*16+y] = 16380;
//         data [(x+4)*16+y] = 8184;
//         data [(x+5)*16+y] = 4080;
//         data [(x+6)*16+y] = 960;
//         data [(x+7)*16+y] = 0;
//     }
//     else if(map[x][y] == 'e'){ //水球炸開
//         data [(x+0)*16+y] = 17480;
//         data [(x+1)*16+y] = 17480;
//         data [(x+2)*16+y] = 8740;
//         data [(x+3)*16+y] = 4370;
//         data [(x+4)*16+y] = 2185;
//         data [(x+5)*16+y] = 4370;
//         data [(x+6)*16+y] = 8740;
//         data [(x+7)*16+y] = 17480;
//     }
//     else if(map[x][y] == '6'){ //道具1還沒畫
//         data [(x+0)*16+y] = 17480;
//         data [(x+1)*16+y] = 17480;
//         data [(x+2)*16+y] = 8740;
//         data [(x+3)*16+y] = 4370;
//         data [(x+4)*16+y] = 2185;
//         data [(x+5)*16+y] = 4370;
//         data [(x+6)*16+y] = 8740;
//         data [(x+7)*16+y] = 17480;
//     }
//     else if(map[x][y] == '7'){ //道具2還沒畫
//         data [(x+0)*16+y] = 17480;
//         data [(x+1)*16+y] = 17480;
//         data [(x+2)*16+y] = 8740;
//         data [(x+3)*16+y] = 4370;
//         data [(x+4)*16+y] = 2185;
//         data [(x+5)*16+y] = 4370;
//         data [(x+6)*16+y] = 8740;
//         data [(x+7)*16+y] = 17480;
//     }
//     else if(map[x][y] == ' '){ //空白
//         data [(x+0)*16+y] = 0;
//         data [(x+1)*16+y] = 0;
//         data [(x+2)*16+y] = 0;
//         data [(x+3)*16+y] = 0;
//         data [(x+4)*16+y] = 0;
//         data [(x+5)*16+y] = 0;
//         data [(x+6)*16+y] = 0;
//         data [(x+7)*16+y] = 0;
//     }
//     else if(map[x][y] == 'B'){ //磚塊
//         printf("draw 1 %d\n",__LINE__);
//         data [(x+0)*16+y] = 65535;
//         data [(x+1)*16+y] = 65535;
//         data [(x+2)*16+y] = 0;
//         data [(x+3)*16+y] = 65535;
//         data [(x+4)*16+y] = 65535;
//         data [(x+5)*16+y] = 0;
//         data [(x+6)*16+y] = 65535;
//         data [(x+7)*16+y] = 65535;
//     }
//     else if(map[x][y] == 'Y'){ //箱子
//         data [(x+0)*16+y] = 0;
//         data [(x+1)*16+y] = 32766;
//         data [(x+2)*16+y] = 26158;
//         data [(x+3)*16+y] = 24966;
//         data [(x+4)*16+y] = 24966;
//         data [(x+5)*16+y] = 25158;
//         data [(x+6)*16+y] = 32766;
//         data [(x+7)*16+y] = 0;
//     }
//     else if(map[x][y] == 'T'){ //樹木
//         data [(x+0)*16+y] = 384;
//         data [(x+1)*16+y] = 960;
//         data [(x+2)*16+y] = 2016;
//         data [(x+3)*16+y] = 4080;
//         data [(x+4)*16+y] = 8184;
//         data [(x+5)*16+y] = 384;
//         data [(x+6)*16+y] = 384;
//         data [(x+7)*16+y] = 384;
//     }
//     else if(map[x][y] == 'x'){ //房子
//         data [(x+0)*16+y] = 4080;
//         data [(x+1)*16+y] = 4104;
//         data [(x+2)*16+y] = 8196;
//         data [(x+3)*16+y] = 32766;
//         data [(x+4)*16+y] = 8196;
//         data [(x+5)*16+y] = 9156;
//         data [(x+6)*16+y] = 9156;
//         data [(x+7)*16+y] = 16380;
//     }
//     else if(map[x][y] == '0'){ //玩家0
//         data [(x+0)*16+y] = 960;
//         data [(x+1)*16+y] = 25542;
//         data [(x+2)*16+y] = 26598;
//         data [(x+3)*16+y] = 32766;
//         data [(x+4)*16+y] = 32766;
//         data [(x+5)*16+y] = 2016;
//         data [(x+6)*16+y] = 1632;
//         data [(x+7)*16+y] = 7800;
//     }
//     else if(map[x][y] == ';'){ //玩家0與水球重疊
//         data [(x+0)*16+y] = 0;
//         data [(x+1)*16+y] = 120;
//         data [(x+2)*16+y] = 633;
//         data [(x+3)*16+y] = 1023;
//         data [(x+4)*16+y] = 13311;
//         data [(x+5)*16+y] = 30840;
//         data [(x+6)*16+y] = 31182;
//         data [(x+7)*16+y] = 12288;
//     }
//     else if(map[x][y] == '/'){ //玩家0與炸紋重疊
//         data [(x+0)*16+y] = 32776;
//         data [(x+1)*16+y] = 33736;
//         data [(x+2)*16+y] = 21452;
//         data [(x+3)*16+y] = 16378;
//         data [(x+4)*16+y] = 8185;
//         data [(x+5)*16+y] = 9154;
//         data [(x+6)*16+y] = 20084;
//         data [(x+7)*16+y] = 32776;
//     }
//     else if(map[x][y] == ')'){ //玩家0被炸
//         data [(x+0)*16+y] = 2632;
//         data [(x+1)*16+y] = 9156;
//         data [(x+2)*16+y] = 21450;
//         data [(x+3)*16+y] = 40953;
//         data [(x+4)*16+y] = 24570;
//         data [(x+5)*16+y] = 9156;
//         data [(x+6)*16+y] = 3696;
//         data [(x+7)*16+y] = 2312;
//     }
//     else if(map[x][y] == '1'){ //玩家1
//         data [(x+0)*16+y] = 2016;
//         data [(x+1)*16+y] = 2016;
//         data [(x+2)*16+y] = 384;
//         data [(x+3)*16+y] = 15996;
//         data [(x+4)*16+y] = 9540;
//         data [(x+5)*16+y] = 8836;
//         data [(x+6)*16+y] = 16380;
//         data [(x+7)*16+y] = 1632;
//     }
//     else if(map[x][y] == 'a'){ //玩家1與水球重疊
//         data [(x+0)*16+y] = 0;
//         data [(x+1)*16+y] = 60;
//         data [(x+2)*16+y] = 60;
//         data [(x+3)*16+y] = 12312;
//         data [(x+4)*16+y] = 30951;
//         data [(x+5)*16+y] = 64665;
//         data [(x+6)*16+y] = 30975;
//         data [(x+7)*16+y] = 12390;
//     }
//     else if(map[x][y] == 'z'){ //玩家1與炸紋重疊
//         data [(x+0)*16+y] = 32776;
//         data [(x+1)*16+y] = 33736;
//         data [(x+2)*16+y] = 17348;
//         data [(x+3)*16+y] = 8578;
//         data [(x+4)*16+y] = 7793;
//         data [(x+5)*16+y] = 10642;
//         data [(x+6)*16+y] = 20468;
//         data [(x+7)*16+y] = 34408;
//     }
//     else if(map[x][y] == '/'){ //玩家1被炸
//         data [(x+0)*16+y] = 1344;
//         data [(x+1)*16+y] = 3024;
//         data [(x+2)*16+y] = 5064;
//         data [(x+3)*16+y] = 8580;
//         data [(x+4)*16+y] = 20082;
//         data [(x+5)*16+y] = 35217;
//         data [(x+6)*16+y] = 20466;
//         data [(x+7)*16+y] = 9828;
//     }
//     else{
//         data [(x+0)*16+y] = 0;
//         data [(x+1)*16+y] = 0;
//         data [(x+2)*16+y] = 0;
//         data [(x+3)*16+y] = 0;
//         data [(x+4)*16+y] = 0;
//         data [(x+5)*16+y] = 0;
//         data [(x+6)*16+y] = 0;
//         data [(x+7)*16+y] = 0;
//     }

// }
