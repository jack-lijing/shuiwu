#include <string.h>
#include <stdio.h>

showbyte(unsigned char *b, int len)
{
	for(;len > 0; len--)
		printf("%x:%.2x\n", b++,*b);
	printf("\n");
}

void main()
{

	char nstr[2]="";
	char bstr[2]=" ";
	char zstr[3]="0";
	char buf[4]="";
	showbyte(zstr, 3);
	showbyte(buf, 4);
	sprintf(buf,"%-4.4s", nstr);
	showbyte(buf, 4);
	sprintf(buf,"%-4.4s", bstr);
	showbyte(buf, 4);
	sprintf(buf,"%-4.4s", zstr);
	showbyte(buf, 4);

	printf("type \n");

	int i = 12345;
	showbyte((unsigned char *)&i,sizeof(int));
	float l = 6.7;
	showbyte((unsigned char *)&l,sizeof(long));
	char c = 'a';
	showbyte((unsigned char *)&c,sizeof(char));

	char b[10]="";
	printf("len:%d\n", sizeof(b));

}
