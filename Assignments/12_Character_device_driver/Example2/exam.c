#include <linux/module.h>       // Needed for all kernel modules: module_init(), module_exit()
#include <linux/fs.h>           // Needed for alloc_chrdev_region(), unregister_chrdev_region()
#include <linux/cdev.h>         // Needed for cdev utilities: cdev_init(), cdev_add(), cdev_del()

/* ---------- Module Metadata ---------- */
#define DRIVER_AUTHOR "Khanh Khanh@gmail.com"    // Author information
#define DRIVER_DESC   "Hello world kernel module"// Short description of the module
#define DRIVER_VERS   "1.0"                      // Version number of the driver

/* Device name shown in /proc/devices or dmesg */
#define DEVICE_NAME   "m_cdev"

/* ---------- Device Structure ---------- */
struct m_foo_dev {
    dev_t dev_num;       // Device number (includes both Major and Minor numbers)
    struct cdev cdev;    // Character device structure (represents the char device in the kernel)
} mdev;                  // Create one instance of our device (named mdev)

/* ---------- Module Init Function ---------- */
/* This runs when you insert the module: insmod m_cdev.ko */
static int __init chdev_init(void)
{
    /* Step 1: Allocate a device number dynamically
     *   - &mdev.dev_num: where the device number will be stored
     *   - 0: starting minor number
     *   - 1: number of devices we want to allocate
     *   - DEVICE_NAME: name shown in /proc/devices
     */
    if (alloc_chrdev_region(&mdev.dev_num, 0, 1, DEVICE_NAME) < 0) {
        pr_err("Failed to alloc chrdev region\n");
        return -1;
    }

    /* Step 2: Initialize cdev structure
     *   - Normally we pass a pointer to struct file_operations (fops) here,
     *     which defines read/write/open/ioctl callbacks.
     *   - But here we pass NULL ? means this driver does not support any operations.
     */
    cdev_init(&mdev.cdev, NULL);

    /* Step 3: Add the character device to the kernel
     *   - &mdev.cdev: the cdev structure we initialized
     *   - mdev.dev_num: the allocated device number (major:minor)
     *   - 1: number of devices
     */
    if (cdev_add(&mdev.cdev, mdev.dev_num, 1) < 0) {
        pr_err("Failed to add cdev\n");

        /* If failed, free the allocated device number */
        unregister_chrdev_region(mdev.dev_num, 1);
        return -1;
    }

    /* Step 4: Print success information */
    pr_info("Major = %d Minor = %d\n", MAJOR(mdev.dev_num), MINOR(mdev.dev_num));
    pr_info("Hello world kernel module loaded\n");

    return 0;   // Success
}

/* ---------- Module Exit Function ---------- */
/* This runs when you remove the module: rmmod m_cdev */
static void __exit chdev_exit(void)
{
    /* Step 1: Remove the cdev from the kernel */
    cdev_del(&mdev.cdev);

    /* Step 2: Free the allocated device numbers */
    unregister_chrdev_region(mdev.dev_num, 1);

    /* Step 3: Print log message */
    pr_info("Goodbye world kernel module unloaded\n");
}

/* ---------- Register Init/Exit Functions ---------- */
module_init(chdev_init);    // Tells the kernel which function to call on module load
module_exit(chdev_exit);    // Tells the kernel which function to call on module unload

/* ---------- Module Information ---------- */
MODULE_LICENSE("GPL");              // Avoids "tainted kernel" warning
MODULE_AUTHOR(DRIVER_AUTHOR);       // Author info
MODULE_DESCRIPTION(DRIVER_DESC);    // Description of module
MODULE_VERSION(DRIVER_VERS);        // Version info
