#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif


static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xb560f28, "pci_release_regions" },
	{ 0x2aca3546, "pci_disable_device" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x4896ad3, "pci_request_regions" },
	{ 0xde80cd09, "ioremap" },
	{ 0xb19b445, "ioread8" },
	{ 0xdbef4a92, "__pci_register_driver" },
	{ 0x9ec8f984, "pci_unregister_driver" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x122c3a7e, "_printk" },
	{ 0x453e7dc, "module_layout" },
};

MODULE_INFO(depends, "");

MODULE_ALIAS("pci:v00008086d000015BEsv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "CC23CE1EA9CD221DB5B5D76");
