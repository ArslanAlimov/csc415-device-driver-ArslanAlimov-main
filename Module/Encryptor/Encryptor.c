/**************************************************************
 * Class:  CSC-415-0 2022
 * Name: Arslan Alimov
 * Student ID: 916612104
 * GitHub Name: Arslan Alimov
 * Project: Assignment 6 – Device Driver
 *
 * File:Encryptor.c
 *
 * Description:  Device Driver in Kernel Space.
 *               Wanted to create device driver that encrypts however didn't figure out how to. The code for encryption is in my Test Code
 *               the usage for it is stringEncrypt(10,message)
 *
 *
 **************************************************************/
#include "Encryptor.h"

// Struct for file operations

struct file_operations file_operations = {
    .open = encrypt_open,
    .owner = THIS_MODULE,
    .release = encrypt_release,
    .write = encrypt_write,
    .read = encrypt_read,
    .unlocked_ioctl = encrypt_myIoCtl,
};

/*
need to create a function which will out put 0 if everything was sucessfull otherwise it will return error
*/

static int encrypt_init(void)
{
    // allocate major number for device
    int device = register_chrdev(major, DEVICE_NAME, &file_operations);
    pr_info("Encryption & Decryption Device Initializing\n");


    if (device < 0)
    {
        // if our major is less than 0 , 0 is our default value since we know that our 415 is a major number
        pr_alert("Error can't assign register number\n");
        return major;
    }
    // if the major number > 0 then our device was successfully registered with that number which is 415
    pr_info("Encryption & Decryption registered by the number of  %d\n", major);
    created_device++;
    myclass = class_create(THIS_MODULE, CLASS_NAME);
    /*
    IS_ERR - used to check for errors if it outputs 0 then no errors
    check for errors . it is a macros
    Error handling
    */
    if (IS_ERR(myclass))
    {
        // unregister char device
        unregister_chrdev(major, DEVICE_NAME);
        pr_alert("Failed to register device class\n");

        /*
        https://www.kernel.org/doc/htmldocs/kernel-hacking/convention-returns.html
        correct way to return ptr
        */
        return PTR_ERR(myclass);
    }
    pr_info("No Errors! Successfull registration\n");
    // register our device driver store into variable for easier access so we don't have to type each time for if statement
    // if is_err(device_create(myclass, NULL, MKDEV(major, 0), NULL, DEVICE_NAME)) e.t.c too much time consuming creates a device /dev/encryptdecrypt
    dev = device_create(myclass, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    /*
     Error handling
     check for device creation if there is any error
     if there is an error then we unregister device
     destroy the class for device
     return the error if its not 0 then there was some error in our call.
    */
    if (IS_ERR(dev))
    {
        /*
        https://www.kernel.org/doc/htmldocs/kernel-api/API---unregister-chrdev.html
        */
        unregister_chrdev(major, DEVICE_NAME);
        /*
        https://docs.huihoo.com/linux/kernel/2.6.26/kernel-api/re596.html
        */
        class_destroy(myclass);

        pr_err("Failed to create the device\n");
        // returns ptr error
        return PTR_ERR(dev);
    }
    pr_info(" Class was created correctly\n");
    return 0;
}
static int encrypt_open(struct inode *inod, struct file *fs)
{
    /*
    We got rid of global buffer and store our buffer in fs->private_data
    */
    char *ThisBuffer = vmalloc(BUFFER_SIZE);
    fs->private_data = ThisBuffer;
    /*
    Increment device opened by 1 each time our device is being opened
    */
    device_opened++;
    pr_info("This Encryption device has been opened - %d times \n", device_opened);
    return 0;
}

static ssize_t encrypt_read(struct file *fs, char *userBuffer, size_t size, loff_t *offset)
{
    /*
    copy to user ( * to, *from, size) copies to the users buffer from the input that is being given in our case it is the message, provided input size
    similar concept to memcpy
    */
    char * ThisBuffer = fs->private_data;
    // we are calculating bytes we are taking the size of our passed data how much it weight we minus that from what we copied to user to get what data we are on right now
    // similar concept to our 2b assignment
    int bytes = size - copy_to_user(ThisBuffer, userBuffer + *offset, size);
    //we are adding to our offset so its the same as bytes and our offest is basically the position 
    *offset += bytes;
    //print info into dmesg or syslog
    pr_info("Read device encrypt bytes %d, offset=%d:\n\n", bytes, (int)*offset);
    return bytes;
}
static int encrypt_release(struct inode *inode, struct file *fs)
{
     // storing buffer in fs-> private data to see if we even have anything
    char * ThisBuffer = fs->private_data;
    //output message to our dmesg
    pr_info("Encrypt Device was closed\n");

    //output message to dmesg
    pr_info("Freeing the  Buffer");
    //check if there is buffer then free
    if (ThisBuffer)
        vfree(ThisBuffer);
        //set buffers to null because for every malloc there should be a null teacher taught us
    ThisBuffer = NULL;
    fs->private_data = NULL;
    return 0;
}

static ssize_t encrypt_write(struct file *fs, const char *userBuffer, size_t size, loff_t *offset)
{

    /*
    We got rid of global buffer and store our buffer in fs->private_data
    */
    char * ThisBuffer = fs->private_data;
    //same concept as above where we calculate bytes that are being written
    int bytes = size - copy_from_user(ThisBuffer + *offset, userBuffer, size);
    *offset += bytes;
    /*
    set size of our input to the size of our buffer that was defined at the beginning
    */
    pr_info("User asked for %zu  characters \n\n Written %d bytes ,offset = %d", size, bytes, (int)*offset);
    // print how many characters user tried to ask for
    return bytes;
}

static void encrypt_exit(void)
{
    /*
    remove the device
    unregister class that was assigned to the device
    destroy the class
    unregister the chr device
    if everything was sucessfull then  we output the message
    we don't need to output anything other than the message if the device was sucessfully exited
    */
    device_destroy(myclass, MKDEV(major, 0));
    class_unregister(myclass);
    class_destroy(myclass);
    unregister_chrdev(major, DEVICE_NAME);
    pr_info("Encrypt Device was exited properly \n");
}

static long encrypt_myIoCtl(struct file *fs, unsigned int commands, unsigned long data)
{
    switch (commands)
    {
    case 1:
        break;
    case 2:
        break;
    default:
        break;
    }

    return 0;
}

module_init(encrypt_init);
module_exit(encrypt_exit);

/*
https://www.kernel.org/doc/html/latest/core-api/printk-basics.html
*/
