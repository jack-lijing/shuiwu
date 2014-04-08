
/************************* DB 调用函数**************/
#include 	<string.h>
#include	<mysql.h>
#include	<my_global.h>
#include 	"water.h"

void finish_with_error(MYSQL *con)
{
		fprintf(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		exit(1);
}

void *DB_init()
{
	MYSQL *dbcon = mysql_init(NULL);
	if(dbcon == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(dbcon));
		exit(1);
	}
	return dbcon;
}

void DB_close(void *con)
{
	mysql_close((MYSQL *)con);
}

void DB_real_connect(void *dbcon)
{
	if(mysql_real_connect((MYSQL *)dbcon, "localhost", "hutc", "hutc", "test",0, NULL, 0) == NULL)
	{
		finish_with_error(dbcon);
	}
}

int Mysql_query(MYSQL *con, char *SQL)
{
	if(mysql_query(con, SQL))
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		return ERRORDB;
	}
	return	SUCCESS;
}

MYSQL_RES* Mysql_store_result(MYSQL *con)
{
	MYSQL_RES *result = mysql_store_result(con);
	if(result == NULL)
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		//return ERRORDB;
	}

	return result;

}

/***********7000:查询用户信息*************/
/*输入:用户水费户号*/
/*输出:响应报文(包含用户信息)*/	
int do7000(Send *S, struct user *p, Recv *R ,void *c)
{
	MYSQL *con = (MYSQL *)c;
	char SQL[256];
	sprintf(SQL,"SELECT Xm,Dz FROM zlsdab Where Hh = %s", p->pwater->account);
	Mysql_query(con, SQL);
	MYSQL_RES	*result = mysql_store_result(con);
	int 	num_fields 	= mysql_num_fields(result);
	MYSQL_ROW 	row 	= mysql_fetch_row(result);

	if(row == 0)
		S->result = ERRORUSER;
	else
	{
		strncpy(p->name, row[0], NAMELEN);
		strncpy(p->addr, row[1], ADDRLEN);
	}
	mysql_free_result(result);
	//组装响应报文主体
	sprintf(S->buffer + HEADLEN_30, "%-12.12s%-60.60s%-60.60s",  
					p->pwater->account ,
					p->name,
					p->addr);

}

/***********7001:用户签约确认*************/
/*输入:用户水费户号	+	用户银行帐号	*/
/*输出:响应报文					*/	
//7001业务之前
int do7001(Send *S, struct user *p, Recv *R, void *c)
{
	MYSQL *con = (MYSQL *)c;
	do7000(S, p, R, con);
	if(S->result == SUCCESS )
	{
		char SQL[256];
		sprintf(SQL,"UPDATE zlsdab SET NZH = %s Where Hh = %s", p->pbank->account, p->pwater->account);
		S->result = Mysql_query(con, SQL);
	}
}

/***********7002:用户签约修改*************/
/*输入:用户水费户号	+	用户银行帐号	*/
/*输出:响应报文					*/	
int do7002(Send *S, struct user *p, Recv *R, void *c)
{
	MYSQL *con = (MYSQL *)c;
	do7000(S, p, R, con);
	if(S->result == SUCCESS )
	{
		char SQL[256];
		sprintf(SQL,"UPDATE zlsdab SET NZH = %s Where Hh = %s", p->pbank->account, p->pwater->account);
		S->result = Mysql_query(con, SQL);
	}
}

/***********7003:用户签约删除*************/
//输入:用户水费户号			
//输出:响应报文  		银行帐号至为空串
int do7003(Send *S, struct user *p, Recv *R, void *c)
{
	MYSQL *con = (MYSQL *)c;
	do7000(S, p, R, con);
	if(S->result == SUCCESS )
	{
		char SQL[256];
		sprintf(SQL,"UPDATE zlsdab SET NZH = 0  Where Hh = %s", p->pwater->account);
		S->result = Mysql_query(con, SQL);
	}
}

