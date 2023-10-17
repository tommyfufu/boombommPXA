#include "project.h"
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>

#include "socket_u.h"
#include "sem_u.h"
void initial();
int kbhit ();


FILE *fp;
static int sem_id = 0;
struct para_th para_th_socket;
struct sock_data sock_data_map;
pthread_mutex_t mutex;
World w;
int flag_start = 0;
void* SocketHandler(void*);
void* GameHandler(void*);
void* BroadHandler(void*);

void handler(int signum) {
    del_semvalue(sem_id);
    printf("close semaphore");
    close(para_th_socket.csock[0]);
    close(para_th_socket.csock[1]);
    printf("close socket");
    endwin();
    exit(1);
}
int connfd;
int debug(string str){
    // //cout<<"str: "<<str<<"--\n";
    if(connfd == -1){
        return -1;
    }
    else{
        str+="\n";
        if(write(connfd, str.c_str(), str.size()) == -1){
            perror("Error: write()\n");
            return -1;
        }
        return 0;
    }
}

int main(int argc, char** argv) {
    connfd = connectsock("127.0.0.1", argv[1], "tcp");
    // if(argc == 3){
    //     connfd = connectsock(argv[1], argv[2], "tcp");
    // }
    // else if(argc == 2){
    //     connfd = connectsock("127.0.0.1", argv[1], "tcp");
    // }
    // else {
    //     connfd = -1;
    // }
    signal(SIGINT, handler);
    
    
    
    // sem
    sem_id = semget((key_t) 1234, 1, 0666 | IPC_CREAT);
    if (!set_semvalue(sem_id)){
        printf("error");
    }

    initial();
    w.sock_data_p = &sock_data_map;
    w.gameroom();
    //cout << "Player setted" << endl;
    w.readmap(w.mapchoose());
    //cout << "Map setted" << endl;
    w.mainmap();
    // endwin();
    // Socket 
    socklen_t addr_size = 0;
    sockaddr_in sadr;
    pthread_t thread_id=0;
    int hsock = socket_host("8888");
    para_th_socket.player_num = 0;
    pthread_mutex_init(&mutex, 0);
    debug("START");
    while(1){
        //printf("waiting for a connection\n");
        //para_th_socket.csock = (int*)malloc(sizeof(int)*2);
        if(para_th_socket.player_num < 2){
            if((para_th_socket.csock[para_th_socket.player_num] = accept(hsock, (sockaddr*)&sadr, &addr_size))!= -1){
                
                debug(string("———————\nReceived connection from") + string(inet_ntoa(sadr.sin_addr)));
                //fflush(stdout);
                pthread_create(&thread_id,0,&SocketHandler, (void*)&para_th_socket);
                pthread_detach(thread_id);
                usleep(1000);
                para_th_socket.player_num++;
            }
        }  
        if (flag_start == 0 && para_th_socket.player_num == 2){
            w.para_world = &para_th_socket;
            
            debug(to_string(__LINE__));
            w.sock_data_p->map_p[0][0] = '1';
            debug(to_string(w.sock_data_p->map_p[0][0]));
            for(int i=0;i<2;i++){
                struct para_th *para_th_socket_temp = (struct para_th * )malloc(sizeof(struct para_th));
                memcpy(para_th_socket_temp,&para_th_socket,sizeof(struct para_th));
                para_th_socket_temp->player_num = i;
                pthread_create(&thread_id,0,&BroadHandler, (void*)para_th_socket_temp);
                pthread_detach(thread_id);
            }
            pthread_create(&thread_id,0,&GameHandler, (void*)&para_th_socket);
            pthread_detach(thread_id);
            memset(w.sock_data_p->buf,'\0',64);
            sprintf(w.sock_data_p->buf,"start");
            flag_start = 1;
        }
        // else{
        //    fprintf(stderr, "Error accept\n");
        // }
    }
    // while(1) {
    //     if (kbhit()) {
    //         x=getch();
    //         // //cout<<x<<endl;
    //         w.checkplayer();
    //         w.checkbomb();
    //         switch (x){
    //             case 'w':
    //             case 's':
    //             case 'a':
    //             case 'd':
    //                 if(w.setted(0)){
    //                     w.move(x,'w','s','a','d',0);
    //                 }
    //                 break;
    //             case '\t':
    //                 if(w.setted(0)){
    //                     w.setbomb(0);
    //                 }
    //                 break;
    //             case KEY_UP:
    //             case KEY_DOWN:
    //             case KEY_LEFT:
    //             case KEY_RIGHT:
    //                 if(w.setted(1)){
    //                     w.move(x,KEY_UP,KEY_DOWN,KEY_LEFT,KEY_RIGHT,1);
    //                 }
    //                 break;
    //             case ' ':
    //                 if(w.setted(1)){
    //                     w.setbomb(1);
    //                 }
    //                 break;
    //             case 'g':
    //             case 'b':
    //             case 'v':
    //             case 'n':
    //                 if(w.setted(2)){
    //                     w.move(x,'g','b','v','n',2);
    //                 }
    //                 break;
    //             case 'x':
    //                 if(w.setted(2)){
    //                     w.setbomb(2);
    //                 }
    //                 break;
    //             case 'o':
    //             case 'l':
    //             case 'k':
    //             case ';':
    //                 if(w.setted(3)){
    //                     w.move(x,'o','l','k',';',3);
    //                 }
    //                 break;
    //             case 'h':
    //                 if(w.setted(3)){
    //                     w.setbomb(3);
    //                 }
    //                 break;
    //             case 27:
    //                 w.quitgame();
    //         }
    //         w.mainmap();
    //     }
    //     else{
    //         w.checkplayer();
    //         w.checkbomb();
    //         w.mainmap();
    //     }
    // }
    endwin();
    if(connfd != -1)
        close(connfd);
    return 0;
}

