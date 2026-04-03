#include<ncurses.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

typedef struct {
	unsigned long long user , nice , system ,idle;
}CPUStats;

void get_cpu_stats(CPUStats *s){
	FILE *fp = fopen("/proc/stat","r");
	if ( fp){
		fscanf(fp,"cpu %llu %llu %llu %llu",&s->user,&s->nice,&s->system,&s->idle);
		fclose(fp);
	}
}

void draw_mem_info(int y,int x){
	FILE *fp = fopen("/proc/meminfo","r");
	char label[32];
	unsigned long total , free ;
	if (fp) {
		fscanf(fp,"Memtotal : %lu KB\n Memfree: %lu kB",&total,&free);
		fclose(fp);
		mvprintw(y,x,"Memory: %lu MB / %lu MB",(total-free)/1024 , total/1024);
	}
}

int main(){
	CPUStats s1,s2;
	initscr();
	noecho();
	curs_set(0);
	timeout(1000);
	while(1){
		get_cpu_stats(&s1);
		usleep(100000);
		get_cpu_stats(&s2);
		unsigned long long totald=(s2.user+ s2.nice + s2.system + s2.idle) - 
                                    (s1.user + s1.nice + s1.system + s1.idle);
		unsigned long long idled = s2.idle - s1.idle;
		double cpu_usage = (double)(totald - idled) / totald*100.0;
		clear();
		attron(A_BOLD | COLOR_PAIR(1));
		mvprintw(1,2,"---system resource  monitor---");
		attroff(A_BOLD);

		mvprintw(3,2,"CPU usage;[%.2f%%]",cpu_usage);
		draw_mem_info(4,2);

		mvprintw(6,2,"PID  | COMMAND");
		mvprintw(7,2,"--------------");

		mvprintw(8,2,"1 | systemd");
		mvprintw(9,2,"%-5d | sys-monitor",getpid());

		mvprintw(12,2,"Press q to exit...");

		refresh();

		int ch=getch();
		if(ch == 'q') break;

	}
	endwin();
	return 0;
}
