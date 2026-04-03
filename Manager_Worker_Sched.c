#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/sched.h>

static struct task_struct *workers[5];
static struct task_struct *manager_thread;
static char *names[] = {"A","B","C","D","E"};
static int worker_fn(void *data){
	char *name=(char *)data;
	while(!kthread_should_stop()){
		set_current_state(TASK_INTERRUPTIBLE);
		schedule();
		if(kthread_should_stop()) break;
		pr_info("%s is executing its time slice..\n",name);
	}
	return 0;
}
static int manager_fn(void *data){
	int i=0;
	pr_info("Manual RR");
	while(!kthread_should_stop()){
		struct task_struct *t=workers[i];
		pr_info("%s",names[i]);
		wake_up_process(t);
		msleep(2000);
		i=(i+1)%5;
	}
	return 0;
}
static int __init my_sched_init(void){
	int i;
	for(i=0;i<5;i++){
		workers[i]=kthread_run(worker_fn, names[i] , names[i]);

	}
	manager_thread=kthread_run(manager_fn,NULL,"Manager_Thread");
	return 0;
}

static void __exit my_sched_exit(void){
	int i;
	kthread_stop(manager_thread);
	for(i=0;i<3;i++){
		kthread_stop(workers[i]);
	}
}
module_init(my_sched_init);
module_exit(my_sched_exit);
MODULE_LICENSE("GPL");
