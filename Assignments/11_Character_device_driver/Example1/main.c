#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("bobobo");
MODULE_DESCRIPTION("Simple Hello World Module");

static int __init exam_init(void) {
    printk(KERN_INFO "Exam Module: Hello, World!\n");
    return 0;
}

static void __exit exam_exit(void) {
    printk(KERN_INFO "Exam Module: Goodbye, World!\n");
}

module_init(exam_init);
module_exit(exam_exit);