/***************************缴费信息查询***********/
int do7004(Send *S, struct user *p, Recv *R, void *c)
{
	MYSQL *con = (MYSQL *)c;
	char SQL[1024];
	sprintf(SQL,"SELECT zlsdab.Hh, zlsdab.Xm, zlsdab.Dz, zlsqkb.QKrq,zlsqkb.Je, \
			(DATEDIFF( curdate(),zlsqkb.cnjqsrq ) * (select cnjsl from zlscnjcsb))\
			FROM zlsdab INNER JOIN zlsqkb ON zlsdab.Hh = zlsqkb.Hh \
			Where zlsdab.Hh = %s ORDER BY zlsqkb.qkrq", p->pwater->account);
	S->result = Mysql_query(con, SQL);
	if(S->result == ERRORDB)
	{
		return;
	}

	MYSQL_RES *result = Mysql_store_result(con);

	MYSQL_ROW row;
	int 	m = 0;
	//最多获取MONTHNUM个月的欠款,默认为12个月
	while((row = mysql_fetch_row(result)) != 0 && m < MONTHNUM)
	{
			strncpy(p->name, row[1], NAMELEN);
			strncpy(p->addr, row[2], ADDRLEN);
			strncpy(p->pwater->table[m].date, row[3], WDATELEN);
			p->pwater->table[m].money = atof(row[4]);
			p->pwater->table[m].tax = atof(row[5]);
			p->pwater->bill_total += p->pwater->table[m].money + p->pwater->table[m].tax;
			++m;
	}
	p->pwater->months = mysql_num_rows(result);
	mysql_free_result(result);

	//填充欠款总额信息
	sprintf(S->buffer + HEADLEN_30, SBODY7004, p->pwater->account, p->name,  p->addr, p->pwater->bill_total, 
	       p->pwater->months);

	//填充欠款详细信息
	char	*pbill		= S->buffer + HEADLEN_30 + BODYLEN7004;
	int i = 0;
	for(; i < m ; i++)
	{
		sprintf( pbill, BILLFORMAT,
				p->pwater->table[i].date, 
				p->pwater->table[i].money,
				p->pwater->table[i].tax
				);
		pbill += 30; 
	}

}

/******************************缴费确认******************/
//7005业务主要考虑错误处理逻辑
int do7005(Send *S, struct user *p, Recv *R, void *c)
{
	MYSQL *con = (MYSQL *)c;
	char SQL[1024];
	//先锁上,防止水厂前台操作
	sprintf(SQL,"update zlsqkb set cnje = (DATEDIFF( curdate(),cnjqsrq ) * (select cnjsl from zlscnjcsb)), pkbz = 1 where hh = %s and pkbz = 0", p->pwater->account);
	Mysql_query(con, SQL);
	if(mysql_affected_rows(con) <= 0)
	{
		
		runlog("", 0, "此用户,或扣款被锁定\n");
		S->result = ERRORLOCK;
		return;
	}
	/*获取欠款账单信息并对账*/
	sprintf(SQL,"SELECT sum(je + cnje) From zlsqkb WHERE Hh = %s ", p->pwater->account);
	Mysql_query(con, SQL);
	MYSQL_RES *result = Mysql_store_result(con);
	MYSQL_ROW row;
	while((row = mysql_fetch_row(result)))
	{
		if( abs(p->pbank->table->money - atof(row[0])) > 0.01)
		{
			S->result = ERRORBOOK;
			//此处需要将pkbz还原
			sprintf(SQL,"update zlsqkb set cnje = (DATEDIFF( curdate(),cnjqsrq ) * (select cnjsl from zlscnjcsb)), pkbz = 0 where hh = %s ", p->pwater->account);
			Mysql_query(con, SQL);
			mysql_free_result(result);
			return;
		}
	}
	mysql_free_result(result);

	sprintf(SQL,"SELECT Hh,Qkrq,Je,Cnje,Cnjqsrq From zlsqkb WHERE Hh = %s", p->pwater->account);
	Mysql_query(con, SQL);
	result = Mysql_store_result(con);
	

	//注意回滚
	while((row = mysql_fetch_row(result)))
	{
		//插入缴费冲正表
		sprintf(SQL,"INSERT INTO zlsjfczb(hh,Qkrq,je,Cnje,Cnjqsrq,jfczrq,lsh) values(%s,%s,%.2f,%.2f,%s,%s,%s)",row[0],row[1],atof(row[2]),atof(row[3]),row[4],p->pbank->table->date,p->pbank->table->order);
		S->result = Mysql_query(con,SQL);
		if(S->result != SUCCESS)
			break;
	}
	mysql_free_result(result);

	
	//删除欠款表用户条目
	if(S->result == SUCCESS)
	{
		sprintf(SQL,"DELETE  From zlsqkb WHERE Hh = %s ", p->pwater->account);
		S->result = Mysql_query(con,SQL);
	}
	
	if(S->result != SUCCESS)
	{
	//	rollbackDB();		
	}
}

//冲正业务
int do7006(Send *S, struct user *p, Recv *R, void *c)
{
	MYSQL *con = (MYSQL *)c;
	char SQL[1024];
	/*获取缴费账单信息*/
	sprintf(SQL,"SELECT Hh,Qkrq,Je,Cnje,Cnjqsrq From zlsjfczb WHERE lsh = %s", p->pbank->table->order);
	Mysql_query(con,SQL);
	MYSQL_RES *result = Mysql_store_result(con);

	if(mysql_num_rows(result) == 0)
	{
		S->result = ERRORPAID;
		return;
	}
	MYSQL_ROW row ;
	while((row = mysql_fetch_row(result)))
	{
		sprintf(SQL,"INSERT INTO zlsqkb(hh,qkrq,je,cnje,cnjqsrq,pkbz) values(%s,%s,%.2f,%.2f,%s,0)",row[0],row[1],atof(row[2]),atof(row[3]),row[4]);
		Mysql_query(con,SQL);
	}
	mysql_free_result(result);
	if(S->result == SUCCESS)
	{
		sprintf(SQL,"DELETE  From zlsjfczb WHERE lsh = %s", p->pbank->table->order);
		S->result = mysql_query(con,SQL);
	}

	if(S->result != SUCCESS)
	{
	//	rollbackDB();		
	}
}

