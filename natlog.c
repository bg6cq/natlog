#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAXLEN 2048

int lastday=0;
FILE *fp=NULL;

void changefile(struct tm *ctm)
{
	char fnbuf[MAXLEN];
	if(fp) pclose(fp);
	snprintf(fnbuf,MAXLEN,"gzip > /home/natlog/%04d.%02d.%02d.%02d%02d%02d.gz",
		ctm->tm_year+1900, ctm->tm_mon+1, ctm->tm_mday,ctm->tm_hour, ctm->tm_min, ctm->tm_sec);
	fp=popen(fnbuf,"w");
}
void Log(char *s)
{
	time_t t;
	struct tm *ctm;
	time(&t);
	ctm = localtime(&t);
	if( ctm->tm_mday != lastday)  {
		changefile(ctm);
		lastday = ctm->tm_mday;
	}
	fprintf(fp,"%02d:%02d:%02d %s\n",ctm->tm_hour, ctm->tm_min, ctm->tm_sec,s);
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

