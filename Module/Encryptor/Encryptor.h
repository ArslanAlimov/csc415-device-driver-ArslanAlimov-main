/**************************************************************
 * Class:  CSC-415-0 2022
 * Name: Arslan Alimov
 * Student ID: 916612104
 * GitHub Name: Arslan Alimov
 * Project: Assignment 6 – Device Driver
 *
 * File:Encryptor.h
 *
 * Description:  Header file for Device Driver in Kernel Space.
 *               to make code a bit more cleaner
 *
 **************************************************************/
#include <linux/module.h>  //loading module into kernel
#include <linux/kernel.h>  //macroses + functions of kernel
#include <linux/fs.h>      //linux file system
#include <linux/uaccess.h> //copy to user
#include <linux/device.h>
#include <linux/sched.h> // execution scheduling

#define DEVICE_NAME "encryptdecrypt" // The device /dev/EncryptDecrypt
#define CLASS_NAME "encdec"          // This is a character device driver
#define BUFFER_SIZE 1000

/*
device Info when use Modinfo
*/
MODULE_AUTHOR("Arslan Alimov");
MODULE_DESCRIPTION("The Encryption Device Driver");
MODULE_LICENSE("GPL");
MODULE_VERSION("1");

/*
    Setting ups for device
*/
static struct class *myclass = NULL; // device  class struct
static struct device *dev = NULL;    // device struct pointer

/*
user stuff
*/
static int major = 415;        // major number to know what device
static int device_opened = 0;  // counter for number of times the device is opened
static int created_device = 0; // increment for devices created

/*
file_operations prototypes
*/
static int encrypt_open(struct inode *, struct file *);
static ssize_t encrypt_read(struct file *, char *, size_t, loff_t *);
static ssize_t encrypt_write(struct file *, const char *, size_t, loff_t *);
static int encrypt_release(struct inode *, struct file *);
static long encrypt_myIoCtl(struct file *, unsigned int, unsigned long);