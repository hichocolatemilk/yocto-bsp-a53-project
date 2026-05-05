#include <linux/init.h>      // For __init and __exit macros
#include <linux/module.h>    // For MODULE macros
#include <linux/kernel.h>    // For KERN_INFO and printk
			     
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Hello");
MODULE_DESCRIPTION("A simple Hello World kernel module");
			     
// Called when module is loaded
static int __init hello_init(void) {
	printk(KERN_INFO "Hello, world!\n");
	return 0; // Return 0 means succe
}
			    
// Called when module is removed
static void __exit hello_exit(void) {
	printk(KERN_INFO "Goodbye, world!\n");
}
module_init(hello_init);
module_exit(hello_exit);
