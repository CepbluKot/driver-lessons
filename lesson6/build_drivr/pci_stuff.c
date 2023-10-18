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

MODULE_LICENSE("GPL");

static const struct pci_device_id intel_pci_tbl[] =
{                                                      
    {PCI_DEVICE( VENDOR_ID, DEVICE_ID)}, {0}
};

MODULE_DEVICE_TABLE(pci, intel_pci_tbl);

static int fake_intel_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
    printk(KERN_INFO "[intel fake driver] I am being probed; \n");

    int ret;
    // ret = pci_enable_device(pdev);
    // if (ret < 0)
    //     return ret;

    ret = pci_request_regions(pdev, "fake intel driver");
    if (ret < 0)
    {
        pci_disable_device(pdev);
        return ret;
    }

    return 0;
}

static void fake_intel_remove(struct pci_dev *pdev)
{
    printk(KERN_INFO "[intel fake driver] I am being removed; \n");
    pci_release_regions(pdev);
    pci_disable_device(pdev);
}

static struct pci_driver fake_intel_driver = {
    .name = "fake intel driver",
    .id_table = intel_pci_tbl,
    .probe = fake_intel_probe,
    .remove = fake_intel_remove

    // /* Power Management hooks */
    // .driver.pm =	&e100_pm_ops,

    // .shutdown =     e100_shutdown,
    // .err_handler = &e100_err_handler,
};

static int __init fake_intel_init(void)
{

    printk(KERN_INFO "[intel fake driver] I am being inited; \n");
    return pci_register_driver(&fake_intel_driver);
}
static void __exit fake_intel_cleanup(void)
{

    printk(KERN_INFO "[intel fake driver] I am being clean up; \n");
    pci_unregister_driver(&fake_intel_driver);
}
