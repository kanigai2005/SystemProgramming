#include<stdio.h>
#include<stdlib.h>
#include<sys/ptrace.h>
#include<sys/user.h>
#include<sys/wait.h>
#include<unistd.h>
#include<sys/types.h>

void print_reg(pid_t pid){
	struct user_regs_struct regs;
	ptrace(PTRACE_GETREGS,pid,NULL,&regs);
	printf("----Registers----\n");
	printf("RIP: 0x%llx\n",regs.rip);
	printf("RAX:0x%llx\n",regs.rax);
	printf("RBX:0x%llx\n",regs.rbx);
	printf("RSP:0x%llx\n",regs.rsp);
}

int main(int argc , char** argv){
	if(argc<2){
		printf("Pid:%s\n",argv[0]);
		return 1;
	}
	pid_t tp=atoi(argv[1]);
	ptrace(PTRACE_ATTACH , tp , NULL ,NULL);
	waitpid(tp,NULL,0);
	printf("Attached\n");
	char command;
	while(1){
		printf("\n[r]egs, [s]tep, [c]ontinue, [q]uit > ");
		scanf("%c",&command);
		if (command == 'q') break;
		switch(command){
			case 'r':
				print_reg(tp);
				break;
			case 'c':
				ptrace(PTRACE_CONT,tp,NULL,NULL);
				printf("Continuing...");
				waitpid(tp,NULL,0);
				break;
			case 's':
				if(ptrace(PTRACE_SINGLESTEP , tp , NULL , NULL)<0){
					perror("ptrace step");
				}
				waitpid(tp,NULL,0);
				printf("stepped to nxt ins\n");
				break;
			default:
				printf("Unknown command\n");
		}
	}
	ptrace(PTRACE_DETACH , tp, NULL,NULL);
	printf("Detached \n");
	return 0;
}
