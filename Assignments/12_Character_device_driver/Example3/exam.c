#include <linux/module.h>   // Required for all kernel modules: module_init(), module_exit()
#include <linux/fs.h>       // For alloc_chrdev_region(), unregister_chrdev_region()
#include <linux/device.h>   // For class_create(), device_create(), device_destroy()

/* ---------- Module Metadata ---------- */
#define DRIVER_AUTHOR "phonglt9 linuxfromscratch@gmail.com"  // Author info
#define DRIVER_DESC   "Hello world kernel module"             // Description
#define DRIVER_VERS   "1.0"                                   // Version

/* ---------- Device Structure ---------- */
struct m_foo_dev {
    dev_t dev_num;            // Device number (Major + Minor)
    struct class *m_class;    // Pointer to device class (used for sysfs and /dev entry)
} mdev;                       // Create global device instance

/* ---------- Constructor (Init function) ----------
 * Runs when you load the module (insmod)
 */
static int __init chdev_init(void)
{
    /* 1. Allocate Major and Minor numbers dynamically
     *   - alloc_chrdev_region(dev_t *dev, unsigned baseminor, unsigned count, const char *name)
     *   - &mdev.dev_num: output parameter for device number
     *   - 0: starting minor number
     *   - 1: number of devices to allocate
     *   - "m_cdev": name that shows up in /proc/devices
     */
    if (alloc_chrdev_region(&mdev.dev_num, 0, 1, "m_cdev") < 0) {
        pr_err("Failed to alloc chrdev region\n");
        return -1;
    }

    pr_info("Major = %d Minor = %d\n", MAJOR(mdev.dev_num), MINOR(mdev.dev_num));

    /* 2. Create a device class
     *   - class_create(owner, name)
     *   - In modern kernels: class_create(THIS_MODULE, "m_class")
     *   - Creates /sys/class/m_class entry
     *   - Required before device_create()
     */
    if ((mdev.m_class = class_create("m_class")) == NULL) {
        pr_err("Cannot create the struct class for my device\n");
        goto rm_device_numb;   // If failed ? free device number
    }

    /* 3. Create the device node automatically
     *   - device_create(class, parent, devt, drvdata, name)
     *   - Creates /dev/m_device (via udev) with the major/minor we allocated
     *   - Also creates /sys/class/m_class/m_device entry
     */
    if ((device_create(mdev.m_class, NULL, mdev.dev_num, NULL, "m_device")) == NULL) {
        pr_err("Cannot create my device\n");
        goto rm_class;         // If failed ? clean class + device number
    }

    pr_info("Hello world kernel module\n");
    return 0;   // Success

/* ---------- Error handling cleanup ---------- */
rm_class:
    class_destroy(mdev.m_class);
rm_device_numb:
    unregister_chrdev_region(mdev.dev_num, 1);
    return -1;
}

/* ---------- Destructor (Exit function) ----------
 * Runs when you remove the module (rmmod)
 */
static void __exit chdev_exit(void)
{
    /* Remove the device node from /dev and sysfs */
    device_destroy(mdev.m_class, mdev.dev_num);

    /* Destroy the device class (/sys/class/m_class) */
    class_destroy(mdev.m_class);

    /* Free the allocated device numbers */
    unregister_chrdev_region(mdev.dev_num, 1);

    pr_info("Goodbye world kernel module\n");
}

/* ---------- Register Init/Exit Functions ---------- */
module_init(chdev_init);   // Call chdev_init() when module is loaded
module_exit(chdev_exit);   // Call chdev_exit() when module is unloaded

/* ---------- Module Information ---------- */
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
MODULE_VERSION(DRIVER_VERS);
