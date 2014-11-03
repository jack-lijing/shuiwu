#include	"csapp.h"
#include	"water.h"

int	readreq(int con, char *recv)
{
	if( Rio_readn(con, recv, 4) != 4)
	{
#ifdef DEBUG
		printf("server receive  bytes < 4 bytes\n");
#endif
		return 1;
	}
	int len = atoi(recv);
	Rio_readn(con, recv + 4, len);
#ifdef DEBUG
	printf("server receive %d bytes\n%s\n" , strlen(recv), recv);
#endif
	runlog("", 0, "Recv=[%s]\n", recv);
	return 0;
}

int	doreq(Recv *R, Send *S, void *dbcon)
{
	struct 	water 	winfo	= { "", 0.0, 0, Calloc(MONTHNUM,sizeof(struct bill) )} ;			//ˮ���˻�/
	struct 	bank 	binfo	= { "", 0.0, Calloc(1,sizeof(struct pay))};					//�����˻� 
	struct 	user 	person 	= { "", "", &winfo, &binfo };							//�û���Ϣ
	char 	*pbrecv;
	char 	buf[ HEADLEN_30 +1];

	sscanf(R->buffer, RECVHEAD, &(R->len), &(R->code), &(R->filelen));	//��䱨��ͷ�ṹ
	pbrecv = R->buffer + 30;
	switch(R->code){
		case	UQUERY:
			S->len = SLEN7000;
			sscanf(pbrecv, RFORMAT7000, person.pwater->account );	//��ȡ�û����
			do7000(S, &person, R, dbcon);
			break;
		case	SIGN:
			sscanf(pbrecv, RFORMAT7001, 
					person.pwater->account,
					person.name, 
					person.addr, 
					person.pbank->account );
			do7001(S, &person, R, dbcon);
			break;
		case	MODIFY:
			sscanf(pbrecv, RFORMAT7002, 
					person.pwater->account,
					person.name, 
					person.addr, 
					person.pbank->account );
			do7002(S, &person, R, dbcon);
			break;
		case	DELETE:
			sscanf(pbrecv, RFORMAT7003, 
					person.pwater->account,
					person.name, 
					person.addr, 
					person.pbank->account );
			do7003(S, &person, R, dbcon);
			break;
		case	BQUERY:
			sscanf(pbrecv, RFORMAT7004 , person.pwater->account);
			do7004(S, &person, R, dbcon);
		 	S->len = HEADLEN_26 + BODYLEN7004 + person.pwater->months*30;		 // 30 = 2(�·�) + 12(���) + 12(���ɽ�)
			break;
		case	BPAY:
			sscanf(pbrecv, RFORMAT7005, 
					person.pwater->account,
					person.name, 
					person.addr,
					&(person.pbank->table->money),
					person.pbank->table->order,
					person.pbank->table->date);
			do7005(S, &person, R, dbcon);
			break;
		case	BREDO:
			sscanf(pbrecv, RFORMAT7006, 
					person.pwater->account,
					person.name, 
					person.addr,
					&person.pbank->table->money,
					person.pbank->table->order,
					person.pbank->table->date);
			do7006(S, &person, R, dbcon);
			break;
		case	BDAILY:
			{
				char	date[9] = {""};
				int	count	= 0;
				float	sum	= 0;
				FILE 	*fp;
				char 	file[30] = {""};
				char 	usrbuf[110] = {""};	//110Ϊһ����¼�ĳ���,����\n + \0
				int 	num = R->filelen / 109;

				sscanf(pbrecv, RFORMAT7007, date,  &count, &sum);
				do7007(S, date, count, sum, R, dbcon);
				//����������ϸ�����˵�,��д���ı�
				

				sprintf(file,"./7-%s.txt", GetSysDate(1));
				if((fp = fopen(file, "w")) == NULL)
				{
					S->result = ERRORFILE;
					break;
			
				}


				while(num > 0 )
				{
					num--;
					//printf("server receive %d bytes %s\n",n,usrbuf);
					//Rio_writen(fileno(fp), usrbuf, Rio_readn(conn, usrbuf, sizeof(usrbuf)));
					Rio_readn(R->conn, usrbuf, sizeof(usrbuf)-1);
					fputs(usrbuf,fp);
				}
				fclose(fp);
			}
			break;
		case	BMONTHQ:
			do7008(S, R, dbcon);
			break;
		case	BMONTHP:
			{
				char 	line[115];
				int	res = 0;			//00�������пۿ�ɹ�,99����ۿ�ʧ��
				//��ȡÿһ�м�¼,��ۿ�ɹ�,��Ƿ����¼�����ɷѱ�
				while(Rio_readn(R->conn, line, sizeof(line)))
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

	sprintf(buf,SENDHEAD, S->len , R->code, S->result, S->filelen);
	memcpy(S->buffer, buf, HEADLEN_30);

	Free(person.pbank->table);
	Free(person.pwater->table);
}

int	writeresp(Recv *R, Send *S)
{
	Rio_writen(S->conn, S->buffer, S->len + 4);		//����ʵ�ʰ�����ΪS->len + 4

	#ifdef DEBUG
	printf("server send %d bytes(strlen)\n%s\n", strlen(S->buffer), S->buffer);
	#endif
	runlog("", 0, "Send=[%s]\n",S->buffer);

	/****** �����7008ҵ�񣬷��������ۿ���Ϣ ************************/
	if(R->code == BMONTHQ && strlen(S->file) > 0)
	{
		FILE 	*fp 	= fopen(S->file,"r");
		int	count = 0;
		char	lineb[LINE7008];
		count	= S->filelen / LINE7008;
		for (; count > 0; count --)
		{
			fread(lineb, LINE7008, 1, fp);
			Rio_writen(S->conn, lineb, LINE7008);
		}
	}
}
