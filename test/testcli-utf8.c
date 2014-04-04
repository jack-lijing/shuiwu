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
                          主函数
 ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  参数:    无
  返回码:  无
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
		printf("testcli 1 用户基本信息查询 2 银行扣款协议确认\n");
		printf("testcli 3 银行扣款协议修改 4 银行扣款协议删除\n");
		printf("testcli 5 缴费信息查询 6 缴费确认\n");
		printf("testcli 7 冲正  8 单笔缴费对帐\n");
		printf("testcli 9 批量扣款信息查询 10 批量扣款信息返回\n");
		fflush(stdout);
		exit(0);
	}
	switch(atoi(argv[1]))
	{
	case 1:
		sprintf(sendbuf,"%-4.4s%-4.4s%-4.4s%-18.18s%-12.12s","0038","7000","","","1234567890");
		break;
	case 2:
		sprintf(sendbuf,"%-4.4s%-4.4s%-4.4s%-18.18s%-12.12s%-18.18s%-60.60s%-20.20s","0136","7001","","","1234567890","330501190001011111","张三","6228480350123456789");
		break;
	case 3:
		sprintf(sendbuf,"%-4.4s%-4.4s%-4.4s%-18.18s%-12.12s%-18.18s%-60.60s%-20.20s","0136","7002","","","1234567890","330501190001011111","张三","6228480350123456788");
		break;
	case 4:
		sprintf(sendbuf,"%-4.4s%-4.4s%-4.4s%-18.18s%-12.12s%-18.18s%-60.60s%-20.20s","0136","7003","","","1234567890","330501190001011111","张三","6228480350123456788");
		break;
	case 5:
		sprintf(sendbuf,"%-4.4s%-4.4s%-4.4s%-18.18s%-12.12s","0038","7004","","","1234567890");
		break;
	case 6:
		sprintf(sendbuf,"%-4.4s%-4.4s%-4.4s%-18.18s%-12.12s%-60.60s%-18.18s%-60.60s%-12.12s%-16.16s%-8.8s","0212","7005","","","1234567890","张三","330500190001011111","织里镇农行","101.01","1000000000000001","20140308");
		break;
	case 7:
		sprintf(sendbuf,"%-4.4s%-4.4s%-4.4s%-18.18s%-12.12s%-60.60s%-18.18s%-60.60s%-12.12s%-16.16s%-8.8s","0212","7006","","","1234567890","张三","330500190001011111","织里镇农行","101.01","1000000000000001","20140308");
		break;
	case 8:
		sprintf(sendbuf,"%-4.4s%-4.4s%-4.4s%-18.18s%-8.8s%-6.6s%-12.12s","0038","7007","","","20140308","1","101.01");
		break;
	case 9:
		sprintf(sendbuf,"%-4.4s%-4.4s%-4.4s%-18.18s","0038","7008","","");
		break;
	case 10:
		sprintf(sendbuf,"%-4.4s%-4.4s%-4.4s%-18.18s","0038","7009","","111.txt");
		break;
	default:
		printf("无此交易\n");
		exit(0);
	}
runlog(__FILE__,__LINE__,"sendbuf=[%s]\n",sendbuf);
	//设置服务端ip地址、端口号
	strcpy(ydhost,"172.20.7.61");
	strcpy(ydport,"9005");
        if(tcp_init(atoi(ydport),ydhost,&tcpp) < 0){
        	printf("TCP初始化失败!\n");
        	return -1;
        };
        if((sd = tcp_connect(&tcpp)) < 0){
              printf("TCP连接失败!\n");
	      closesocket(sd);
              return -1;
        }
       	if(writen(sd,sendbuf,sizeof(sendbuf)) < sizeof(sendbuf)){
              printf("TCP发送报文失败!\n");
              closesocket(sd);
              return -1;
        }

	char recvlen[4]	= {""};
        if(readn(sd,recvlen,4)<0){/*失败*/
              printf("TCP接收报文失败![%d]\n",len);
              closesocket(sd);
              return -1;
        };
	len = atoi(recvlen);
        if(readn(sd,recvbuf,len)<0){/*失败*/
              printf("TCP接收报文失败![%d]\n",len);
              closesocket(sd);
              return -1;
	}
	closesocket(sd);
	printf("receive %d bytes [%s%s]\n", strlen(recvbuf)+4 , recvlen, recvbuf);
	runlog(__FILE__,__LINE__,"recvbuf=[%s%s]\n", recvlen, recvbuf);
	return 0;

} /*main*/

/*==============================================================================
                      init_tcp
 ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  参数:
  返回码:  
==============================================================================*/
int tcp_init(int port,char *hostNameOrIP,TCPPARMS *tcpp)
 {
  struct protoent  *ptrp;
  struct hostent   *ptrh;
  struct sockaddr_in sad;

	if (port < 0 ) return -1;
    	memset((char *)&sad,0,sizeof(sad));

    /**** Map TCP tranbsport protocol name to protocol number*****/
    	if (((int)(ptrp= getprotobyname("tcp"))) == 0) {
		perror("不能将\"tcp\"映射到协议号");
		return (-1);
    		}

    /****** Convert host name to equivalent IP address******/
    	if( ( (char *) (ptrh = gethostbyname(hostNameOrIP))) != NULL)
       			memcpy(&sad.sin_addr, ptrh->h_addr, ptrh->h_length);
    	else
       		sad.sin_addr.s_addr = inet_addr(hostNameOrIP);

    /*** 设置主机的属性*************/
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
    /* 建立软插座 */
    if((sd=socket(PF_INET,SOCK_STREAM,tcpp->protono)) < 0){
           return(-1);;
    }
   /* 建立联接 */
  if(connect(sd,(struct sockaddr *) &(tcpp->sad),csize)< 0) {
          close(sd);
          return(-2);
  }
  return sd;
}

/*==============================================================================
                     readn
                 从TCP套接口读出
 ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  参数:     套接口        sd         int
            接收缓冲区    vptr       char *
            接收长度      n          int
  返回码:  < 0; 失败 , >=0,接收到的实际长度 
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
                 写入TCP套接口
 ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
  参数:     套接口        sd         int
            写入缓冲区    vptr       char *
            写入长度      n          int
  返回码:  < 0; 失败 , >=0,接收到的实际长度 
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
			
