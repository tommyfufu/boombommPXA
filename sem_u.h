#pragma once
#include <cstdio>      // fprintf(), perror()
#include <cstdlib>     // exit()
#include <cstring>     // memset()
#include <csignal>    // signal()
#include <fcntl.h>     // open()
#include <unistd.h>    // read(), write(), close()
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/wait.h>

union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
};

int set_semvalue(int);
void del_semvalue(int);
int semaphore_p(int);
int semaphore_v(int);