//单笔缴费对账
int do7007(Send *S, char *date, int count, float sum, Recv *R, void *c)
{
	MYSQL *con = (MYSQL *)c;
	char SQL[1024];
	sprintf(SQL,"SELECT Hh,Je,(DATEDIFF(curdate(),cnjqsrq ) * (select cnjsl from zlscnjcsb)),lsh From zlsjfczb WHERE jfczrq = %s", date);

	S->result = Mysql_query(con,SQL);
	MYSQL_RES *result = Mysql_store_result(con);

	MYSQL_ROW row;
	float 	sum_db 	= 0 ;
	int	count_db = 0;
	while((row = mysql_fetch_row(result)))
	{
		count_db++;
		sum_db += atof(row[1]) + atof(row[2]);
		//暂时只对总帐
	}
	if(count_db != count || sum_db != sum)
		S->result = ERRORBOOK;
	mysql_free_result(result);
}

int do7008(Send *S, Recv *R, void *c)
{
	MYSQL *con = (MYSQL *)c;
	//先锁上,并计算滞纳金
	char SQLPK[1024] = "update zlsqkb INNER JOIN zlsdab on zlsqkb.hh =zlsdab.hh  set pkbz = 1,Cnje = (DATEDIFF(curdate(),zlsqkb.cnjqsrq ) * (select cnjsl from zlscnjcsb)) where zlsdab.nzh != 0";
	Mysql_query(con,SQLPK);

	char SQL[1024] = "SELECT zlsdab.Hh, zlsdab.Xm,zlsdab.Nzh,zlsqkb.Je,Cnje FROM zlsdab INNER JOIN zlsqkb ON zlsdab.Hh =  zlsqkb.Hh WHERE zlsdab.Nzh != 0";

	S->result = Mysql_query(con,SQL);
	MYSQL_RES *result = Mysql_store_result(con);

	MYSQL_ROW row;
	float 	sum 	= 0 ;
	int	count 	= mysql_num_rows(result);
	/********************生成文件****/
	int 	fd = 0;
	FILE 	*fp;
	char 	file[FILELEN] = {""};
	sprintf(file,"./8-%s", GetSysDate(2));
	if((fp = fopen(file, "a+")) == NULL)
	{
		S->result = ERRORFILE;
		return -1;
	}
	fd = fileno(fp);
	lockf(fd, F_LOCK, 0L);

	while((row = mysql_fetch_row(result)))
	{
		//滞纳金计算
		sum = atof(row[3]) + atof(row[4]);
		fprintf(fp, "%-12.12s%-60.60s%-20.20s%-12.2f\n",row[0], row[1], row[2], sum );
		sum = 0 ;
		//Rio_writen(scon,line,LINELEN);
	}
	lockf(fd, F_ULOCK, 0L);
	fclose(fp);
	mysql_free_result(result);
	strncat(S->file, file, FILELEN);
	/************文件生成完毕*****************************/
	S->filelen = count * LINE7008;
}

int do7009(struct user *p, int res, void *c)
{
	MYSQL *con = (MYSQL *)c;
	int e_one, e_two, e_three;
	char SQL[1024];
	//扣款不成功则将批扣字段回复至0
	if(res != 0)
	{
		sprintf(SQL,"update zlsqkb  set pkbz = 0 where hh = %s", p->pwater->account);
		Mysql_query(con,SQL);
		return;
	}
	sprintf(SQL,"SELECT Hh,Qkrq,Je,Cnje,cnjqsrq From zlsqkb WHERE Hh = %s ", p->pwater->account);
	e_one = Mysql_query(con,SQL);
	MYSQL_RES *result = Mysql_store_result(con);

	MYSQL_ROW row;
	if(row == 0)
		return;
	while((row = mysql_fetch_row(result)))
	{
		sprintf(SQL,"INSERT INTO zlsjfczb(hh,qkrq,je,cnje,jfczrq,lsh,cnjqsrq) values(%s,%s,%.2f,%.2f,%s,%s,%s)",row[0],row[1],atof(row[2]),atof(row[3]),p->pbank->table->date,"0",row[4]);
		printf("%s\n",SQL);
		e_two = Mysql_query(con,SQL);
	}
	mysql_free_result(result);
	sprintf(SQL,"DELETE  From zlsqkb WHERE Hh = %s", p->pwater->account);
	e_three = Mysql_query(con,SQL);

	if(e_one || e_two || e_three)
	{
	//	rollbackDB();		
		//a->result = ERRORDB;
	}
}
