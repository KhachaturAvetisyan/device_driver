#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define LOOP_MAJOR 0
#define LOOP_NAME "loop_device"
#define BYTES_PER_ROW 16

MODULE_LICENSE("GPL");

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
    char *hex_buffer;
    size_t i, j;
    ssize_t bytes_written = 0;
    int rows = count / BYTES_PER_ROW;
    int remaining = count % BYTES_PER_ROW;

    hex_buffer = kmalloc(BYTES_PER_ROW * 3 + 2, GFP_KERNEL);
    if (!hex_buffer)
        return -ENOMEM;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < BYTES_PER_ROW; j++) {
            sprintf(&hex_buffer[j * 3], "%02X ", buf[i * BYTES_PER_ROW + j] & 0xFF);
        }
        hex_buffer[BYTES_PER_ROW * 3] = '\n';
        hex_buffer[BYTES_PER_ROW * 3 + 1] = '\0';

        if (ppos)
            if (*ppos + bytes_written + count > i * BYTES_PER_ROW)
                break;

        pr_info("%s", hex_buffer);

        // Write to the output file
        kernel_write(output_file_ptr, hex_buffer, BYTES_PER_ROW * 3 + 1, &output_file_ptr->f_pos);
        bytes_written += BYTES_PER_ROW;
    }

    if (remaining > 0) {
        for (j = 0; j < remaining; j++) {
            sprintf(&hex_buffer[j * 3], "%02X ", buf[i * BYTES_PER_ROW + j] & 0xFF);
        }
        hex_buffer[remaining * 3] = '\n';
        hex_buffer[remaining * 3 + 1] = '\0';

        pr_info("%s", hex_buffer);

        // Write to the output file
        kernel_write(output_file_ptr, hex_buffer, remaining * 3 + 1, &output_file_ptr->f_pos);
        bytes_written += remaining;
    }

    kfree(hex_buffer);

    return bytes_written;
}

static struct file_operations loop_fops = {
    .owner = THIS_MODULE,
    .read = loop_read,
    .write = loop_write,
    .open = loop_open,
};

static int __init loop_init(void)
{
    int ret;

    ret = register_chrdev(LOOP_MAJOR, LOOP_NAME, &loop_fops);
    if (ret < 0) {
        pr_err("loop_device: Unable to register character device\n");
        return ret;
    }

    // Open the output file for writing
    output_file_ptr = filp_open(output_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (IS_ERR(output_file_ptr)) {
        pr_err("loop_device: Unable to open output file\n");
        unregister_chrdev(LOOP_MAJOR, LOOP_NAME);
        return PTR_ERR(output_file_ptr);
    }

    pr_info("loop_device: Device created with major %d\n", LOOP_MAJOR);
    return 0;
}

static void __exit loop_exit(void)
{
    // Close the output file
    if (output_file_ptr) {
        filp_close(output_file_ptr, NULL);
    }

    unregister_chrdev(LOOP_MAJOR, LOOP_NAME);
    pr_info("loop_device: Device removed\n");
}

module_init(loop_init);
module_exit(loop_exit);
