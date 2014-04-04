
/************************* DB 调用函数**************/
#include 	<string.h>
#include 	"water.h"


void *DB_init()
{
	return NULL;
}

void DB_close(void *con)
{
}

void DB_real_connect(void *dbcon)
{

}


/***********7000:查询用户信息*************/
/*输入:用户水费户号*/
/*输出:响应报文(包含用户信息)*/	
int do7000(Send *S, struct user *p, Recv *R ,void *c)
{
	return 0;
}

/***********7001:用户签约确认*************/
/*输入:用户水费户号	+	用户银行帐号	*/
/*输出:响应报文					*/	
//7001业务之前
int do7001(Send *S, struct user *p, Recv *R, void *c)
{
		return 0;
}

/***********7002:用户签约修改*************/
/*输入:用户水费户号	+	用户银行帐号	*/
/*输出:响应报文					*/	
int do7002(Send *S, struct user *p, Recv *R, void *c)
{
		return 0;
}

/***********7003:用户签约删除*************/
//输入:用户水费户号			
//输出:响应报文  		银行帐号至为空串
int do7003(Send *S, struct user *p, Recv *R, void *c)
{
		return 0;
}

/***************************缴费信息查询***********/
int do7004(Send *S, struct user *p, Recv *R, void *c)
{
		return 0;
}

/******************************缴费确认******************/
//7005业务主要考虑错误处理逻辑
int do7005(Send *S, struct user *p, Recv *R, void *c)
{
		return 0;
}

//冲正业务
int do7006(Send *S, struct user *p, Recv *R, void *c)
{
		return 0;
}

//单笔缴费对账
int do7007(Send *S, char *date, int count, float sum, Recv *R, void *c)
{
		return 0;
}

int do7008(Send *S, Recv *R, void *c)
{
		return 0;
}

int do7009(struct user *p, int res, void *c)
{
		return 0;
}
