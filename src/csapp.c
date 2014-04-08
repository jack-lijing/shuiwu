#include "csapp.h"

/*********************************************************
			Error-handling functions
*********************************************************/
void unix_error(char *msg)
{
	fprintf(stdout, "%s: %s", msg, strerror(errno));
	exit(0);
}

void app_error(char *msg)
{
	fprintf(stderr, "%s", msg);
	exit(0);
}

void Close(int connfd)
{
#ifdef MSDOS
	if (closesocket(connfd) == -1)
#endif
#ifdef	LINUX
	if (close(connfd) == -1)
#endif
	{
		unix_error("Close error");
	}
}

/*************************************************************
	* Sockets interface wrappers	*
**************************************************************/
int Socket(int domain, int type, int protocol)
{
	int rc;
	if((rc = socket(domain, type, protocol)) < 0)
		unix_error("Socket error");
	return rc;
}

void Setsockopt(int s, int level, int optname, const void *optval, int optlen)
{
	int rc;
	if((rc = setsockopt(s, level, optname, optval, optlen)) < 0)
		unix_error("Setsockopt error");
}

void Bind(int sockfd, struct sockaddr *my_addr, int addrlen)
{
	int rc;
	if((rc = bind(sockfd, my_addr, addrlen)) < 0)
		unix_error("Bind error");
}

int Accept(int listenfd, struct sockaddr *addr, int* addrlen)
{
	int rt;
	rt = accept(listenfd, addr, addrlen);
	if (rt == -1)
	{
		unix_error("Accept error");
	}
	return rt;
}

/********************************************************
	*The Rio package robust I/O functions
**********************************************************/

/* rio_readn robustly read n bytes(unbuffered)*/
ssize_t rio_readn(int fd, void * userbuf, size_t n)
{
	size_t nleft = n;
	ssize_t nread;
	char *bufp = userbuf;
	while( nleft > 0)
	{
#if defined(MSDOS)
		if ((nread = recv(fd , bufp, nleft, 0)) < 0 )
#elif defined(LINUX)
		if ((nread = read(fd , bufp, nleft)) < 0 )
#endif
		{
			if(errno == EINTR)
				nread = 0;
			else 
				nread = -1;
		}
		else if (nread == 0)
		{
			break;
		}
		nleft -= nread;
		bufp += nread;
	}
	return (n - nleft);
}

ssize_t Rio_readn(int fd, void *userbuf, size_t n)
{
	ssize_t rt;
	rt = rio_readn(fd, userbuf, n);
	if (rt == -1)
		unix_error("Rio_readn error");
	return rt;
}

/*	robustly write n bytes (unbuffered)	*/
ssize_t rio_writen(int fd, void *userbuf, size_t n)
{
	size_t nleft = n;
	ssize_t nwritten;
	char *bufp = userbuf;
	while( nleft >0 )
	{
#if defined(MSDOS)
		if((nwritten = send(fd, bufp, nleft, 0)) <= 0)
#elif	defined(LINUX)
		if((nwritten = write(fd, bufp, nleft)) <= 0)
#endif
		{
			if(errno == EINTR)
				nwritten = 0;
			else 
				return  -1;
		}
		nleft -= nwritten;
		bufp += nwritten;
	}
	return n;
}

ssize_t Rio_writen(int fd, void *userbuf, size_t n)
{
	ssize_t rt;
	rt = rio_writen(fd, userbuf, n);
	if (rt == -1)
		unix_error("Rio_writen error");
	return rt;
}



/*	Associate a descriptor with a read buffer and reset buffer*/
void rio_readinitb(rio_t *rp, int fd)
{
	rp->rio_fd = fd;
	rp->rio_cnt = 0;
	rp->rio_bufptr = rp->rio_buf;
}

void Rio_readinitb(rio_t *rp, int fd)
{
	rio_readinitb(rp, fd);
}

/*
 * rio_read - This is a wrapper for the Unix read() function that
 * transfers min(n,rio_cnt) bytes from an internal buffer to a user
 * buffer, where n is the number of bytes requested by the user and 
 * rio_cnt is the number if unread bytes in the internal buffer, On
 * entry, rio_read() refills the internal buffer via a call to
 * read() if the internal buffer is empty
*/
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
{
	int cnt;

	while(rp->rio_cnt <= 0)			/*refill if buf is empty*/
	{		
#if defined(MSDOS)
		rp->rio_cnt = recv(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf), 0 );
#elif	defined(LINUX)
		rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf) );
#endif
		if( rp->rio_cnt < 0)
		{
			if(errno != EINTR)			// interrupted by sig handler return	
				return -1;
		}
		else if(rp->rio_cnt == 0)		//EOF
			return 0;
		else
			rp->rio_bufptr = rp->rio_buf;	//reset buffer ptr
	}

	/*Copy min(n, rp->rio_cnt) bytes from internal buf to user buf*/
	cnt = n;
	if(rp->rio_cnt < n)
		cnt = rp->rio_cnt;
	//memcpy((void *)usrbuf, (const void *)rp->rio_bufptr, (size_t)cnt);
	memcpy(usrbuf, rp->rio_bufptr, cnt);
	rp->rio_bufptr += cnt;
	rp->rio_cnt -= cnt;
	return cnt;

}

static ssize_t Rio_read(rio_t *rp, char *usrbuf, size_t n)
{
	ssize_t rt;
	rt = rio_read(rp, usrbuf, n);
	if (rt == -1)
		unix_error("Rio_read error");
	return rt;
}

