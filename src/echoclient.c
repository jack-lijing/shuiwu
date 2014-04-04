#include "csapp.h"

int main(int argc, char *argv[])
{
	char *host;
	int port, clientfd;
	char buf[MAXLINE];
	rio_t rp;

	if( argc != 3)
	{
		printf("usage:%s <hostname> <port>\n",argv[0]);
		exit(0);
	}

	host = argv[1];
	port = atoi(argv[2]);
	clientfd = Open_clientfd(host, port);
	Rio_readinitb(&rp, clientfd);
	while(Fgets(buf, MAXLINE, stdin) != NULL)
	{
		Rio_writen(clientfd, buf, strlen(buf));
		Rio_readlineb(&rp, buf, MAXLINE);
		Fputs(buf, stdout);
	}
	Close(clientfd);
	exit(0);
}
