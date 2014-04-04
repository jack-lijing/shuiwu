
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

char * getclock(void);
//中文
/* 获取系统日期 i=0 返回 yyyymmdd i=1 返回yyyy-mm-dd*/
char *GetSysDate(int i)
{
    time_t tm;
    static char date[11];
    
    tm = time(NULL);
	if (i==0) {
    strftime(date, 9, "%Y%m%d", localtime(&tm));
	date[8]=0;
	}
	if (i==1) {
    strftime(date, 11, "%Y-%m-%d", localtime(&tm));
	date[10]=0;
    }
    return date;
}

int runlog(char * file, int line, char * msg, ...)
{
   va_list args;
   FILE * fp;
   int fd;
   char rq[8+1];
   int  i;
   char path[80], buff[512];


   va_start(args, msg);
   vsprintf(buff, msg, args);
   va_end(args);
   
   strcpy(rq,GetSysDate(0));

   sprintf(path, "./log.%s", rq+4);
   if ((fp = fopen(path, "a+")) == NULL)
      return -1;

   fd = fileno(fp);
   lockf(fd, F_LOCK, 0L);

   fprintf(fp, "%s|%s|%d|%s\n", getclock(), file, line, buff);

   lockf(fd, F_ULOCK, 0L);
   fclose(fp);
   return 0;
}
char * getclock(void)
{
  static char buf[18];
  struct tm * p;
  time_t clck;

  clck = time((time_t *)0);
  p = localtime(&clck);
  sprintf(buf, "%04d/%02d/%02d-%02d:%02d:%02d",
          p->tm_year+1900, p->tm_mon + 1, p->tm_mday,
          p->tm_hour, p->tm_min, p->tm_sec);
  return buf;
}
