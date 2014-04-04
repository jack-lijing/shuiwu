#ifndef __CSAPP_H__
#define __CSAPP_H__
#endif

#define	LINUX 1
#define	MSDOS 2
#define SYSTEM	LINUX

#if	SYSTEM == MSDOS
	#include <winsock.h>
#elif	SYSTEM == LINUX
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <errno.h>
	#include <unistd.h>
	#include <netdb.h>
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <setjmp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <math.h>

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
void app_error(char *msg);
void Close(int connfd);
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

