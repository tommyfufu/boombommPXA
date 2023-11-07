#pragma once
#include <cstdio>      // fprintf(), perror()
#include <cstdlib>     // exit()
#include <cstring>     // memset()
#include <csignal>    // signal()
#include <fcntl.h>     // open()
#include <unistd.h>    // read(), write(), close()
#include <pthread.h>
#include <sys/socket.h> // socket(), connect()
#include <netinet/in.h> // struct sockaddr_in
#include <arpa/inet.h>  // htons()
#include <sys/stat.h>
#include <sys/wait.h>


int socket_host(char*);
    