#include <string.h>
#include <math.h>
#include <stdio.h>

void main(int argc, char** argv)
{
	char *p = argv[1];
	int len = strlen(p);
	int l = len;
	int sum = 0;
	int bit = 0;
	char ch;
	for(; len > 0; len--)
	{
		ch = *(p+len-1);
		if((*(p+len - 1)) > 'a' && (*(p+len -1) < 'f'))
			bit = ch - 'a' + 10; 
		else
			bit = ch - '0';
		bit = bit * pow(16.0,l-len);
		sum += bit;

	}
	printf("%d\n", sum);
}
