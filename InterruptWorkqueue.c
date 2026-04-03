#include<linux/module.h>
#include<linux/interrupt.h>
#include<linux/workqueue.h>
#include<linux/delay.h>

#define IRQ 1

struct work_struct wrk;

static void queue_bottom_half(struct work_struct *work){
	pr_info("work started");
	msleep(100);
	pr_info("work completed");
}


static irqreturn_t top_half(int irq , void *dev){
	schedule_work(&wrk);
	return IRQ_HANDLED;
}

static  int __init my_wrk_init(void){
	INIT_WORK(&wrk,queue_bottom_half);
	return request_irq(IRQ,top_half,IRQF_SHARED,"workqueue",(void*)top_half);}

static void __exit my_wrk_exit(void){
	free_irq(IRQ,(void*)(top_half));
	cancel_work_sync(&wrk);
}

module_init(my_wrk_init);
module_exit(my_wrk_exit);
MODULE_LICENSE("GPL");
