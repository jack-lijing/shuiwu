#define DEBUG

#define	FILELEN	20
#define	NAMELEN	60
#define	ADDRLEN	60
#define	WACCLEN	12
#define	BACCLEN	20
#define	ORDELEN	16
#define	DATE	12
#define	BILLTOT	12
#define	COUNTLEN	4
#define	HEADLEN_30	30	//交易报文头长度
#define	HEADLEN_26	26	//交易报文头长度
#define	MONTHNUM	12
#define	WDATELEN	6
#define	LINE7008	105	

#define ERRORLOCK	6
#define	ERRORPAID	5	//无此银行流水号
#define	ERRORBOOK	4
#define	ERRORDB		3
#define	ERRORFILE	2
#define	ERRORUSER	1
#define	SUCCESS		0



#define	SLEN7000	176	//7000业务响应报文长度#
#define	BODYLEN7004	 WACCLEN + NAMELEN + ADDRLEN + BILLTOT +COUNTLEN

#define SENDHEAD	"%-4.4d%-4.4d%-4.4d%-18.18d"	
#define SBODY7000	"%-12.12s%-60.60s%-60.60s"	
#define SBODY7004	"%-12.12s%-60.60s%-60.60s%-12.2f%-4.4d"	

#define RFORMAT7000	"%12s"
#define RFORMAT7001	"%12s%60s%20s"
#define RFORMAT7002	"%12s%60s%20s"
#define RFORMAT7003	"%12s%60s%20s"
#define RFORMAT7004	"%12s"
#define RFORMAT7005	"%12s%60s%60s%12f%16s%8s"
#define	RFORMAT7006	"%12s%60s%60s%12f%16s%8s"
#define RFORMAT7007	"%8s%6d%12f"
#define RFORMAT7008	
#define RFORMAT7009	

#define	BILLFORMAT	"%-6.6s%-12.2f%-12.2f"
#define	RECVHEAD	"%4d%4d%22d"		//报文头格式
#define	BUFLEN		2056	
//接受报文格式
typedef struct	{
	char	buffer[BUFLEN];			//接受区缓存
	int	len;				//接受报文长度,比实际长度-4
	int	code;				//接受报文编码
	int	filelen;			//记录附加文件长度
}Recv;

//发送报文结构体
typedef struct{
	char	buffer[BUFLEN];		//发送报文缓存区
	int	len;			//发送报文体长度(默认值为26--报文头长度),注:发送实际长度为 len + 4
	int	result;			//发送报文结果编码
	char	file[FILELEN];		//发送文本名称
	int	filelen;		//接受报文编码
}Send;

//	用户信息
struct	user{
	char	name[ NAMELEN + 1 ];			//姓名,可能有单位名称
	char	addr[ ADDRLEN + 1 ];			//地址
	struct	water	*pwater;		//水费账户
	struct	bank	*pbank;			//银行账户
};

/*账单信息:
	对于水费是应收款(以月份单位统计)
	对于银行是已收款(以单日日期统计)
 */
struct bill{
	float	money;				//账单金额
	float	tax;				//滞纳金
	char	date[7]; 			//账单日期
};


//水费账户信息
struct	water{
	char	account[WACCLEN + 1];			//账户号码
	float	bill_total;			//应收款总计
	int	months;				//应收款月份数目
	struct	bill	*table;			//应收月份明细
};

struct pay{
	float	money;					//账单金额
	char	order[ORDELEN + 1];				//银行流水号码,银行专用
	char	date[9]; 				//账单日期
};
//银行账户信息
struct	bank{
	char		account[BACCLEN];	//账户号码
	float		paid_total;		//已付款总数
	struct		pay	*table;		//当日已付账单明细
};


/***********************************************************************/
/*			数据库调用封装API				*/
/***********************************************************************/
//SQLSERVER数据方法如无此API则返回NULL
void *DB_init();
void DB_close(void *con);
void DB_real_connect(void *dbcon);

/*******	以下部分为数据层业务调用API		****************/
/*	
输入:
	用户水费账户号码puser->pwater->account
输出:
	填充puser用户姓名,地址, 操作结果;
 */
int do7000(Send *S, struct user *p, Recv *R ,void *c);
int do7001(Send *S, struct user *p, Recv *R, void *c);
int do7002(Send *S, struct user *p, Recv *R, void *c);
int do7003(Send *S, struct user *p, Recv *R, void *c);
int do7004(Send *S, struct user *p, Recv *R, void *c);
int do7005(Send *S, struct user *p, Recv *R, void *c);
int do7006(Send *S, struct user *p, Recv *R, void *c);
int do7007(Send *S, char *date, int count, float sum, Recv *R, void *c);
int do7008(Send *S, Recv *R, void *c);
int do7009(struct user *p, int res, void *c);
/*	
输入:
	用户水费账户号码puser->pwater->account
输出:
	如果成功return 0, 并填充puser用户银行信息:银行帐号等
	如果出错return -1
 */

void 	protocol(int connfd, void *con);
