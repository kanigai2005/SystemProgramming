#include<linux/module.h>
#include<linux/interrupt.h>

#define IRQ 1
static void tasklet_bottom_half(unsigned long data){
	pr_info("Tasklet : bottomhalf");
}

DECLARE_TASKLET_OLD(my_tasklet,tasklet_bottom_half);

static irqreturn_t top_half(int irq , void *dev_id){
	tasklet_schedule(&my_tasklet);
	return IRQ_HANDLED;
}

static int __init my_tasklet_init(void){
	return request_irq(IRQ,top_half,IRQF_SHARED,"tasklet_dev",(void*)(top_half));
}

static void __exit my_tasklet_exit(void){
	free_irq(IRQ,(void*)(top_half));
	tasklet_kill(&my_tasklet);
}
module_init(my_tasklet_init);
module_exit(my_tasklet_exit);
MODULE_LICENSE("GPL");
