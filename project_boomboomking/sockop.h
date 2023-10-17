
#define SEM_MODE 0666
#define BUFSIZE 1024

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
int passivesock(const char *service, const char *transport, int qlen);
int connectsock(const char *host, const char *service, const char *transport);
int makesem(long int key, int val);
int rmsem(long int key);
int getandlock(int s);
int releaseandunlock(int s);

