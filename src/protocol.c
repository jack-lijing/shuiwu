#include	"csapp.h"
#include	"water.h"
void protocol(int conn, void *dbcon)
{
	/********************��ʼ���洢�ṹ****/
	Recv 	R 	= {{""}, 0, 0, 0};
	Send	S	= {{""}, HEADLEN_26 , SUCCESS, "", 0};
	struct 	water 	winfo	= { "", 0.0, 0, Calloc(MONTHNUM,sizeof(struct bill) )} ;			//ˮ���˻�/
	struct 	bank 	binfo	= { "", 0.0, Calloc(1,sizeof(struct pay))};					//�����˻� 
	struct 	user 	person 	= { "", "", &winfo, &binfo };							//�û���Ϣ
	char *pbrecv;
	char buf[ HEADLEN_30 +1];
	memset( R.buffer, 0 , sizeof(R.buffer));
	memset( S.buffer, 0 , sizeof(S.buffer));
	memset( S.file, 0 , sizeof(S.file));
	
	/*****	**************��ʼ������**********/

	/********	���뱨��ͷ����ʽ��	****/
	if (Rio_readn(conn, R.buffer, HEADLEN_30) < HEADLEN_30)		//����30���ȱ���ͷ
	{
		runlog("", 0, " the length of protocol <30 bytes, Error");
#ifdef DEBUG
		printf("the length of protocal  < 30 bytes, Error!\n");
#endif
		return;
	}

	sscanf(R.buffer, RECVHEAD, &R.len, &R.code, &R.filelen);	//��䱨��ͷ�ṹ

	/*******	���뱨����		*****/
	pbrecv = R.buffer + 30;				//ָ������
	Rio_readn(conn, pbrecv, R.len - 26);				//��ȡ��������
#ifdef DEBUG
	printf("server receive %d bytes\n%s\n" , strlen(R.buffer), R.buffer);
#endif
	runlog("", 0, "Recv=[%s]\n", R.buffer);

	switch(R.code){
		case	7000:
			S.len = SLEN7000;
			sscanf(pbrecv, RFORMAT7000, person.pwater->account );	//��ȡ�û����
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
		 	S.len = HEADLEN_26 + BODYLEN7004 + person.pwater->months*30;		 // 30 = 2(�·�) + 12(���) + 12(���ɽ�)
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
				FILE *fp;
				char file[30] = {""};
				char usrbuf[170] = {""};	//170Ϊһ����¼�ĳ���,����\n + \0
				int num = R.filelen / 169;

				sscanf(pbrecv, RFORMAT7007, date,  &count, &sum);
				do7007(&S, date, count, sum, &R, dbcon);
				//����������ϸ�����˵�,��д���ı�
				

				sprintf(file,"./7-%s.txt", GetSysDate(1));
				if((fp = fopen(file, "w")) == NULL)
				{
					S.result = ERRORFILE;
					break;
				}


				while(num > 0 )
				{
					num--;
					//printf("server receive %d bytes %s\n",n,usrbuf);
					//Rio_writen(fileno(fp), usrbuf, Rio_readn(conn, usrbuf, sizeof(usrbuf)));
					Rio_readn(conn, usrbuf, sizeof(usrbuf)-1);
					fputs(usrbuf,fp);
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
				int	res = 0;			//00�������пۿ�ɹ�,99����ۿ�ʧ��
				//��ȡÿһ�м�¼,��ۿ�ɹ�,��Ƿ����¼�����ɷѱ�
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

	//sprintf���ڽ�β+NULL, ��ʹ��memcpy������ͷ������Buffer��
	//PS sprintf��������ȫ,���ܷ������������,����ѡ��snprintf

	sprintf(buf,SENDHEAD, S.len , R.code, S.result, S.filelen);
	memcpy(S.buffer, buf, HEADLEN_30);


#ifdef DEBUG
	printf("server send %d bytes(strlen)\n%s\n", strlen(S.buffer), S.buffer);
#endif
	runlog("", 0, "Send=[%s]\n",S.buffer);
	Rio_writen(conn, S.buffer, S.len + 4);		//����ʵ�ʰ�����ΪS.len + 4

	/****** �����7008ҵ�񣬷��������ۿ���Ϣ ************************/
	if(strlen(S.file) > 0)
	{
		FILE 	*fp 	= fopen(S.file,"r");
		int 	fd 	= fileno(fp);
		rio_t	fio;
		char 	buf[MAXLINE];
		int 	nread;
		rio_readinitb(&fio, fd);
		while((nread = Rio_readlineb(&fio, buf, MAXLINE ))!=0)
			Rio_writen(conn, buf, nread);
	}
	Free(person.pbank->table);
	Free(person.pwater->table);
}
