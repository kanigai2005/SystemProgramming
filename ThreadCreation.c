#include <linux/module.h>    
#include <linux/kernel.h>    
#include <linux/kthread.h> 
#include <linux/delay.h>    

static struct task_struct *my_thread;

static int my_thread_function(void *data) {
    char *thread_name = (char *)data;
    pr_info("My Kernel Thread: %s is starting\n", thread_name);
    while (!kthread_should_stop()) {
        pr_info("My Kernel Thread: I am alive and working...\n");
        msleep(5000);
    }

    pr_info("My Kernel Thread: Stopping now\n");
    return 0;
}

static int __init my_module_init(void) {
    pr_info("My Kernel Thread: Initializing module\n");
    my_thread = kthread_run(my_thread_function, "Thread-1", "my_kthread_task");
    if (IS_ERR(my_thread)) {
        pr_err("My Kernel Thread: Failed to create thread\n");
        return PTR_ERR(my_thread);
    }

    return 0;
}

static void __exit my_module_exit(void) {
    pr_info("My Kernel Thread: Cleaning up module\n");
    if (my_thread) {
        kthread_stop(my_thread);
    }
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