/*	robustly read a text line(buffered)	*/
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
	unsigned int n;
	int	rc;
	char c, *bufp = usrbuf;
	for ( n = 1; n < maxlen; n++)
	{
		if((rc = rio_read(rp, &c, 1)) == 1)
		{
			*bufp++ = c;
			if( c =='\n')
				break;
		}
		else if(rc == 0)
		{
			if(n == 1)
				return 0;
			else 
				break;
		}else
			return -1;
	}
	*bufp = 0;
	return n;
}

ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
	ssize_t rt;
	rt = rio_readlineb(rp, usrbuf, maxlen);
	if (rt == -1)
		unix_error("Rio_readlineb error");
	return rt;
}

/*	Robustly read n bytes(buffered)	*/
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n)
{
	int rc, nleft;
	char *bufp = usrbuf;
	nleft = n;
	while( nleft > 0)
	{
		if ((rc = rio_read(rp, bufp, nleft)) < 0)
		{
			if(errno != EINTR)
				return -1;
			else 
				continue;
		}
		else if (rc == 0)
		{
			return 0;
		}
		else
		{
			bufp += rc;
			nleft -= rc;
		}
	}
	return n - nleft;
}

ssize_t Rio_readnb(rio_t *rp, void *usrbuf, size_t n)
{
	ssize_t rt;
	rt = rio_readnb(rp, usrbuf, n);
	if (rt == -1)
		unix_error("Rio_readnb error");
	return rt;
}

/***************************************************
	*	Client/server helper functions	*
****************************************************/
//���ɹ���������������unix������-1����DNS������-2
int open_clientfd(char *hostname, int port)
{
	int clientfd;
	struct hostent *hp;
	struct sockaddr_in serv_in;

	clientfd = socket(AF_INET, SOCK_STREAM, 0);
	if(clientfd < 0)
		return -1;
	
	hp = gethostbyname(hostname);
	if(hp == NULL)
		return -2;
	memset(&serv_in, 0, sizeof(serv_in));
	memcpy((char*)hp->h_addr, (char*)&serv_in.sin_addr.s_addr, hp->h_length);
	serv_in.sin_family = AF_INET;
	serv_in.sin_port = htons((unsigned short)port);
	if( connect(clientfd, (struct sockaddr *)&serv_in, sizeof(serv_in)) < 0)
		return -1;
	else
		return clientfd;
}

int Open_clientfd(char *hostname, int port)
{
	int rt;
	rt = open_clientfd(hostname, port);
	if ( rt == -1 )
		unix_error("Open_clientfd unix error");
	if ( rt == -2)
		unix_error("Open_clientfd DNS error");
	return rt;
}

int open_listenfd(int port)
{
	int listenfd, optval = 1;
	struct sockaddr_in serveraddr;

	/**Create a socket descriptor*/
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
		return -1;

	/*Eliminates "Address already in use" error from bind*/
	if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (const void*)&optval, sizeof(int)) < 0)
		return -1;

	/**Listenfd will be an endpoint for all requests to port on any ip address for this host*/
	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short)port);

	if(bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
		return -1;

	/*Make it a listening socket ready to accept connection request*/
	if(listen(listenfd, LISTENQ) < 0)
		return -1;
	return listenfd;
}

int Open_listenfd(int port)
{
	int rt;
	rt = open_listenfd(port);
	if( rt == -1)
		unix_error("open_listenfd error");
	return rt;
}
/******************************************************/
/*Wrappers for dynamic storage allocation functions*/
/*****************************************************/
void *Malloc(size_t size)
{
	void *ptr;
	ptr = malloc(size);
	if( NULL == ptr)
		unix_error("Malloc error");
	return ptr;
}

void *Calloc(size_t number, size_t size)
{
	void *ptr;
	if( NULL == (ptr = calloc(number, size)))
		unix_error("Malloc error");
	return ptr;
}

void Free(void *ptr)
{
	free(ptr);
}

//-----------------���ߺ���---------------------------------/
/* ��ȡϵͳ���� i=0 ���� yyyymmdd i=1 ����yyyy-mm-dd i=2 ����yyyymmddhhmmss*/
char * getclock(void)
{
  static char buf[18];
  struct tm * p;
  time_t clck;

  clck = time((time_t *)0);
  p = localtime(&clck);
  sprintf(buf, "%04d/%02d/%02d-%02d:%02d:%02d",
          p->tm_year+1900, p->tm_mon + 1, p->tm_mday,
          p->tm_hour, p->tm_min, p->tm_sec);
  return buf;
}

char *GetSysDate(int i)
{
    time_t tm;
    static char date[15];
    
    tm = time(NULL);
	if (i==0) {
    strftime(date, 9, "%Y%m%d", localtime(&tm));
	date[8]=0;
	}
	if (i==1) {
    strftime(date, 11, "%Y-%m-%d", localtime(&tm));
	date[10]=0;
    }
	if (i==2) {
    strftime(date, 15, "%Y%m%d%H%M%S", localtime(&tm));
	date[14]=0;
    }
    return date;
}

int runlog(char * file, int line, char * msg, ...)
{
   va_list args;
   FILE * fp;
   int fd;
   char rq[8+1];
   char path[80], buff[512];


   va_start(args, msg);
   vsprintf(buff, msg, args);
   va_end(args);
   
   strcpy(rq, GetSysDate(0));

   sprintf(path, "./%s.log", rq+4);
   if ((fp = fopen(path, "a+")) == NULL)
      return -1;
   fd = fileno(fp);
#if defined(LINUX)
   lockf(fd, F_LOCK, 0L);
#endif
   fprintf(fp, "%s|%s|%d|%s\n", getclock(), file, line, buff);
#if defined(LINUX)
   lockf(fd, F_ULOCK, 0L);
#endif
   fclose(fp);
   return 0;
}
