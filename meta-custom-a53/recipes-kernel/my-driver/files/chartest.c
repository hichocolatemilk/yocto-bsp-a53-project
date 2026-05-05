#include <linux/init.h>      // For __init and __exit macros
#include <linux/module.h>    // For MODULE macros
#include <linux/kernel.h>    // For KERN_INFO and printk
#include <linux/uaccess.h>	// copy_to_user(), copy_from_user()
#include <linux/string.h>  //strlen
#include <linux/errno.h> // EFAULT
#include <linux/device.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("char driver");
MODULE_DESCRIPTION("A simple char driver kernel module");

static int __init Char_init(void);
static int Char_open(struct inode *inode, struct file *filp);
static int Char_release(struct inode *inode, struct file *filp);
static ssize_t Char_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t Char_write(struct file *filp, const char __user *buf, size_t len, loff_t *off);
static void __exit Char_exit(void);

static int major;
static char driver_buf[100];
static int stored_len = 0;  
static struct class *cls;

struct file_operations fops ={
    .owner = THIS_MODULE,
    .read = Char_read,
    .write = Char_write,
    .open = Char_open,
    .release = Char_release
 };

// Called when module is loaded
static int __init Char_init(void) {
    major = register_chrdev(0, "chartest", &fops);
    if(major < 0){
        printk(KERN_CRIT "major failed %d\n", major);
        return major;
    }
    printk(KERN_NOTICE "Char Init!\n");
    // cls = class_create(THIS_MODULE, "chartest"); //6.4이전
    cls = class_create("chartest"); // 6.4 이후
    device_create(cls, NULL, MKDEV(major,0), NULL, "chartest");
    pr_info("Major number %d\n", major);
    pr_info("Device file : /dev/%s\n", "chartest");

	return 0; 
}

//open echo로하는거에는 크게 상관?없을거같음.
static int Char_open(struct inode *inode, struct file *filp){
    printk(KERN_INFO "Char_open\n");
    return 0;
}

//close echo로하는거에는 크게 상관?없을거같음.
static int Char_release(struct inode *inode, struct file *filp){
    printk(KERN_INFO "Char_close\n");
    return 0;
}

//
static ssize_t Char_read(struct file *filp, char __user *buf, size_t len, loff_t *off){
   
    if (*off >= stored_len)
        return 0;

   if (len > stored_len - *off) {
        len = stored_len - *off;
    }
    //dst,src,len 리턴 없으면 WARNING 나옴
    if(copy_to_user(buf, driver_buf + *off, len)){
        return -EFAULT;
    }
    *off += len;
    printk(KERN_INFO "%s", driver_buf);
    return len;
}


static ssize_t Char_write(struct file *filp, const char __user *buf, size_t len, loff_t *off){
   
    if (len > 100) 
        len = 100;
    //dst,src,len 리턴 없으면 WARNING 나옴
    if(copy_from_user(driver_buf, buf, len)){
        return -EFAULT;
    }
    stored_len = len;
    if (stored_len < 100) 
        driver_buf[stored_len] = '\0';
    printk(KERN_INFO "%s", driver_buf);
    return len;
}

// Called when module is removed
static void __exit Char_exit(void) {
    device_destroy(cls, MKDEV(major,0));
    class_destroy(cls);
    unregister_chrdev(major, "chartest");
	printk(KERN_INFO "Goodbye, chartest!\n");
}

module_init(Char_init);
module_exit(Char_exit);
