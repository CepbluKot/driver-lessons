#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */

#include <linux/netdevice.h> 

MODULE_LICENSE("GPL");

static int chriz_open(struct net_device *dev)
{
	printk(KERN_INFO "fake_net_driver open... fake_net_driver Interface %s with MAC %02x:%02x:%02x:%02x:%02x:%02x \n", dev->name
            , dev->dev_addr[0]
            , dev->dev_addr[1]
            , dev->dev_addr[2]
            , dev->dev_addr[3]
            , dev->dev_addr[4]
            , dev->dev_addr[5]);

	printk(KERN_INFO "fake_net_driver open...\n");
	netif_start_queue(dev);
	return 0;
}

static int chriz_release(struct net_device *dev)
{
	printk(KERN_INFO "fake_net_driver release...\n");
	netif_stop_queue(dev);
	return 0;
}

static int chriz_xmit(struct sk_buff *skb, struct net_device *dev)
{
	printk(KERN_INFO "fake_net_driver xmit... %s\n", skb->head);
	dev->stats.tx_packets += 1;
dev->stats.tx_bytes += skb->len;

	dev_kfree_skb(skb);
	return 0;
}

static int chriz_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd)
{
	printk(KERN_INFO "fake_net_driver ioctl: command: %x\n", cmd);
	return 0;
}

// ---------------------------------------------------------------

static int my_set_mac_address(struct net_device *dev,
						       void *addr)
{

	memcpy(dev->dev_addr, addr + 2, 6);

	printk(KERN_INFO "fake_net_driver wanna set new mac %02x:%02x:%02x:%02x:%02x:%02x \n"
            , dev->dev_addr[0]
            , dev->dev_addr[1]
            , dev->dev_addr[2]
            , dev->dev_addr[3]
            , dev->dev_addr[4]
            , dev->dev_addr[5]);
	return 0;
} 

static const struct net_device_ops chriz_netdev_ops = {
	.ndo_open = chriz_open,
	.ndo_stop = chriz_release,
	.ndo_set_mac_address = my_set_mac_address,
	.ndo_start_xmit = chriz_xmit,
	// .ndo_do_ioctl = chriz_ioctl,
};

void chriz_init(struct net_device *dev)
{

	ether_setup(dev);
	dev->netdev_ops = &chriz_netdev_ops;
	printk(KERN_INFO "fake_net_driver device initialized!\n");
}

// ---------------------------------------------------------------

struct net_device *chriz_dev = NULL;

static int __init chriz_init_module(void)
{
	printk(KERN_INFO "fake_net_driver init module...\n");
	chriz_dev = alloc_netdev(0, "fake_net_driver", NET_NAME_UNKNOWN, chriz_init);
	if (register_netdev(chriz_dev)) {
		printk("fake_net_driver: device init error\n");
		return 1;
	}
	return 0;
}

static void __exit chriz_exit_module(void)
{
	unregister_netdev(chriz_dev);
	printk(KERN_INFO "fake_net_driver exit module... Goodbye!\n");
}

module_init(chriz_init_module);
module_exit(chriz_exit_module);