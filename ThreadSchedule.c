#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/kthread.h>
#include<linux/sched.h>
#include<linux/delay.h>

static struct task_struct *t1;
static struct task_struct *t2; 
static struct task_struct *t3; 

static int process_logic(void *data){

	char *proc_name = (char *) data;
	int c=0;
	while(c<10){
		c++;
		pr_info("\n[%s] is running . iteration = [%d] ",proc_name , c );
		msleep(5000);
	}
	return 0;
}
static int __init scheduler_project_init(void){
	pr_info("starting scheduler proj ...\n"); 
	t1=kthread_run(process_logic,"P A","A");
	t2=kthread_run(process_logic,"P B","B");
	t3=kthread_run(process_logic,"P C","C");	
	return 0;
}
static void __exit scheduler_project_exit(void){
	pr_info("Cleaning up");
	//return 0;
}
module_init(scheduler_project_init);
module_exit(scheduler_project_exit);
MODULE_LICENSE("GPL");
