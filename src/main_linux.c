#include	"csapp.h"
#include	"water.h"



/******************������*************************/
int main(int argc, char *argv[])
{
	int listenfd, port, *connfdp, connfd;
	int clientlen = sizeof(struct sockaddr_in);
	struct sockaddr_in clientaddr;
	pthread_t tid;

	if(argc != 2)
	{
		printf("usgage: %s <port>", argv[0]);
		exit(0);
	}
	/*******����socket�ӿ�*********/	
	port = atoi(argv[1]);
	port = 9005;
	listenfd = Open_listenfd(port);

	/*******�������ݿ�************/
	void *dbcon = DB_init();
	DB_real_connect(dbcon);

	while (1)
	{
		connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
		echo(connfd, dbcon);
		Close(connfd);
	}
	DB_close(dbcon);
}

