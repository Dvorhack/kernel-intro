#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "chall2"
#define CLASS_NAME "chall2"
#define NB_CHAPITRE 10

MODULE_AUTHOR("Dvorhack");
MODULE_DESCRIPTION("Challenge 2");
MODULE_LICENSE("GPL");

int major;
static struct class*  my_class  = NULL;
static struct device* my_device = NULL;

unsigned long (*function)(void) = NULL;
char* quotes[] = { "You got it !", "Almost there", "Try one more ?" };


static ssize_t chall_2_ioctl(struct file *filp, unsigned int code, unsigned long data) {
    printk(KERN_INFO "[chall_2] in chall_2_ioctl");
    int ret = 0;

    switch(code){
        case 0xc0fe:
            unsigned int rand_nb;
            get_random_bytes(&rand_nb, sizeof rand_nb); 
            char *quote = quotes[rand_nb % 3];
            ret = copy_to_user((void *)data, quote, strlen(quote));
        break;
        case 0xbabe:
            function = (unsigned long (*)(void)) data;
            function();
        break;
    
    }
    return ret;
}

static struct file_operations fops = {
    .unlocked_ioctl = chall_2_ioctl,
};

static int __init chall_2_init(void) {
    major = register_chrdev(0, DEVICE_NAME, &fops);

    if ( major < 0 )
        printk(KERN_INFO "[chall_2] Error assigning Major Number!");
    
    // Register device class
    my_class = class_create(CLASS_NAME);
    if (IS_ERR(my_class)) {
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_INFO "[chall_2] Failed to register device class\n");
    }

    // Register the device driver
    my_device = device_create(my_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(my_device)) {
        class_destroy(my_class);
        unregister_chrdev(major, DEVICE_NAME);
        printk(KERN_INFO "[chall_2] Failed to create the device\n");
    }

    printk(KERN_INFO "[chall_2] registered !\n");

    return 0;
}

static void __exit chall_2_exit(void) {
    device_destroy(my_class, MKDEV(major, 0));              // remove the device
    class_unregister(my_class);                             // unregister the device class
    class_destroy(my_class);                                // remove the device class
    unregister_chrdev(major, DEVICE_NAME);                  // unregister the major number
    printk(KERN_INFO "[chall_2] Closing!\n");
}

module_init(chall_2_init);
module_exit(chall_2_exit);