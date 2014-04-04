#!/bin/bash

[ "$#" -lt 2 ] && echo "The Usage of Test:  \
		testcli 0 $用户帐号				用户基本信息查询 
		testcli 1 $用户帐号 $银行卡					银行扣款协议确认\n            
		testcli 2 $用户帐号 $银行卡					行扣款协议修改 
		testcli 3 $用户帐号 $银行卡					银行扣款协议删除\n     
		testcli 4 $用户帐号 						缴费信息查询
		testcli 5 $用户帐号 						缴费确认\n                
		testcli 6 冲正  7 单笔缴费对帐\n    "      
i=0;
hh=$2
user=${hh:-11111111111}
nzh=$3
bank=${nzh:-22222222222222222222}
echo "./testcli $1 $user $bank"
./testcli $1 $user $bank
#while [ "$i" != "10" ]
#do
#	./testcli $i $u $b
#	i=$(($i+1))
#done
#include "csapp.h"
//#ifndef __CSAPP_H__
#define __CSAPP_H__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <setjmp.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*Misc constants*/
#define RIO_BUFSIZE 8192
#define MAXLINE 8192
#define LISTENQ 1024

// Persistent state for the  robust I/O package
typedef struct 
{
	int rio_fd;
	int rio_cnt;  // unread bytes in internal buf
	char *rio_bufptr;
	char rio_buf[RIO_BUFSIZE];
}rio_t;


typedef struct sockaddr SA;
extern char **environ;

/*Our own error-handling functions*/
void unix_error(char *msg);
void posix_error(int code, char *msg);
void dns_error(char *msg);
void app_error(char *msg);


/*Process control wrapper*/
pid_t Fork(void);
void Execve(const char*filename, char *const argv[], char *const envp[]);
pid_t Wait(int *status);
pid_t Waitpid(pid_t pid, int *iptr, int options);
void Kill(pid_t pid, int signum);
unsigned int Sleep(unsigned int seconds);
void Pause(void);
unsigned int Alarm(unsigned int seconds);
void Setpgid(pid_t pid, pid_t pgid);
pid_t Getpgrp();

/*Signal wrapper*/
typedef void handler_t(int);
handler_t *Signal(int signum, handler_t *handler);
void Sigfillset(sigset_t *set);
void Sigaddset(sigset_t *set, int signum);
void Sigdelset(sigset_t *set, int signum);
void Sigemptyset(sigset_t *set);
void Sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
int Sigismember(const sigset_t *set, int signum);

/*Unix I/O wrappers*/
int Open(const char *pathname, int flags, mode_t mode);
ssize_t Read(int fd, void *buf, size_t count);
ssize_t Write(int fd, const void *buf, size_t count);
off_t Lseek(int fildes, off_t offset, int whence);
void Close(int fd);
int Select(int n,fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
int Dup2(int oldd, int newd);
void Stat(const char *filename, struct stat *buf);
void Fstat(int fd, struct stat *buf);

/*Memory mapping wrappers*/
void *Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);
void Munmap(void *start, size_t length);

/*Standard I/O wrappers*/
void Fclose(FILE *fp);
FILE *Fdopen(int fd, const char *type);
char *Fgets(char *ptr, int n, FILE *stream);
FILE *Fopen(const char *filename, const char *mode);
void Fputs(const char *ptr, FILE *stream);
size_t Fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
void Fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

/*Dynamic storage allocation wrappers*/
void *Malloc(size_t size);
void *Realloc(void *ptr, size_t size);
void *Calloc(size_t number, size_t size);
void Free(void *ptr);

/* Sockets interface wrappers */
int Socket(int domain, int type, int protocol);
void Setsockopt(int s, int level, int optname, const void *optval, int optlen);
void Bind(int sockfd, struct sockaddr *my_addr, int addlen);
void Listen(int s, int backlog);
int Accept(int listenfd, struct sockaddr *addr, int *addrlen);
void Connect(int sockfd, struct sockaddr *serv_addr, int addrlen);

// Rio (Robust I/O) package
ssize_t rio_writen(int fd, void *buf, size_t n);
ssize_t rio_readn(int fd, void *buf, size_t n);
void rio_readinitb(rio_t *rp, int fd);
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n);


/** Wrappers for Rio package*/
ssize_t Rio_writen(int fd, void *buf, size_t n);
ssize_t Rio_readn(int fd, void *buf, size_t n);
void Rio_readinitb(rio_t *rp, int fd);
ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);
ssize_t Rio_readnb(rio_t *rp, void *usrbuf, size_t n);


/***********************netlink***************************/
struct hostent *Gethostbyname(const char *hostname);
struct hostent *Gethostbyaddr(const char *addr, int len, int type);

//