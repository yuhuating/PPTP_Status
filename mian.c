#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
	char buf[128];
	char tmp[128];
	char user[10];
	char ppp[10];
	char date[20];
	char time[20];
	char ip[20];
	char addr[20];
	char cmd[100];
	int ppp_num = 0;

	FILE *fp_who;

	fp_who = popen("who","r");
	if( fp_who == NULL){
		printf("执行who命令出现异常\n");
	}
	while(fgets(buf, sizeof buf, fp_who)){
		if(strstr(buf,"ppp") != NULL){
			ppp_num++;
			sscanf(buf,"%s%s%s%s%*[^(](%[^)]",user,ppp,date,time,ip);
			sprintf(date,"%s %s",date,time);

			FILE *fp_ifconfig;
			memset(cmd,0,100);
			sprintf(cmd,"ifconfig %s|grep addr",ppp);
			fp_ifconfig = popen(cmd,"r");
			if( fp_ifconfig == NULL){
				printf("执行ifconfig命令出现异常\n");
			}

			while(fgets(tmp, sizeof tmp, fp_ifconfig)){
				sscanf(tmp,"%*[^P]%*[^:]:%[^ ]",addr);
				printf("%s -- %s --  %s -- %s --%s\n",user,ppp,date,ip,addr);
			}
			pclose(fp_ifconfig);
		}

	}
	pclose(fp_who);

	if(ppp_num == 0){
		printf("当前没有PPTP用户在线\n");
	}
	return 0;

 }
