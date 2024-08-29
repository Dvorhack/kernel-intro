#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "chall1"
#define CLASS_NAME "chall1"
#define NB_CHAPITRE 10

MODULE_AUTHOR("Dvorhack");
MODULE_DESCRIPTION("Challenge 1");
MODULE_LICENSE("GPL");

int major;
static struct class*  my_class  = NULL;
static struct device* my_device = NULL;

char* quotes[] = { "You got it !", "Almost there", "Try one more ?" };

static ssize_t chall_1_read(struct file *filp, char __user *buffer, size_t len, loff_t *off) {
    printk(KERN_INFO "[chall_1] in chall_1_read");

    unsigned int rand_nb;
    get_random_bytes(&rand_nb, sizeof rand_nb); 
    char *quote = quotes[rand_nb % 3];

    int ret = copy_to_user(buffer, quote, strlen(quote));

    return ret;
}

static ssize_t chall_1_write(struct file *filp, const char __user *buffer, size_t len, loff_t *off) {
    printk(KERN_INFO "[chall_1] in chall_1_write");
    int ret = 0;
    char secure_buffer[0x100];

    ret = copy_from_user(secure_buffer, buffer, len);
    return ret;
}

static struct file_operations fops = {
    .read = chall_1_read,
    .write = chall_1_write,
};

static int __init chall_1_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if ( major < 0 )
        printk(KERN_INFO "[chall_1] Error assigning Major Number!");
    
    // Register device class
    my_class = class_create(CLASS_NAME);
    if (IS_ERR(my_class)) {
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_INFO "[chall_1] Failed to register device class\n");
    }

    // Register the device driver
    my_device = device_create(my_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(my_device)) {
        class_destroy(my_class);
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_INFO "[chall_1] Failed to create the device\n");
    }

    printk(KERN_INFO "[chall_1] registered !\n");

    return 0;
}

static void __exit chall_1_exit(void) {
    device_destroy(my_class, MKDEV(major, 0));              // remove the device
    class_unregister(my_class);                             // unregister the device class
    class_destroy(my_class);                                // remove the device class
    unregister_chrdev(major, DEVICE_NAME);                  // unregister the major number
    printk(KERN_INFO "[chall_1] Closing!\n");
}

module_init(chall_1_init);
module_exit(chall_1_exit);