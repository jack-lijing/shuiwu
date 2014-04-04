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
#define  TCP_BLOCK     9056 

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
	if(argc < 2)
	{
		printf("testfile 8 批量扣款信息查询 9 批量扣款信息返回\n");
	}
		int     sd,j;             /* socket descriptor */
  	char    ydport[10];
	char    ydhost[40];
	char	sendbuf[1024];
	char	buf[1024];
	char	recvbuf[1024];
	char	filelen[18+1];
	int	len;
  	TCPPARMS tcpp;
        
	int retCode;
	
	memset(sendbuf,0,sizeof(sendbuf));
	memset(recvbuf,0,sizeof(recvbuf));
	memset(buf,0,sizeof(buf));

	char recvfile[] = { "7008recv" };
	char sendfile[] = { "7009send" };
	int i = 0;
	switch(atoi(argv[1]))
	{
		case 8:
			sprintf(sendbuf,"%-4.4s%-4.4s%-4.4s%-18.18s","0026","7008","","");
			break;
		case 9:
			i =  productext(recvfile, sendfile);
			sprintf(sendbuf,"%-4.4s%-4.4s%-4.4s%-18.18d","0026","7009","",i);
			break;
		default:
			exit(0);
	}
	      
	runlog(__FILE__,__LINE__,"sendbuf=[%s]\n",sendbuf);
	//设置服务端ip地址、端口号
	strcpy(ydhost,"localhost");
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
       	if(writen(sd,sendbuf,strlen(sendbuf)) < strlen(sendbuf)){
              printf("TCP发送报文失败!\n");
              closesocket(sd);
              return -1;
        }

	//接受文件
	switch(atoi(argv[1]))
	case 8:
	{
        	if(readn(sd,recvbuf,30)<0){/*失败*/
              		printf("TCP接收报文失败![%d]\n",len);
              		closesocket(sd);
              		return -1;
		}
		sprintf(filelen,"%18.18s",recvbuf+12);
		recvtext(sd,"7008recv",filelen);	
		break;
	case 9:
 		sendtext(sd, sendfile, i);
		break;
	}

	closesocket(sd);
runlog(__FILE__,__LINE__,"recvbuf=[%s]\n",recvbuf);
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

int productext(char *src, char *dest)
{
	FILE	*frp, *fwp;
	frp = fopen(src,"r");
	fwp = fopen(dest,"w");
	if(frp == NULL || fwp == NULL)
	{
		fprintf(stderr, "无法打开文件!\n");
		return;
	}
	int i;
	char buf[104];
	char a[11] = {"2014032800\n"};
	char ch = fgetc(frp);
	while(ch != EOF)
	{
		if(ch != '\n')
			fputc(ch, fwp);
		else
		{
			fwrite(a, 1, 11, fwp);
		}
		ch = fgetc(frp);
	}
	int len = ftell(fwp);
	fclose(frp);
	fclose(fwp);
	printf("文件长度:%d\n",len );
	return len;
}
			
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━*/
//	函数名:recvtext
//	  功能: 接收文本
//        参数: "sd" -软插座描述符  "name"--文件名  "len"--文件长度
//	  返回: '0'--成功  其它--失败
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━*/
 recvtext(int sd,char name[16],char len[20])
 {
 FILE 	*fp;
 char 	recvbuf[1280];
 int 	i,count,fsize;

  	fp=fopen(name,"w");
  	if(fp==NULL)return(-2); 
  	count=atoi(len)/TCP_BLOCK;
  	fsize=atoi(len)%TCP_BLOCK;
  	for(i=0;i<count;i++)
		{
  		readn(sd,recvbuf,TCP_BLOCK);
		fwrite(recvbuf,1,TCP_BLOCK,fp);
		}
  	if(fsize >0){
		readn(sd,recvbuf,fsize);	
		fwrite(recvbuf,1,fsize,fp);
		}
  	fclose(fp);
  	return 0;
 	}

/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━*/
//	函数名:sendtext
//	  功能: 发送文本
//        参数:sd--软插座描述符 fname--文件名
//	  返回:
/*━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━*/
 sendtext(int sd,char fname[16], int len)
 {
  FILE 	*fp;
  char 	sendbuf[1280];
  char 	buf[128];
  int 	i,count,fsize;

  	if((fp=fopen(fname,"r"))==NULL){
			printf("打开传送文件%s失败!",fname);
			fflush(stdout);
			return(-2); 
			}
  	for(i=0;;i++)
		{
		if(fread(sendbuf,1,len,fp)<1)break;
  		writen(sd,sendbuf,len);
		}
  	fclose(fp);
  	return 0;
 }

