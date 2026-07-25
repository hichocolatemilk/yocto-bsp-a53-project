#include <linux/init.h>      // For __init and __exit macros
#include <linux/module.h>    // For MODULE macros
#include <linux/kernel.h>    // For KERN_INFO and printk
#include <linux/uaccess.h>	// copy_to_user(), copy_from_user()
#include <linux/string.h>  //strlen
#include <linux/errno.h> // EFAULT
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("char driver");
MODULE_DESCRIPTION("A simple char driver kernel module");

// static int __init Char_init(void);
// static int char_open(struct inode *inode, struct file *filp);
// static int char_release(struct inode *inode, struct file *filp);
// static ssize_t char_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
// static ssize_t char_write(struct file *filp, const char __user *buf, size_t len, loff_t *off);
// static void __exit Char_exit(void);

static int major;
static char driver_buf[100];
static int stored_len = 0;  
static struct class *cls;

//기존 방식 char 그냥 선언
// static int __init char_init(void) {
//     major = register_chrdev(0, "chartest", &fops);
//     if(major < 0){
//         printk(KERN_CRIT "major failed %d\n", major);
//         return major;
//     }
//     printk(KERN_NOTICE "Char Init!\n");

// 	return 0; // Return 0 means succe
// }

//open echo로하는거에는 크게 상관?없을거같음.
static int char_open(struct inode *inode, struct file *filp){
    printk(KERN_INFO "Char_open\n");
    return 0;
}

//close echo로하는거에는 크게 상관?없을거같음.
static int char_release(struct inode *inode, struct file *filp){
    printk(KERN_INFO "Char_close\n");
    return 0;
}

//
static ssize_t char_read(struct file *filp, char __user *buf, size_t len, loff_t *off){
   
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


static ssize_t char_write(struct file *filp, const char __user *buf, size_t len, loff_t *off){
   
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

static struct file_operations fops ={
    .owner = THIS_MODULE,
    .read = char_read,
    .write = char_write,
    .open = char_open,
    .release = char_release
 };


//compatilbe을 보고 있으면 probe하는 방식
static int char_probe(struct platform_device *pdev){
    
    major = register_chrdev(0, "chartest", &fops);
    if(major < 0){
        printk(KERN_CRIT "major failed %d\n", major);
        return major;
    }
    // cls = class_create(THIS_MODULE, "chartest"); //6.4이전
    cls = class_create("chartest"); // 6.4 이후
    device_create(cls, NULL, MKDEV(major,0), NULL, "chartest");
    printk(KERN_NOTICE "Platform Device Matched!...\n");
    printk(KERN_NOTICE "Char Init!\n");

	return 0; // Return 0 means succe
}

static int char_remove(struct platform_device *pdev){
    device_destroy(cls, MKDEV(major,0));
    class_destroy(cls);
    unregister_chrdev(major, "chartest");
    printk(KERN_INFO "Platform Device Removed!...\n");
	printk(KERN_INFO "Goodbye, driver!\n");
    return 0;
}

//dts의 compatble과 맞춰야함
static const struct of_device_id my_dev_match[] = {
    { .compatible = "my_chartest"},
    {                            }
};

MODULE_DEVICE_TABLE(of, my_dev_match);

//name은 위의 match 테이블과 이름이 같아야함.
static struct platform_driver char_platform_driver = {
    .probe = char_probe,
    .remove = char_remove,
    .driver = {
        .name = "my_chartest",
        .owner = THIS_MODULE,
        .of_match_table = my_dev_match, // 위에 만든 매칭 테이블 등록
    },
};

// Called when module is loaded
// module_init(char_init);
// module_exit(char_exit);
module_platform_driver(char_platform_driver);
