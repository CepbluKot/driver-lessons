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


    unsigned int register_data_1;
    unsigned int register_data_2;
    unsigned int register_data_3;
    unsigned int register_data_4;
    unsigned int register_data_5;
    unsigned int register_data_6;

    for (int i = 0; i < mem_len - 6; i++)
    {
        // my mac: f8:75:a4:fc:4e:c8

        // looking for mac addr
        register_data_1 = ioread8(logical_address + i);
        register_data_2 = ioread8(logical_address + i + 1);
        register_data_3 = ioread8(logical_address + i + 2);
        register_data_4 = ioread8(logical_address + i + 3);
        register_data_5 = ioread8(logical_address + i + 4);
        register_data_6 = ioread8(logical_address + i + 5);

        if (register_data_1 == 0xf8 &&
            register_data_2 == 0x75 &&
            register_data_3 == 0xa4 &&
            register_data_4 == 0xfc &&
            register_data_5 == 0x4e &&
            register_data_6 == 0xc8)
        {
            printk(KERN_INFO "[found mac from id %i] %x %x %x %x %x %x \n", i, register_data_1, register_data_2, register_data_3, register_data_4, register_data_5, register_data_6);
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
