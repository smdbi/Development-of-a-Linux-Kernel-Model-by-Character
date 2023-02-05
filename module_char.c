
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>

#define DEVICE_NAME "mhedhbi_device"

static int major_number;
static char *name = "Mhedhbi";
static char *name_printer = " Personne inconnue ";
static char *name_reader;
//static char *file_name = "/etc/name"; declare var static pour stocker file name

static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);

static int dev_open(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "Module opened\n");
    return 0;
}
static ssize_t name_read(struct file *file, char __user *user_buf, size_t count, loff_t *ppos)
{
    printk(KERN_INFO "Reading name...\n");
    name_reader = name_printer;
    if (strcmp(name_reader, name) == 0)
    {
        name_reader = " Siwar ";
    }

    copy_to_user(user_buf, name_reader, strlen(name_reader) + 1); // Copie le nom à lire dans le tampon utilisateur.

    return strlen(name_reader) + 1; // Renvoie la longueur du nom à lire
}

static ssize_t name_write(struct file *file, const char __user *user_buf, size_t count, loff_t *ppos)
{
    char buf[count];// Déclare un tampon pour stocker le nom.

    copy_from_user(buf, user_buf, count);
    buf[count - 1] = '\0'; // Ajoute un caractère de fin de chaîne au tampon.

    printk(KERN_INFO "Writing name...\n");
    if (strcmp(buf, name) == 0)
    {
        name_printer = " Siwar ";// Affecte le prénom à la variable de nom à imprimer si les noms sont égaux.

    }
    else
    {
        name_printer = " Personne inconnue "; // Affecte la chaîne "Personne inconnue" à la variable de nom à imprimer si les noms sont différents.

    }

    return count;
}

static const struct file_operations fops = {
    .open = dev_open,
    .owner = THIS_MODULE,
    .read = name_read,
    .write = name_write,
    .release = dev_release,
};

static int __init name_init(void)
{
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "Module failed to unload: %d\n", major_number);
        return major_number;
    }
    printk(KERN_INFO "Module loaded successfully with a major number= %d\n",major_number);
    return 0;
}

static void __exit name_exit(void)
{
    printk(KERN_INFO "Unloading module...\n");
    unregister_chrdev(major_number, DEVICE_NAME);
    printk(KERN_INFO "Module unloaded successfully\n");
}

static int dev_release(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "Module closed\n");
    return 0;
}

module_init(name_init);
module_exit(name_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Siwar Mhedhbi");
MODULE_DESCRIPTION("Module noyau linux par caractère");
