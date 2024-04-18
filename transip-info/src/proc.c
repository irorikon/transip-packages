/*
 * @Author: iRorikon
 * @Date: 2024-04-18 22:58:30
 * @FilePath: \transip-packages\transip-info\src\proc.c
 */
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/seq_file.h>
#include <linux/version.h>
#include "transip-info.h"

static int proc_show(struct seq_file *s, void *v)
{
    const char *value = s->private;

    seq_printf(s, "%s\n", value);

    return 0;
}


static int proc_open(struct inode *inode, struct file *file)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 17, 0)
    return single_open(file, proc_show, PDE_DATA(inode));
#else
    return single_open(file, proc_show, pde_data(inode));
#endif
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0)
struct file_operations proc_fops = {
    .owner = THIS_MODULE,
    .read = seq_read,
    .open = proc_open,
    .llseek = seq_lseek,
    .release = single_release,
};
#else
struct proc_ops proc_fops = {
    .proc_read = seq_read,
    .proc_open = proc_open,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};
#endif

struct proc_dir_entry *create_proc_node(const char *name, const char *value)
{
    return proc_create_data(name, 0, transip_info.parent, &proc_fops, (void *)value);
}

int proc_init_transip_info(void)
{
    transip_info.parent = proc_mkdir(PROC_DIR, NULL);
    if (!transip_info.parent)
        return -ENOENT;

    return 0;
}

int proc_remove_transip_info(void)
{
    proc_remove(transip_info.parent);
    return 0;
}
