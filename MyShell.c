#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>

#define MAX 1024

int main(){
char input[MAX];
//char token[MAX];

while(1){
	printf(">shell");
	fgets(input,MAX,stdin);
	input[strcspn(input,"\n")]; 
	if(strcmp(input,"exit")==0){
		break;
	}
	char *arg[100];
	int i=0;
	char *token=strtok(input," ");
	while(token){
		arg[i++]=token;
		token=strtok(NULL," ");
	}
	arg[i]=NULL;
	if(strcmp(arg[0],"cd")==0){
		if(arg[1]!=NULL){
			chdir(arg[1]);
		}else{
			printf("error");
		}
		continue;
	}
	pid_t pid = fork();
	if(pid==0){
		execvp(arg[0],arg);
		perror("error");
	}
	else if(pid>0){
		wait(NULL);
	}
	else{
		perror("fork failed");
	}
}
return 0;
}
