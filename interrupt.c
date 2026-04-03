#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/interrupt.h>
#include<linux/workqueue.h>

#define IRQ 1

static struct work_struct wrk;

static void bottom_half(struct work_struct *work){
	pr_info("Interrupt : Bottom half\n");
}

static irqreturn_t top_half(int irq,void *dv_id){
	schedule_work(&wrk);
	return IRQ_HANDLED;
}

static int __init my_irq_init(void){
	int res;
	INIT_WORK(&wrk,bottom_half);
	res = request_irq(IRQ,top_half,IRQF_SHARED,"int_dev",(void*)(top_half));
	if(res){
		pr_err("Interrupt failed to reg IRQ %d\n",IRQ);
		return res;
	}
	pr_info("Module loaded %d",IRQ);
	return 0;
}

static void __exit my_irq_exit(void){
	free_irq(IRQ,(void*)(top_half));
	cancel_work_sync(&wrk);
	pr_info("Module unloaded");
}
module_init(my_irq_init);
module_exit(my_irq_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("kanigai");
