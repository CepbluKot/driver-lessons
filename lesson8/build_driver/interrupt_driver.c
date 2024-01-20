#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>
#include <linux/time.h>
#include <linux/ioctl.h>

// #ifndef MODULE_INTERFACE_H
// #define MODULE_INTERFACE_H


#define MAX_IO_BUFFER 40

#define MAJOR_NUMBER 240
#define DEVICE_NAME "exampl"

#define QUERY_GET_RESET_COUNT _IOR(MAJOR_NUMBER, 1, char *)
#define QUERY_GET_RESET_DATE  _IOR(MAJOR_NUMBER, 2, unsigned long long *)
#define QUERY_RESET_COUNTER   _IO(MAJOR_NUMBER, 3)


MODULE_LICENSE("GPL");


static int dev_open_count;
static unsigned interrupt_counter;
static unsigned long long last_reset_date;

static dev_t first;
static struct class *cl;
static struct cdev char_dev;


static int device_open(struct inode*, struct file*);
static int device_release(struct inode*, struct file*);
static irq_handler_t keyboard_handler(int, void*, struct pt_regs*);


long device_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param);

static struct file_operations fops =
{
    .open = device_open,
    .release = device_release,
    .unlocked_ioctl = device_ioctl
};

static int __init init(void)
{
    dev_open_count = 0;

    if (alloc_chrdev_region(&first, 0, 1, DEVICE_NAME) < 0)
    {
        return -1;
    }
    printk(KERN_INFO "initialized with <Major, Minor>: <%d, %d>\n", MAJOR(first), MINOR(first));
    if ((cl = class_create( "chardrv")) == NULL)
    {
        printk(KERN_INFO "class_create() failed");
        unregister_chrdev_region(first, 1);
        return -1;
    }
    if (device_create(cl, NULL, first, NULL, DEVICE_NAME) == NULL)
    {
        printk(KERN_INFO "device_create() failed");
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return -1;
    }

    cdev_init(&char_dev, &fops);
    if (cdev_add(&char_dev, first, 1) == -1)
    {
        device_destroy(cl, first);
        class_destroy(cl);
        unregister_chrdev_region(first, 1);
        return -1;
    }

    int irq = request_irq(1, (irq_handler_t)keyboard_handler, IRQF_SHARED, "keyboard-irq-handler", (void*)keyboard_handler);
    if (irq != 0)
    {
        printk(KERN_ALERT "Failure at requesting interrupt handler\n");
        return irq;
    }

    reset_counter();
    printk(KERN_INFO "Module key-handler successfully loaded\n");
    return 0;
}

static void __exit cleanup(void)
{
    free_irq(1, (void*)keyboard_handler);

    device_destroy(cl, first);
    class_destroy(cl);
    unregister_chrdev_region(first, 1);

    printk(KERN_INFO "Module key-handler successfully unloaded\n");
}

module_init(init);
module_exit(cleanup);

long device_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
    switch (ioctl_num)
    {
    case QUERY_GET_RESET_COUNT:
        write_reset_count((char*)ioctl_param);
        break;
    case QUERY_RESET_COUNTER:
        reset_counter();
        break;
    }

    return 0;
}


static int device_open(struct inode *i, struct file *f)
{
    if (dev_open_count)
        return -EBUSY;

    ++dev_open_count;
    try_module_get(THIS_MODULE);
    return 0;
}

static int device_release(struct inode *i, struct file *f)
{
    --dev_open_count;
    module_put(THIS_MODULE);
    return 0;
}

static irq_handler_t keyboard_handler(int irq, void *dev_id, struct pt_regs *regs)
{
    ++interrupt_counter;
    return (irq_handler_t)IRQ_HANDLED;
}