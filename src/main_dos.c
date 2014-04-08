#define	MSDOS
#include	"csapp.h"
#include	"water.h"


/******************主函数*************************/
int main(int argc, char *argv[])
{
	int listenfd, port, connfd;
	int clientlen = sizeof(struct sockaddr_in);
	struct sockaddr_in clientaddr;

	void *dbcon = DB_init();

	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)
	{
		printf("socket initial error!\n");
		exit(-1);
	}

	if(argc != 2)
	{
		printf("usgage: %s <port>", argv[0]);
		exit(0);
	}
	/*******监听socket接口*********/	
	port = atoi(argv[1]);
	
	listenfd = Open_listenfd(port);
	
	DB_real_connect(dbcon);

	while (1)
	{
		connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
		protocol(connfd, dbcon);
		Close(connfd);
	}
	DB_close(dbcon);

	if(WSACleanup() == SOCKET_ERROR)
	{
		printf("WSACLeanup failed withe error %d \n", WSAGetLastError());
	}
}




