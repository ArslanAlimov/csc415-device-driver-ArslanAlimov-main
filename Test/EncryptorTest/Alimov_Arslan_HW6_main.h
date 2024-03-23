/**************************************************************
* Class:  CSC-415-02
* Name: Arslan Alimov
* Student ID: 916612104
* GitHub Name: Arslan Alimov
* Project: Assignment 6 – Device Driver
*
* File: alimov_arslan_hw6_main.h
*
* Description:  Header file for Testing device driver
*               
*
**************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 1000
static char input[1000] = {0}; // Memory for the string that will be passed from user
// Prototypes for encrypt decrypt int its our key or "salt" char is our input[]
void stringEncrypt(int,char []);
void stringDecrypt(int,char[]);