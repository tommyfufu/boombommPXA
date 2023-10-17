#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include  "asm-arm/arch-pxa/lib/creator_pxa270_lcd.h"
//#include <show_item.h>

void show_item(char **map, int x, int y, unsigned short *data){



    if(map[x][y] == 'b'){ //水球
        data [(x)*16+y] = 960;
        data [(x+1)*16+y] = 4080;
        data [(x+2)*16+y] = 8184;
        data [(x+3)*16+y] = 16380;
        data [(x+4)*16+y] = 8184;
        data [(x+5)*16+y] = 4080;
        data [(x+6)*16+y] = 960;
        data [(x+7)*16+y] = 0;
    }
    else if(map[x][y] == 'e'){ //水球炸開
        data [(x+0)*16+y] = 17480;
        data [(x+1)*16+y] = 17480;
        data [(x+2)*16+y] = 8740;
        data [(x+3)*16+y] = 4370;
        data [(x+4)*16+y] = 2185;
        data [(x+5)*16+y] = 4370;
        data [(x+6)*16+y] = 8740;
        data [(x+7)*16+y] = 17480;
    }
    else if(map[x][y] == '6'){ //道具1還沒畫
        data [(x+0)*16+y] = 17480;
        data [(x+1)*16+y] = 17480;
        data [(x+2)*16+y] = 8740;
        data [(x+3)*16+y] = 4370;
        data [(x+4)*16+y] = 2185;
        data [(x+5)*16+y] = 4370;
        data [(x+6)*16+y] = 8740;
        data [(x+7)*16+y] = 17480;
    }
    else if(map[x][y] == '7'){ //道具2還沒畫
        data [(x+0)*16+y] = 17480;
        data [(x+1)*16+y] = 17480;
        data [(x+2)*16+y] = 8740;
        data [(x+3)*16+y] = 4370;
        data [(x+4)*16+y] = 2185;
        data [(x+5)*16+y] = 4370;
        data [(x+6)*16+y] = 8740;
        data [(x+7)*16+y] = 17480;
    }
    else if(map[x][y] == ' '){ //空白
        data [(x+0)*16+y] = 0;
        data [(x+1)*16+y] = 0;
        data [(x+2)*16+y] = 0;
        data [(x+3)*16+y] = 0;
        data [(x+4)*16+y] = 0;
        data [(x+5)*16+y] = 0;
        data [(x+6)*16+y] = 0;
        data [(x+7)*16+y] = 0;
    }
    else if(map[x][y] == 'B'){ //磚塊
        data [(x+0)*16+y] = 65535;
        data [(x+1)*16+y] = 65535;
        data [(x+2)*16+y] = 0;
        data [(x+3)*16+y] = 65535;
        data [(x+4)*16+y] = 65535;
        data [(x+5)*16+y] = 0;
        data [(x+6)*16+y] = 65535;
        data [(x+7)*16+y] = 65535;
    }
    else if(map[x][y] == 'Y'){ //箱子
        data [(x+0)*16+y] = 0;
        data [(x+1)*16+y] = 32766;
        data [(x+2)*16+y] = 26158;
        data [(x+3)*16+y] = 24966;
        data [(x+4)*16+y] = 24966;
        data [(x+5)*16+y] = 25158;
        data [(x+6)*16+y] = 32766;
        data [(x+7)*16+y] = 0;
    }
    else if(map[x][y] == 'T'){ //樹木
        data [(x+0)*16+y] = 384;
        data [(x+1)*16+y] = 960;
        data [(x+2)*16+y] = 2016;
        data [(x+3)*16+y] = 4080;
        data [(x+4)*16+y] = 8184;
        data [(x+5)*16+y] = 384;
        data [(x+6)*16+y] = 384;
        data [(x+7)*16+y] = 384;
    }
    else if(map[x][y] == 'x'){ //房子
        data [(x+0)*16+y] = 4080;
        data [(x+1)*16+y] = 4104;
        data [(x+2)*16+y] = 8196;
        data [(x+3)*16+y] = 32766;
        data [(x+4)*16+y] = 8196;
        data [(x+5)*16+y] = 9156;
        data [(x+6)*16+y] = 9156;
        data [(x+7)*16+y] = 16380;
    }
    else if(map[x][y] == '0'){ //玩家0
        data [(x+0)*16+y] = 960;
        data [(x+1)*16+y] = 25542;
        data [(x+2)*16+y] = 26598;
        data [(x+3)*16+y] = 32766;
        data [(x+4)*16+y] = 32766;
        data [(x+5)*16+y] = 2016;
        data [(x+6)*16+y] = 1632;
        data [(x+7)*16+y] = 7800;
    }
    else if(map[x][y] == ';'){ //玩家0與水球重疊
        data [(x+0)*16+y] = 0;
        data [(x+1)*16+y] = 120;
        data [(x+2)*16+y] = 633;
        data [(x+3)*16+y] = 1023;
        data [(x+4)*16+y] = 13311;
        data [(x+5)*16+y] = 30840;
        data [(x+6)*16+y] = 31182;
        data [(x+7)*16+y] = 12288;
    }
    else if(map[x][y] == '/'){ //玩家0與炸紋重疊
        data [(x+0)*16+y] = 32776;
        data [(x+1)*16+y] = 33736;
        data [(x+2)*16+y] = 21452;
        data [(x+3)*16+y] = 16378;
        data [(x+4)*16+y] = 8185;
        data [(x+5)*16+y] = 9154;
        data [(x+6)*16+y] = 20084;
        data [(x+7)*16+y] = 32776;
    }
    else if(map[x][y] == ')'){ //玩家0被炸
        data [(x+0)*16+y] = 2632;
        data [(x+1)*16+y] = 9156;
        data [(x+2)*16+y] = 21450;
        data [(x+3)*16+y] = 40953;
        data [(x+4)*16+y] = 24570;
        data [(x+5)*16+y] = 9156;
        data [(x+6)*16+y] = 3696;
        data [(x+7)*16+y] = 2312;
    }
    else if(map[x][y] == '1'){ //玩家1
        data [(x+0)*16+y] = 2016;
        data [(x+1)*16+y] = 2016;
        data [(x+2)*16+y] = 384;
        data [(x+3)*16+y] = 15996;
        data [(x+4)*16+y] = 9540;
        data [(x+5)*16+y] = 8836;
        data [(x+6)*16+y] = 16380;
        data [(x+7)*16+y] = 1632;
    }
    else if(map[x][y] == 'a'){ //玩家1與水球重疊
        data [(x+0)*16+y] = 0;
        data [(x+1)*16+y] = 60;
        data [(x+2)*16+y] = 60;
        data [(x+3)*16+y] = 12312;
        data [(x+4)*16+y] = 30951;
        data [(x+5)*16+y] = 64665;
        data [(x+6)*16+y] = 30975;
        data [(x+7)*16+y] = 12390;
    }
    else if(map[x][y] == 'z'){ //玩家1與炸紋重疊
        data [(x+0)*16+y] = 32776;
        data [(x+1)*16+y] = 33736;
        data [(x+2)*16+y] = 17348;
        data [(x+3)*16+y] = 8578;
        data [(x+4)*16+y] = 7793;
        data [(x+5)*16+y] = 10642;
        data [(x+6)*16+y] = 20468;
        data [(x+7)*16+y] = 34408;
    }
    else if(map[x][y] == '/'){ //玩家1被炸
        data [(x+0)*16+y] = 1344;
        data [(x+1)*16+y] = 3024;
        data [(x+2)*16+y] = 5064;
        data [(x+3)*16+y] = 8580;
        data [(x+4)*16+y] = 20082;
        data [(x+5)*16+y] = 35217;
        data [(x+6)*16+y] = 20466;
        data [(x+7)*16+y] = 9828;
    }
    else{
        data [(x+0)*16+y] = 0;
        data [(x+1)*16+y] = 0;
        data [(x+2)*16+y] = 0;
        data [(x+3)*16+y] = 0;
        data [(x+4)*16+y] = 0;
        data [(x+5)*16+y] = 0;
        data [(x+6)*16+y] = 0;
        data [(x+7)*16+y] = 0;
    }
}


