
/************************* DB ���ú���**************/
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


/***********7000:��ѯ�û���Ϣ*************/
/*����:�û�ˮ�ѻ���*/
/*���:��Ӧ����(�����û���Ϣ)*/	
int do7000(Send *S, struct user *p, Recv *R ,void *c)
{
	return 0;
}

/***********7001:�û�ǩԼȷ��*************/
/*����:�û�ˮ�ѻ���	+	�û������ʺ�	*/
/*���:��Ӧ����					*/	
//7001ҵ��֮ǰ
int do7001(Send *S, struct user *p, Recv *R, void *c)
{
		return 0;
}

/***********7002:�û�ǩԼ�޸�*************/
/*����:�û�ˮ�ѻ���	+	�û������ʺ�	*/
/*���:��Ӧ����					*/	
int do7002(Send *S, struct user *p, Recv *R, void *c)
{
		return 0;
}

/***********7003:�û�ǩԼɾ��*************/
//����:�û�ˮ�ѻ���			
//���:��Ӧ����  		�����ʺ���Ϊ�մ�
int do7003(Send *S, struct user *p, Recv *R, void *c)
{
		return 0;
}

/***************************�ɷ���Ϣ��ѯ***********/
int do7004(Send *S, struct user *p, Recv *R, void *c)
{
		return 0;
}

/******************************�ɷ�ȷ��******************/
//7005ҵ����Ҫ���Ǵ������߼�
int do7005(Send *S, struct user *p, Recv *R, void *c)
{
		return 0;
}

//����ҵ��
int do7006(Send *S, struct user *p, Recv *R, void *c)
{
		return 0;
}

//���ʽɷѶ���
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
