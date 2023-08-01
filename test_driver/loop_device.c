#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/cdev.h> 	

#define LOOP_MAJOR 0
#define LOOP_NAME "loop_device"
#define BYTES_PER_ROW 16

MODULE_LICENSE("GPL");

int major = 0;

// Character device structure
static struct cdev my_cdev;

static char *output_file = "/tmp/output";
static struct file *output_file_ptr;

static int loop_open(struct inode *inode, struct file *file)
{
    pr_info("loop_device: Device opened\n");

    return 0;
}

static ssize_t loop_read(struct file *file, char __user *buf,
                         size_t count, loff_t *ppos)
{
    // Reading from /dev/loop is not implemented in this example.
    return 0;
}

static ssize_t loop_write(struct file *file, const char __user *buf,
                          size_t count, loff_t *ppos)
{
    // char *hex_buffer;
    // size_t i, j;
    // ssize_t bytes_written = 0;
    // int rows = count / BYTES_PER_ROW;
    // int remaining = count % BYTES_PER_ROW;

    // // Open the output file for writing
    // output_file_ptr = filp_open(output_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    // if (IS_ERR(output_file_ptr)) 
    // {
    //     pr_err("loop_device: Unable to open output file\n");
    //     unregister_chrdev(LOOP_MAJOR, LOOP_NAME);
    //     return PTR_ERR(output_file_ptr);
    // }

    // pr_info("loop_device: %s file opend.\n", output_file);


    pr_info("loop_device: %s\n", buf);

    // hex_buffer = kmalloc(BYTES_PER_ROW * 3 + 2, GFP_KERNEL);
    // if (!hex_buffer)
    //     return -ENOMEM;

    // for (i = 0; i < rows; i++) {
    //     for (j = 0; j < BYTES_PER_ROW; j++) {
    //         sprintf(&hex_buffer[j * 3], "%02X ", buf[i * BYTES_PER_ROW + j] & 0xFF);
    //     }
    //     hex_buffer[BYTES_PER_ROW * 3] = '\n';
    //     hex_buffer[BYTES_PER_ROW * 3 + 1] = '\0';

    //     if (ppos)
    //         if (*ppos + bytes_written + count > i * BYTES_PER_ROW)
    //             break;

    //     pr_info("%s", hex_buffer);

    //     // Write to the output file
    //     kernel_write(output_file_ptr, hex_buffer, BYTES_PER_ROW * 3 + 1, &output_file_ptr->f_pos);
    //     bytes_written += BYTES_PER_ROW;
    // }

    // if (remaining > 0) {
    //     for (j = 0; j < remaining; j++) {
    //         sprintf(&hex_buffer[j * 3], "%02X ", buf[i * BYTES_PER_ROW + j] & 0xFF);
    //     }
    //     hex_buffer[remaining * 3] = '\n';
    //     hex_buffer[remaining * 3 + 1] = '\0';

    //     pr_info("%s", hex_buffer);

    //     // Write to the output file
    //     kernel_write(output_file_ptr, hex_buffer, remaining * 3 + 1, &output_file_ptr->f_pos);
    //     bytes_written += remaining;
    // }

    // kfree(hex_buffer);


    // Close the output file
    // if (output_file_ptr) 
    // {
    //     filp_close(output_file_ptr, NULL);

    //     pr_info("loop_device: %s file closed.\n", output_file);
    // }

    // return bytes_written;

    return 0;
}

static struct file_operations loop_fops = {
    .owner = THIS_MODULE,
    .read = loop_read,
    .write = loop_write,
    .open = loop_open,
};

static int __init loop_init(void)
{
    dev_t dev;
    int ret;

    // Request a dynamic major number from the kernel for our device
    ret = alloc_chrdev_region(&dev, 0, 1, LOOP_NAME);

    if (ret < 0) 
    {
        printk(KERN_ERR "Failed to allocate major number\n");
        return ret;
    }

    // Extract the major number from the device identifier (dev)
    major = MAJOR(dev);

    printk(KERN_INFO "My Driver: Registered with major number %d\n", major);

    // Initialize the character device structure and associate it with file operations
    cdev_init(&my_cdev, &loop_fops);
    my_cdev.owner = THIS_MODULE;

    // Add the character device to the kernel
    ret = cdev_add(&my_cdev, dev, 1);
    if (ret < 0) 
    {
        printk(KERN_ERR "Failed to add character device\n");
        unregister_chrdev_region(dev, 1);
        return ret;
    }

    return 0; // Return 0 on success
}

static void __exit loop_exit(void)
{
    // unregister_chrdev(LOOP_MAJOR, LOOP_NAME);

    // Unregister the major number when the module is unloaded
    unregister_chrdev_region(MKDEV(major, 0), 1);
    
    pr_info("loop_device: Device removed\n");
}

module_init(loop_init);
module_exit(loop_exit);