void initial()
{
    initscr();
    cbreak();       //unblocking keyboard intupt
    start_color();
    init_color(COLOR_BLACK, 100, 100, 100);
    init_pair(0,COLOR_BLACK,COLOR_BLACK);
    init_pair(1,COLOR_RED,COLOR_BLACK);
    init_pair(2,COLOR_GREEN,COLOR_BLACK);
    init_pair(3,COLOR_YELLOW,COLOR_BLACK);
    init_pair(4,COLOR_BLUE,COLOR_BLACK);
    init_pair(5,COLOR_MAGENTA,COLOR_BLACK);
    init_pair(6,COLOR_CYAN,COLOR_BLACK);
    init_pair(7,COLOR_WHITE,COLOR_BLACK);
    nonl();
    curs_set(0);
    nodelay(stdscr,TRUE);
    noecho();
    // echo();
    intrflush(stdscr,FALSE);
    keypad(stdscr,TRUE);
    refresh();
}
int kbhit()
{
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}
void* GameHandler(void* para_st){
    //cout << "GAME" << endl;
    while (1)
    {
        /* code */
        //debug("game handler");
        pthread_mutex_lock(&mutex);
        w.checkplayer();
        w.checkbomb();
        w.mainmap();
        pthread_mutex_unlock(&mutex);
        usleep(1000);
        // fp = fopen("./result.txt","w+");
        // for (size_t i = 0; i < 13; i++)
        // {
        //     /* code */
        //     for (size_t j = 0; j < 15; j++)
        //     {
        //         /* code */
        //         fprintf(fp,"%c",w.sock_data_p->map_p[i][j]);
        //     }
        //     fprintf(fp,"\n");
        // }
        // fclose(fp);
        // sleep(1);
    }
}
void* SocketHandler(void* para_st){
    //cout << "THREAD" << endl;
    struct para_th *para_ith_socket;
    para_ith_socket = (struct para_th * )malloc(sizeof(struct para_th));
    memcpy(para_ith_socket,para_st,sizeof(struct para_th));
    //cout << para_ith_socket->csock[para_ith_socket->player_num] << para_ith_socket->player_num << endl;
    char buffer[1024];
    char x;
    int buffer_len = 1;
    char temp[1024];
    int temp_len = 1024;
    int bytecount;
    struct sock_data sock_data_map_th;
    usleep(1000);
    if(para_th_socket.player_num != 2){
        sprintf(sock_data_map_th.buf,"wait");
        send(para_ith_socket->csock[para_ith_socket->player_num], &sock_data_map_th, sizeof(struct sock_data),0);
        debug("Waiting _ socket send");
    }
    while(1){
        if(para_th_socket.player_num != 2){
            sleep(1);
            debug("Waiting _ p");
        }
        else{
            break;
        }
    }
    while(1){
        ////cout << para_ith_socket->csock[para_ith_socket->player_num] << para_ith_socket->player_num << endl;
        bytecount = recv(para_ith_socket->csock[para_ith_socket->player_num], &x, buffer_len, 0);
        if(bytecount == -1){
            continue;
        }
        else if(bytecount == 0){
            debug("leave \n");
            break;
        }
        else{
            //cout << x << endl;
            debug(to_string(x));
            pthread_mutex_lock(&mutex);
            w.checkplayer();
            w.checkbomb();
            switch (x){
                case 'w':
                case 's':
                case 'a':
                case 'd':
                    ////cout << "AAAAAA" << endl;
                    if(w.setted(para_ith_socket->player_num)){
                        w.move(x,'w','s','a','d',para_ith_socket->player_num);
                    }
                    break;
                case ' ':
                    if(w.setted(para_ith_socket->player_num)){
                        w.setbomb(para_ith_socket->player_num);
                    }
                    break;
            }
            w.mainmap();
            pthread_mutex_unlock(&mutex);
        }
    }
}

