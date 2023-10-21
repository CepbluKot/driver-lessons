// 00:1f.6 Ethernet controller: Intel Corporation Ethernet Connection (6) I219-V (rev 30)
// vendor id - 8086
// device id - 15be

#define VENDOR_ID 0x8086
#define DEVICE_ID 0x15be
#define REGISTER_OFFSET 0x05400

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/pci.h>

MODULE_LICENSE("GPL");

static const struct pci_device_id intel_pci_tbl[] =
    {
        {PCI_DEVICE(VENDOR_ID, DEVICE_ID)}, {0}};

MODULE_DEVICE_TABLE(pci, intel_pci_tbl);

static int fake_intel_probe(struct pci_dev *pdev, const struct pci_device_id *ent)
{
    printk(KERN_INFO "[intel fake driver] I am being probed; \n");

    int ret;
    int bar = 2; // Bar to be reserved
    int bar_mask;
    unsigned long io_base = 0;
    unsigned long mem_len = 0;
    unsigned int register_data = 0;
    static void *logical_address;

    // ret = pci_enable_device(pdev);
    // if (ret < 0)
    //     return ret;

    ret = pci_request_regions(pdev, "fake intel driver");
    if (ret < 0)
    {
        pci_disable_device(pdev);
        printk(KERN_INFO "[intel fake driver] I am failed i am being disabled; \n");

        return ret;
    }

    int req_bar_id = 0;
    io_base = pci_resource_start(pdev, req_bar_id);
    mem_len = pci_resource_len(pdev, req_bar_id);

    logical_address = ioremap(io_base, mem_len);

    int count = 0;
    bool counting = false;

    for (int i = 0; i < mem_len; i++)
    {

        register_data = ioread8(logical_address + i);

        if (register_data == 0xf8)
            counting = true;

        if (counting && count < 8)  // looking for mac addr
        {
            printk(KERN_INFO "[id %i]  looking for MAC = %x \n", i, register_data);
            count += 1;

            if (count == 7)
            {
                printk(KERN_INFO "---- \n");
                counting = false;
                count = 0;
            }
        }
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
    // .shutdown =     e100_shutdown,
    // .err_handler = &e100_err_handler,
};

int init_module(void)
{
    printk(KERN_INFO "[intel fake driver] I am being inited; \n");
    return pci_register_driver(&fake_intel_driver);
}
void cleanup_module(void)
{
    printk(KERN_INFO "[intel fake driver] I am being clean up; \n");
    pci_unregister_driver(&fake_intel_driver);
}
