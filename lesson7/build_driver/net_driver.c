// 00:1f.6 Ethernet controller: Intel Corporation Ethernet Connection (6) I219-V (rev 30)
// vendor id - 8086
// device id - 15be

#define VENDOR_ID 0x8086
#define DEVICE_ID 0x15be

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/pci.h>
#include <linux/netdevice.h>

MODULE_LICENSE("GPL");


static int netdev_open(struct net_device *dev)
{
	printk(KERN_INFO "[intel fake net driver] open...\n");
	netif_start_queue(dev);
	return 0;
}


static int netdev_release(struct net_device *dev)
{
	printk(KERN_INFO "[intel fake net driver] release...\n");
	netif_stop_queue(dev);
	return 0;
}

static int netdev_xmit(struct sk_buff *skb, struct net_device *dev)
{
	printk(KERN_INFO "[intel fake net driver] xmit...\n");
	dev_kfree_skb(skb);
	return 0;
}

static int netdev_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	printk(KERN_INFO "[intel fake net driver] ioctl: command: %x\n", cmd);
	return 0;
}



static const struct net_device_ops netdev_ops = {
	.ndo_open = netdev_open,
	.ndo_stop = netdev_release,
	.ndo_start_xmit = netdev_xmit,
	.ndo_do_ioctl = netdev_ioctl,
};

void netdev_init(struct net_device *dev)
{
	ether_setup(dev);
	dev->netdev_ops = &netdev_ops;
	printk(KERN_INFO "[intel fake net driver] net device initialized!\n");
}

struct net_device *netdev = NULL;

int init_module(void)
{

    netdev = alloc_netdev(0, "fake_intel_netdev", NET_NAME_UNKNOWN, netdev_init);
	if (register_netdev(netdev)) {
		printk(KERN_INFO "[intel fake net driver] device init error\n");
		return 1;
	}

    printk(KERN_INFO "[intel fake net driver] I am being inited; \n");

    return 0;
}

void cleanup_module(void)
{
    unregister_netdev(netdev);

    printk(KERN_INFO "[intel fake net driver] I am being clean up; \n");

    // pci_unregister_driver(&fake_intel_driver);
}
