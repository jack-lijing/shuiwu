#include <string.h>
#include <stdlib.h>
#include <stdio.h>
void main()
{
//	char buf[1024]="01787001                      11165400    黄国民                                                      杨湾小  区  2幢404社                                            6228480358377193273 ";
	char buf[1024]="01787002                      11601860    恒庆房产                                                                                                                6228480353117448210 ";
	char s1[20],s2[20];
	memset(s1, 0 , strlen(s1));
	memset(s2, 0 , strlen(s2));
	char h[31]="";
	char w[13]="";
	char name[61]="";
	char addr[61]="";
	char b[21]="";
	/*
	int i=0;
	for(; i< 26; i++)
	{	
		buf[2*i]='a'+i;
		buf[2*i+1]=' ';
	}
*/
	memset(s1, 0 , strlen(s1));
	memset(s2, 0 , strlen(s2));
	sscanf(buf,"%30s%12s%60s%60s%20s",h, w, name,addr, b);
	printf("head = %s water = %s name = %s addr =%s bank =%s\n",h, w, name,addr ,b);
	//sscanf(buf+162,"%20s",b);
	//printf("head = %s water = %s name = %s addr =%s bank =%s\n",h, w, name,addr ,b);
	sscanf(buf,"%30s%12s%60s%*[ ]%*60c%20s",h, w, name, b);
	printf("head = %s water = %s name = %s bank =%s\n",h, w, name ,b);
	memset(addr, 0, 60);
	sprintf(addr,"%-60.60s", buf+102);
	printf("head = %s water = %s name = %s addr =%s bank =%s\n",h, w, name,addr ,b);
}
