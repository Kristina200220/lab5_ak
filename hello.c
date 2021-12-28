#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/ktime.h>
#include <linux/slab.h>
#include <linux/errno.h>

MODULE_AUTHOR("Kristina Bondar <kristina.bondar200220@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static uint count = 1;

module_param(count, uint, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(count, "A parameter that determines how many times the message will be displayed. Accepts values from 0-10.");

struct my_struct {
    struct list_head tlist;
    ktime_t time;
};

static LIST_HEAD(my_list_head);

static int __init hello_init(void)
{

    if (count == 0) {
		pr_warn("Be careful, the value is so small!!!\n");
    } else if (count >= 5 && count <= 10) {
		pr_warn("Be careful, the value is midle!\n");
    } else if (count > 10) {
		pr_err("Value is not correct!)\n");
		return -EINVAL;
    }

    int i;
	for (i = 0; i < count; i++) {

		struct my_struct *ms = NULL;

		ms = kmalloc(sizeof(struct my_struct), GFP_KERNEL);
		ms->time = ktime_get();
		list_add_tail(&ms->tlist, &my_list_head);

		pr_info("Hello, world!\n");
    }

    return 0;
}

static void __exit hello_exit(void)
{
    int count = 0;
    struct my_struct *t, *tmp;

    if (ZERO_OR_NULL_PTR(&my_list_head))
    	return 0;

    list_for_each_entry_safe(t, tmp, &my_list_head, tlist) {
		pr_info("i: %d; printing time = %lld\n", count++, ktime_to_ns(t->time));
		list_del(&t->tlist);
		kfree(t);
    }
    pr_info("Module removed\n");
}

module_init(hello_init);
module_exit(hello_exit);