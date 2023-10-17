#include "sockop.h"

int passivesock(const char *service, const char *transport, int qlen)
{
    struct servent *pse;
    struct sockaddr_in sin;
    int s, type;

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;

    if ((pse = getservbyname(service, transport)))
        sin.sin_port = htons(ntohs((unsigned short)pse->s_port));
    else if ((sin.sin_port = htons((unsigned short)atoi(service))) == 0)
    {
        printf("Cant get \"%s\" service entry\n ", service);
        exit(EXIT_FAILURE);
    }

    if (strcmp(transport, "udp") == 0)
        type = SOCK_DGRAM;
    else
        type = SOCK_STREAM;

    s = socket(PF_INET, type, 0);
    if (s < 0)
    {
        printf("Cant create socket: %s\n ", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        printf("Can't bind to port %s: %s\n ", service, strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (type == SOCK_STREAM && listen(s, qlen) < 0)
    {
        printf("Can't listen on port %s: %s\n ", service, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return s;
}
int connectsock(const char *host, const char *service, const char *transport)
{
    struct hostent *phe;
    struct servent *pse;
    struct sockaddr_in sin;
    int s, type;

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;

    if ((pse = getservbyname(service, transport)))
        sin.sin_port = pse->s_port;
    else if ((sin.sin_port = htons((unsigned short)atoi(service))) == 0)
    {
        printf("Cant get \"%s\" service entry\n ", service);
        exit(EXIT_FAILURE);
    }

    if ((phe = gethostbyname(host)))
        memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
    else if ((sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE)
    {
        printf("Cant get \"%s\" host entry\n ", host);
        exit(EXIT_FAILURE);
    }

    if (strcmp(transport, "udp") == 0)
        type = SOCK_DGRAM;
    else
        type = SOCK_STREAM;

    s = socket(PF_INET, type, 0);
    if (s < 0)
    {
        printf("Cant create socket: %s\n ", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
        printf("Can't connect to %s.%s: %s\n ", host, service, strerror(errno));
        exit(EXIT_FAILURE);
    }

    return s;
}
int makesem(long int key, int val)
{
    int s;
    s = semget(key, 1, IPC_CREAT | IPC_EXCL | SEM_MODE);
    printf("make s: %d\n", s);
    if (s < 0)
    {
        fprintf(stderr, "makesem: creation of semaphore %ld failed: %s\n", key, strerror(errno));
        return -1;
    }
    printf("Semaphore %ld created\n", key);
    if (semctl(s, 0, SETVAL, val) < 0)
    {
        fprintf(stderr, "makesem: Unable to initialize semaphore:%s\n", strerror(errno));
        return -1;
    }
    printf("Semaphore %ld has been init to %d\n", key, val);
    return s;
}
int rmsem(long int key)
{
    int s;
    s = semget(key, 1, 0);
    if (s < 0)
    {
        fprintf(stderr, "rmsem: failed to find semaphore %ld failed: %s\n", key, strerror(errno));
        return -1;
    }
    printf("Semaphore %ld found\n", key);
    if (semctl(s, 0, IPC_RMID, 0) < 0)
    {
        fprintf(stderr, "rmsem: Unable to remove semaphore:%ld\n", key);
        return -1;
    }
    printf("Semaphore %ld has been remove\n", key);
    return 0;
}
int getandlock(int s)
{
    struct sembuf sop;
    sop.sem_flg = 0;
    sop.sem_num = 0;
    sop.sem_op = -1;
    if (semop(s, &sop, 1) < 0)
    {
        fprintf(stderr, "getandlock:semop faild: %s\n", strerror(errno));
        return -1;
    }
    else
        return 0;
}
int releaseandunlock(int s)
{
    struct sembuf sop;
    sop.sem_flg = 0;
    sop.sem_num = 0;
    sop.sem_op = 1;
    if (semop(s, &sop, 1) < 0)
    {
        fprintf(stderr, "releaseandunlock:semop faild: %s\n", strerror(errno));
        return -1;
    }
    else
        return 0;
}