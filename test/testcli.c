#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <fcntl.h>
#include <netdb.h>

#define  closesocket            close

typedef struct  {
  struct sockaddr_in sad;
  int    protono;
}TCPPARMS;

/*==============================================================================
                           main
                          ������
 ����������������������������������������������������������������������������
  ����:    ��
  ������:  ��
==============================================================================*/
main(int argc,char *argv[])
{
	int     sd,j;             /* socket descriptor */
  	char    ydport[10];
	char    ydhost[40];
	char	sendbuf[1024];
	char	buf[1024];
	char	recvbuf[1024];
	int	len;
  	TCPPARMS tcpp;
        
	int retCode;
	
	memset(sendbuf,0,sizeof(sendbuf));
	memset(recvbuf,0,sizeof(recvbuf));
	memset(buf,0,sizeof(buf));
	      
	if(argc<2)
	{
		printf("testcli 0 �û�������Ϣ��ѯ 1 ���пۿ�Э��ȷ��\n");
		printf("testcli 2 ���пۿ�Э���޸� 3 ���пۿ�Э��ɾ��\n");
		printf("testcli 4 �ɷ���Ϣ��ѯ 5 �ɷ�ȷ��\n");
		printf("testcli 6 ����  7 ���ʽɷѶ���\n");
		printf("testcli 8 �����ۿ���Ϣ��ѯ 9 �����ۿ���Ϣ����\n");
		fflush(stdout);
		exit(0);
	}
	switch(atoi(argv[1]))
	{
	case 0:
		sprintf(sendbuf,"%-4.4s%-4.4s%-4.4s%-18.18s%-12.12s","0038","7000","","",argv[2]);
		break;
	case 1:
		sprintf(sendbuf,"%-4.4s%-4.4s%-4.4s%-18.18s%-12.12s%-60.60s%-20.20s","0136","7001","","",argv[2],"����",argv[3]);
		break;
	case 2:
		sprintf(sendbuf,"%-4.4s%-4.4s%-4.4s%-18.18s%-12.12s%-60.60s%-20.20s","0136","7002","","",argv[2],"����",argv[3]);
		break;
	case 3:
		sprintf(sendbuf,"%-4.4s%-4.4s%-4.4s%-18.18s%-12.12s%-60.60s%-20.20s","0136","7003","","",argv[2],"����",argv[3]);
		break;
	case 4:
		sprintf(sendbuf,"%-4.4s%-4.4s%-4.4s%-18.18s%-12.12s","0038","7004","","",argv[2]);
		break;
	case 5:
		sprintf(sendbuf,"%-4.4s%-4.4s%-4.4s%-18.18s%-12.12s%-60.60s%-60.60s%-12.12s%-16.16s%-8.8s","0212","7005","","",argv[2],"����","֯����ũ��","264.50","1000000000000001","20140308");
		break;
	case 6:
		sprintf(sendbuf,"%-4.4s%-4.4s%-4.4s%-18.18s%-12.12s%-60.60s%-60.60s%-12.12s%-16.16s%-8.8s","0212","7006","","",argv[2],"����","֯����ũ��","101.01","1000000000000001","20140308");
		break;
	case 7:
		sprintf(sendbuf,"%-4.4s%-4.4s%-4.4s%18.18s%-8.8s%-6.6s%-12.12s\
%-12.12s%-60.60s%-60.60s%-12.12s%-16.16s%-8.8s\n%-12.12s%-60.60s%-60.60s%-12.12s%-16.16s%-8.8s\n",
				"0052","7007","","374","20140325","1","12.21"
				,"11111111111","����","֯����ũ��","101.01","1000000000000001","20140308"
				,"22222222222","����","֯����ũ��","233.01","1000000000000001","20140308");
		break;
	case 8:
		sprintf(sendbuf,"%-4.4s%-4.4s%-4.4s%-18.18s","0038","7008","","");
		break;
	case 9:
		sprintf(sendbuf,"%-4.4s%-4.4s%-4.4s%-18.18s","0038","7009","","111.txt");
		break;
	default:
		printf("�޴˽���\n");
		exit(0);
	}
        printf("Send %d bytes[%s]\n", strlen(sendbuf), sendbuf);
runlog(__FILE__,__LINE__,"sendbuf=[%s]\n",sendbuf);
	//���÷����ip��ַ���˿ں�
	//strcpy(ydhost,"172.20.7.61");
	//strcpy(ydhost,"127.0.0.1");
	//strcpy(ydport,"9005");
	strcpy(ydhost,"172.20.8.206");
	strcpy(ydport,"443");
        if(tcp_init(atoi(ydport),ydhost,&tcpp) < 0){
        	printf("TCP��ʼ��ʧ��!\n");
        	return -1;
        };
        if((sd = tcp_connect(&tcpp)) < 0){
              printf("TCP����ʧ��!\n");
	      closesocket(sd);
              return -1;
        }
       	if(writen(sd,sendbuf,sizeof(sendbuf)) < sizeof(sendbuf)){
              printf("TCP���ͱ���ʧ��!\n");
              closesocket(sd);
              return -1;
        }

	char recvlen[4]	= {""};
        if(readn(sd,recvlen,4)<0){/*ʧ��*/
              printf("TCP���ձ���ʧ��![%d]\n",len);
              closesocket(sd);
              return -1;
        };
	len = atoi(recvlen);
        if(readn(sd,recvbuf,len)<0){/*ʧ��*/
              printf("TCP���ձ���ʧ��![%d]\n",len);
              closesocket(sd);
              return -1;
	}
	closesocket(sd);
	printf("Receive %d bytes [%s%s]\n", strlen(recvbuf)+4 , recvlen, recvbuf);
	runlog(__FILE__,__LINE__,"recvbuf=[%s%s]\n", recvlen, recvbuf);
	return 0;

} /*main*/

