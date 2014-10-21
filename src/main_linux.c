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
	listenfd = Open_listenfd(port);

	/*******�������ݿ�************/
	void *dbcon = DB_init();
	DB_real_connect(dbcon);

	while (1)
	{
		connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
	/********************��ʼ���洢�ṹ****/
		Recv 	R 	= {{""}, 0, 0, 0, connfd};
		Send	S	= {{""}, HEADLEN_26 , SUCCESS, "", 0, connfd};
		memset( R.buffer, 0 , sizeof(R.buffer));
		memset( S.buffer, 0 , sizeof(S.buffer));
		memset( S.file, 0 , sizeof(S.file));
	
	//	Rio_readinitb(&rio, connfd);

	//	int n = Rio_read(&rio, recv, MAXLINE );
		if(!readreq(connfd, R.buffer))
		{
			doreq(&R, &S, dbcon);
			writeresp(&R, &S);
		}
		Close(connfd);
	}
	DB_close(dbcon);
}

