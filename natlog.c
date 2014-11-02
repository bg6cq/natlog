#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAXLEN 2048

FILE *fp = NULL;

void changefile(struct tm *ctm)
{
	char fnbuf[MAXLEN];
	if(fp) pclose(fp);
	snprintf(fnbuf,MAXLEN,"gzip > /home/natlog/%04d.%02d.%02d.%02d%02d%02d.gz",
		ctm->tm_year+1900, ctm->tm_mon+1, ctm->tm_mday,ctm->tm_hour, ctm->tm_min, ctm->tm_sec);
	fp = popen(fnbuf,"w");
}
void Log(char *s)
{
	time_t t;
	static time_t last_t=0;
	static int tm_hour, tm_min, tm_sec;
	time(&t);
	if ( t != last_t ) {
		static int lastday = 0;
		struct tm *ctm;
		last_t = t;
		ctm = localtime(&t);
		tm_hour = ctm->tm_hour;
		tm_min = ctm->tm_min;
		tm_sec = ctm->tm_sec;
		if( ctm->tm_mday != lastday)  {
			changefile(ctm);
			lastday = ctm->tm_mday;
		}
	}
	fprintf(fp,"%02d:%02d:%02d %s\n", tm_hour, tm_min, tm_sec, s);
}
int main(void)
{	
	char buf[MAXLEN];
	
	while(fgets(buf,MAXLEN,stdin)) {
		int len = strlen(buf);
		if(len<20) continue;
		if(buf[len-1]=='\n') { 
			len--; 
			buf[len]=0;
		}
		if(  (buf[len-2]=='=') 	&& (buf[len-1]=='0') ) continue;
		Log( buf+ 9);
	}	
	return 0;
}

