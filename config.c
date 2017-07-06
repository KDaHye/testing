#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE 128

int main(int argc, char * argv[]){
    
    int flag = 0;
    FILE * userInfoFile;
    FILE * preseed_temp;
    FILE * preseed;
    
    char userName[BUFFER_SIZE];
    char passwd[BUFFER_SIZE];
    char cmd_md5Passwd[BUFFER_SIZE];
    char readLine[BUFFER_SIZE];
    char writeBuffer[BUFFER_SIZE];

    if( (userInfoFile = fopen("user.info", "r")) == NULL){
        printf(" user.info file not exist \n");
        exit(0);
    }

    fgets(userName, BUFFER_SIZE, userInfoFile);
    fgets(passwd, BUFFER_SIZE, userInfoFile);
    sprintf(cmd_md5Passwd, "echo \"%s\" | mkpasswd -s -m md5 >> user.info", passwd);
    system(cmd_md5Passwd);
    
    for(int i = 0; i<4; ++i) 
        fgets(passwd, BUFFER_SIZE, userInfoFile);

    if( (preseed_temp = fopen("preseed.temp.cfg", "r")) == NULL){
        printf("preseed.temp.cfg file open fail\n");
        exit(0);
    }
    if( (preseed= fopen("preseed.cfg", "w+")) == NULL){
        printf("preseed.cfg file open fail\n");
        exit(0);
    }

    while( fgets(readLine, BUFFER_SIZE, preseed_temp) != NULL ){
        
        writeBuffer[0] = '+';

        if(strstr(readLine, "Create") && !flag)
	    flag = 1;

        if(flag){
            if(strstr(readLine, "netcfg")){
                readLine[(int)strlen(readLine) -2] = '\0';
                sprintf(writeBuffer, "%s %s", readLine, userName);
            }
            else if(strstr(readLine, "password")){
                readLine[(int)strlen(readLine) -2] = '\0';
                sprintf(writeBuffer, "%s %s", readLine, passwd);
            }
            else if(strstr(readLine, "passwd")){
                readLine[(int)strlen(readLine) -2] = '\0';
                sprintf(writeBuffer, "%s %s", readLine, userName);
            }
	}

        if(writeBuffer[0] == '+')
            fputs(readLine, preseed);
        else
            fputs(writeBuffer, preseed);

        
    }
    return 0;
}

