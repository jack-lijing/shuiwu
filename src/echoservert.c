#include	"csapp.h"
#include	"water.h"

void 	echo(int connfd, void *con);
void 	*thread(void *vargp);


/******************主函数*************************/
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
	/*******监听socket接口*********/	
	port = atoi(argv[1]);
	port = 9005;
	listenfd = Open_listenfd(port);

	/*******连接数据库************/
	void *dbcon = DB_init();
	DB_real_connect(dbcon);

	while (1)
	{
		//connfdp = Malloc(sizeof(int));
		//*connfdp = Accept(listenfd, (SA *)&clientaddr, &clientlen);
		//Pthread_create(&tid, NULL, thread, connfdp);
		connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
		echo(connfd, dbcon);
		Close(connfd);
	}
	DB_close(dbcon);
}


void echo(int conn, void *dbcon)
{
	/********************初始化存储结构****/
	Recv 	R 	= {{}, 0, 0, 0};
	memset( R.buffer, 0 , sizeof(R.buffer));
	Send	S	= {{}, HEADLEN_26 , SUCCESS, "", 0};
	memset( S.buffer, 0 , sizeof(S.buffer));
	memset( S.file, 0 , sizeof(S.file));
	struct 	water 	winfo	= { "", 0.0, 0, Calloc(MONTHNUM,sizeof(struct bill) )} ;			//水费账户/
	struct 	bank 	binfo	= { "", 0.0, Calloc(1,sizeof(struct pay))};					//银行账户 
	struct 	user 	person 	= { "", "", &winfo, &binfo };							//用户信息
	/*****	**************初始化结束**********/

	/********	读入报文头并格式化	****/
	if (Rio_readn(conn, R.buffer, HEADLEN_30) < HEADLEN_30)		//读入30长度报文头
	{
		runlog("", 0, "报文长度<30,出错");
		printf("报文头长度 < 30, 无此格式!\n");
		return;
	}
	char res[5];
	sscanf(R.buffer, RECVHEAD, &R.len, &R.code, &R.filelen);	//填充报文头结构

	/*******	读入报文体		*****/
	char 	*pbrecv = R.buffer + 30;				//指向报文体
	Rio_readn(conn, pbrecv, R.len - 26);				//读取报文主体
	printf("server receive %d bytes\n%s\n" , strlen(R.buffer), R.buffer);
	runlog("", 0, "Recv=[%s]\n", R.buffer);

	switch(R.code){
		case	7000:
			S.len = SLEN7000;
			sscanf(pbrecv, RFORMAT7000, person.pwater->account );	//获取用户编号
			do7000(&S, &person, &R, dbcon);
			break;
		case	7001:
			sscanf(pbrecv, RFORMAT7001, 
					person.pwater->account,
					person.name, 
					person.pbank->account );
			do7001(&S, &person, &R, dbcon);
			break;
		case	7002:
			sscanf(pbrecv, RFORMAT7002, 
					person.pwater->account,
					person.name, 
					person.pbank->account );
			do7002(&S, &person, &R, dbcon);
			break;
		case	7003:
			sscanf(pbrecv, RFORMAT7003, 
					person.pwater->account,
					person.name, 
					person.pbank->account );
			do7003(&S, &person, &R, dbcon);
			break;
		case	7004:
			sscanf(pbrecv, RFORMAT7004 , person.pwater->account);
			do7004(&S, &person, &R, dbcon);
		 	S.len = HEADLEN_26 + BODYLEN7004 + person.pwater->months*30;		 // 30 = 2(月份) + 12(金额) + 12(滞纳金)
			break;
		case	7005:
			sscanf(pbrecv, RFORMAT7005, 
					person.pwater->account,
					person.name, 
					person.addr,
					&(person.pbank->table->money),
					person.pbank->table->order,
					person.pbank->table->date);
			do7005(&S, &person, &R, dbcon);
			break;
		case	7006:
			sscanf(pbrecv, RFORMAT7006, 
					person.pwater->account,
					person.name, 
					person.addr,
					&person.pbank->table->money,
					person.pbank->table->order,
					person.pbank->table->date);
			do7006(&S, &person, &R, dbcon);
			break;
		case	7007:
			{
				char	date[9] = {""};
				int	count	= 0;
				float	sum	= 0;
				sscanf(pbrecv, RFORMAT7007, date,  &count, &sum);
				do7007(&S, date, count, sum, &R, dbcon);
				//接受银行详细对账账单,并写入文本
				FILE *fp;
				char file[30] = {""};
				sprintf(file,"./每日对账%s", GetSysDate(1));
				if((fp = fopen(file, "w")) == NULL)
				{
					S.result = ERRORFILE;
					break;
				}
				char usrbuf[187] = {""};	//187为一条记录的长度,包含\n
				int num = R.filelen / sizeof(usrbuf);
				while(num > 0 )
				{
					num--;
					int n = Rio_readn(conn, usrbuf, sizeof(usrbuf));
					//printf("server receive %d bytes %s\n",n,usrbuf);
					Rio_writen(fileno(fp), usrbuf, n);
				}
				fclose(fp);
			}
			break;
		case	7008:
			do7008(&S, &R, dbcon);
			break;
		case	7009:
			{
				char 	line[115];
				int	res = 0;			//00代表银行扣款成功,99代码扣款失败
				//读取每一行记录,如扣款成功,则将欠款表记录移至缴费表
				while(Rio_readn(conn, line, sizeof(line)))
				{
					sscanf(line, "%12s%60s%20s%12f%8s%2d", 
						person.pwater->account,
						person.name, 
						person.pbank->account,
						&person.pbank->table->money,
						person.pbank->table->date,
						&res);
					
					do7009(&person, res, dbcon);
					memset(line, 0 , sizeof(line));
				}
			}
			break;
			
		default:
			app_error("No this Code ,Please check");
			break;
	}

	//sprintf会在结尾+NULL, 故使用memcpy将报文头拷贝至Buffer中
	//PS sprintf函数不安全,可能发生缓冲区溢出,建议选用snprintf
	char buf[ HEADLEN_30 +1];
	snprintf(buf, HEADLEN_30+1 ,SENDHEAD, S.len , R.code, S.result, S.filelen);
	memcpy(S.buffer, buf, HEADLEN_30);


	printf("server send %d bytes(strlen)\n%s\n", strlen(S.buffer), S.buffer);
	runlog("", 0, "Send=[%s]\n",S.buffer);
	Rio_writen(conn, S.buffer, S.len + 4);		//发送实际包长度为S.len + 4

	/****** 如果是7008业务，发送批量扣款信息 ************************/
	if(strlen(S.file) > 0)
	{
		FILE 	*fp 	= fopen(S.file,"r");
		int 	fd 	= fileno(fp);
		rio_t	fio;
		rio_readinitb(&fio, fd);
		char 	buf[MAXLINE];
		int 	nread;
		while((nread = Rio_readlineb(&fio, buf, MAXLINE ))!=0)
			Rio_writen(conn, buf, nread);
	}
	Free(person.pbank->table);
	Free(person.pwater->table);

}

/*
void *thread(void *vargp)
{
	int connfd = *((int *) vargp);
	Pthread_detach(pthread_self());
	Free(vargp);
	echo(connfd);
	Close(connfd);
	return NULL;
}
*/