/*==============================================================================
                      init_tcp
 ����������������������������������������������������������������������������
  ����:
  ������:  
==============================================================================*/
int tcp_init(int port,char *hostNameOrIP,TCPPARMS *tcpp)
 {
  struct protoent  *ptrp;
  struct hostent   *ptrh;
  struct sockaddr_in sad;

	if (port < 0 ) return -1;
    	memset((char *)&sad,0,sizeof(sad));

    /**** Map TCP tranbsport protocol name to protocol number*****/
    	if (((ptrp= getprotobyname("tcp"))) == NULL) {
		perror("���ܽ�\"tcp\"ӳ�䵽Э���");
		return (-1);
    		}

    /****** Convert host name to equivalent IP address******/
    	if( ( (char *) (ptrh = gethostbyname(hostNameOrIP))) != NULL)
       			memcpy(&sad.sin_addr, ptrh->h_addr, ptrh->h_length);
    	else
       		sad.sin_addr.s_addr = inet_addr(hostNameOrIP);

    /*** ��������������*************/
    	sad.sin_family = AF_INET;  /* set family to Internet */
    	sad.sin_port   = htons((u_short)port);/*set remotehost listen port*/
    	memcpy(&sad.sin_addr, ptrh->h_addr, ptrh->h_length);
    	memcpy(&(tcpp->sad) ,&sad,sizeof(sad));
    	tcpp->protono = ptrp->p_proto;
    	return 0;
	}

int  tcp_connect(TCPPARMS *tcpp)
{
    int sd,csize;
    csize = sizeof(struct sockaddr_in);
    /* ��������� */
    if((sd=socket(PF_INET,SOCK_STREAM,tcpp->protono)) < 0){
           return(-1);;
    }
   /* �������� */
  if(connect(sd,(struct sockaddr *) &(tcpp->sad),csize)< 0) {
          close(sd);
          return(-2);
  }
  return sd;
}

/*==============================================================================
                     readn
                 ��TCP�׽ӿڶ���
 ����������������������������������������������������������������������������
  ����:     �׽ӿ�        sd         int
            ���ջ�����    vptr       char *
            ���ճ���      n          int
  ������:  < 0; ʧ�� , >=0,���յ���ʵ�ʳ��� 
==============================================================================*/

readn(int sd,char *vptr,int n)
{
int	nleft;
int	nread;
char	*ptr;

	ptr=vptr;
	nleft=n;
	while(nleft>0){
		if((nread=read(sd,ptr,nleft))<0){
			if(errno==EINTR)
					nread=0;
			else
				return(-1);
			}
		else if(nread==0)break;
		nleft-=nread;
		  ptr+=nread;
		}
	return(n-nleft);
}

/*==============================================================================
                   writen
                 д��TCP�׽ӿ�
 ����������������������������������������������������������������������������
  ����:     �׽ӿ�        sd         int
            д�뻺����    vptr       char *
            д�볤��      n          int
  ������:  < 0; ʧ�� , >=0,���յ���ʵ�ʳ��� 
==============================================================================*/
writen(int sd,char *vptr,int n)
{
int	nleft;
int	nwritten;
char	*ptr;
int	total;

	ptr=vptr;
	nleft=n;
	total=0;
	while(nleft>0){
		if((nwritten=write(sd,ptr,nleft))<=0){
			if(errno==EINTR)nwritten=0;
			else return(-1);
			}
		total+=nwritten;
		nleft-=nwritten;
		ptr+=nwritten;
		}
	return(total);
}
			
