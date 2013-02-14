#include<stdio.h>
#include <string.h>
#include <stdlib.h>

char* formCommPacket(char* type, char** argv){
	int packet_l;
	char* commpacket; 
	if(strcmp(type, "DataSendSessionInfo")==0){ //
		packet_l=strlen(argv[0])+strlen(argv[1])+strlen("Session:#ofChunks")+1;
		commpacket = malloc(packet_l*sizeof(char));
		strcpy(commpacket, "Session:");
		strcat(commpacket, argv[0]);
		strcat(commpacket, "#ofChunks");
		strcat(commpacket, argv[1]);
	}
	else{
		printf("Type is not recognized by formCommPacket()\n");
		exit(1);
	}
	return commpacket;
}

int main()
{
	char* argv[] = {"A", "1200"};

	printf("come on\n");
	char* str = formCommPacket("DataSendSessionInfo", argv);
	printf("str:%s\n", str);
	//strcat(str, " haha");
	//printf("str:%s\n", str);
	return 0;
/*
	char buf[100];
	strcpy(buf, "come on\0");
    printf("length:%d\n", (int)strlen(buf));
	
	char random_str[]="hadi kardasim ol artik! ";
	char temp_buffer[10];
	sprintf(temp_buffer, "%d", (10));
	strcat(random_str, temp_buffer);
	printf("%s\n", random_str);
*/
}