void* BroadHandler(void* para_st){
    //cout << "THREAD" << endl;
    struct para_th *para_ith_socket;
    para_ith_socket = (struct para_th * )malloc(sizeof(struct para_th));
    memcpy(para_ith_socket,para_st,sizeof(struct para_th));
    //cout << para_ith_socket->csock[para_ith_socket->player_num] << para_ith_socket->player_num << endl;
    char buffer[1024];
    char x;
    int buffer_len = 1;
    char temp[1024];
    int temp_len = 1024;
    int bytecount;
    struct sock_data sock_data_map_th;
    // pthread_mutex_lock(&mutex);
    // memcpy(&sock_data_map_th,w.sock_data_p,sizeof(struct sock_data));
    // pthread_mutex_unlock(&mutex);
    // debug(w.sock_data_p->buf);
    // sprintf(sock_data_map_th.buf,"start");
    // send(para_ith_socket->csock[para_ith_socket->player_num], &sock_data_map_th, sizeof(struct sock_data),0);
    // debug(string("send"));
    // debug(to_string(para_ith_socket->player_num));
    while(1){
        usleep(50000);
        semaphore_p(sem_id);
        if(strcmp(w.sock_data_p->buf,"start")==0){ // Send map
            pthread_mutex_lock(&mutex);
            memcpy(&sock_data_map_th,w.sock_data_p,sizeof(struct sock_data));
            pthread_mutex_unlock(&mutex);
            //sprintf(sock_data_map_th.buf,"start");
            send(para_ith_socket->csock[para_ith_socket->player_num], &sock_data_map_th, sizeof(struct sock_data),0);
            //debug(string("send in game"));
        }
        else{
            debug("win wait for continue");
        }
        semaphore_v(sem_id);
    }
}

// void* BroadHandler(void* para_st){
//     //cout << "THREAD" << endl;
//     struct para_th *para_ith_socket;
//     para_ith_socket = (struct para_th * )malloc(sizeof(struct para_th));
//     memcpy(para_ith_socket,para_st,sizeof(struct para_th));
//     //cout << para_ith_socket->csock[para_ith_socket->player_num] << para_ith_socket->player_num << endl;
//     char buffer[1024];
//     char x;
//     int buffer_len = 1;
//     char temp[1024];
//     int temp_len = 1024;
//     int bytecount;
//     struct sock_data sock_data_map_th;
//     pthread_mutex_lock(&mutex);
//     memcpy(&sock_data_map_th,w.sock_data_p,sizeof(struct sock_data));
//     pthread_mutex_unlock(&mutex);
//     sprintf(sock_data_map_th.buf,"START");
//     send(para_ith_socket->csock[para_ith_socket->player_num], &sock_data_map_th, sizeof(struct sock_data),0);
//     debug(string("send"));
//     debug(to_string(para_ith_socket->player_num));
// }