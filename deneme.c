#include<stdio.h>
#include <string.h>

main()
{
	char buf[100];
	strcpy(buf, "come on\0");
    printf("length:%d\n", (int)strlen(buf));
	
	char random_str[]="hadi kardasim ol artik! ";
	char temp_buffer[10];
	sprintf(temp_buffer, "%d", (10));
	strcat(random_str, temp_buffer);
	printf("%s\n", random_str);
	
}

