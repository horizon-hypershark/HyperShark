#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xbce771f0, "module_layout" },
	{ 0xf9fdaf8c, "kmalloc_caches" },
	{ 0x105e2727, "__tracepoint_kmalloc" },
	{ 0x63ecad53, "register_netdevice_notifier" },
	{ 0xfe769456, "unregister_netdevice_notifier" },
	{ 0x9629486a, "per_cpu__cpu_number" },
	{ 0x2bc09a00, "pf_ring_add_module_dependency" },
	{ 0xb72397d5, "printk" },
	{ 0xacdeb154, "__tracepoint_module_get" },
	{ 0xb4390f9a, "mcount" },
	{ 0x1f9526ed, "module_put" },
	{ 0x91920ad0, "kmem_cache_alloc" },
	{ 0x7ecb001b, "__per_cpu_offset" },
	{ 0x37a0cba, "kfree" },
	{ 0x701d0ebd, "snprintf" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=pf_ring";


MODULE_INFO(srcversion, "37ED3AA4C4D1CA826CD3A69");
