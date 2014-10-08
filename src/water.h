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
#define	HEADLEN_30	30	//���ױ���ͷ����
#define	HEADLEN_26	26	//���ױ���ͷ����
#define	MONTHNUM	12
#define	WDATELEN	6
#define	LINE7008	105	

#define ERRORLOCK	6
#define	ERRORPAID	5	//�޴�������ˮ��
#define	ERRORBOOK	4
#define	ERRORDB		3
#define	ERRORFILE	2
#define	ERRORUSER	1
#define	SUCCESS		0



#define	SLEN7000	176	//7000ҵ����Ӧ���ĳ���#
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
#define	RECVHEAD	"%4d%4d%22d"		//����ͷ��ʽ
#define	BUFLEN		2056	
//���ܱ��ĸ�ʽ
typedef struct	{
	char	buffer[BUFLEN];			//����������
	int	len;				//���ܱ��ĳ���,��ʵ�ʳ���-4
	int	code;				//���ܱ��ı���
	int	filelen;			//��¼�����ļ�����
}Recv;

//���ͱ��Ľṹ��
typedef struct{
	char	buffer[BUFLEN];		//���ͱ��Ļ�����
	int	len;			//���ͱ����峤��(Ĭ��ֵΪ26--����ͷ����),ע:����ʵ�ʳ���Ϊ len + 4
	int	result;			//���ͱ��Ľ������
	char	file[FILELEN];		//�����ı�����
	int	filelen;		//���ܱ��ı���
}Send;

//	�û���Ϣ
struct	user{
	char	name[ NAMELEN + 1 ];			//����,�����е�λ����
	char	addr[ ADDRLEN + 1 ];			//��ַ
	struct	water	*pwater;		//ˮ���˻�
	struct	bank	*pbank;			//�����˻�
};

/*�˵���Ϣ:
	����ˮ����Ӧ�տ�(���·ݵ�λͳ��)
	�������������տ�(�Ե�������ͳ��)
 */
struct bill{
	float	money;				//�˵����
	float	tax;				//���ɽ�
	char	date[7]; 			//�˵�����
};


//ˮ���˻���Ϣ
struct	water{
	char	account[WACCLEN + 1];			//�˻�����
	float	bill_total;			//Ӧ�տ��ܼ�
	int	months;				//Ӧ�տ��·���Ŀ
	struct	bill	*table;			//Ӧ���·���ϸ
};

struct pay{
	float	money;					//�˵����
	char	order[ORDELEN + 1];				//������ˮ����,����ר��
	char	date[9]; 				//�˵�����
};
//�����˻���Ϣ
struct	bank{
	char		account[BACCLEN];	//�˻�����
	float		paid_total;		//�Ѹ�������
	struct		pay	*table;		//�����Ѹ��˵���ϸ
};


/***********************************************************************/
/*			���ݿ���÷�װAPI				*/
/***********************************************************************/
//SQLSERVER���ݷ������޴�API�򷵻�NULL
void *DB_init();
void DB_close(void *con);
void DB_real_connect(void *dbcon);

/*******	���²���Ϊ���ݲ�ҵ�����API		****************/
/*	
����:
	�û�ˮ���˻�����puser->pwater->account
���:
	���puser�û�����,��ַ, �������;
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
����:
	�û�ˮ���˻�����puser->pwater->account
���:
	����ɹ�return 0, �����puser�û�������Ϣ:�����ʺŵ�
	�������return -1
 */

void 	protocol(int connfd, void *con);
