#include <linux/module.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/wait.h>
#include <linux/kthread.h>


struct task_struct *ts;

MODULE_LICENSE("GPL");

int thread(void *data)
{
    while (1)
    {
        printk("Hello. This is linux torvalds. Thank for playing linux. \n");
        msleep(100);
        if (kthread_should_stop())
            break;
    }
    return 0;
}

int init_module(void)
{
    printk(KERN_INFO "init_module() called\n");
    ts = kthread_run(thread, NULL, "foo kthread");
    return 0;
}

void cleanup_module(void)
{
    printk(KERN_INFO "cleanup_module() called\n");
    kthread_stop(ts);
}