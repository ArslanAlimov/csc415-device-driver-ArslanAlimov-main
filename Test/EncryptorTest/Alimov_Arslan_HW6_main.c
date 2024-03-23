/**************************************************************
 * Class:  CSC-415-0 2022
 * Name: Arslan Alimov
 * Student ID: 916612104
 * GitHub Name: Arslan Alimov
 * Project: Assignment 6 – Device Driver
 *
 * File:Alimov_Arslan_HW6_main.c
 *
 * Description:  Device Test in Kernel Space.
 *
 *
 **************************************************************/

#include "Alimov_Arslan_HW6_main.h"
/*
Simple encryption / decryption metods
we are using a seed it has to be any number
create a counter for our for loop
check for end of string (length) could do it different way such as for int counter ; input[counter] != "\0"
as we know our strings are consist of ASCII characters when we put it all in a string our for loop will go through our string and add the seed(key) for encryption
or decryption
for example if A  065 + key = 65 +3 = 67
and to get it back to normal we need to - those 3 which was our key or seed
could do one method with a switch case, but would be messier code
*/
void stringEncrypt(int seed, char input[])
{
    int counter;
    for (int counter = 0; strlen(input); counter++)
    {
        input[counter] = input[counter] + seed;
    }
}
void stringDecrypt(int seed, char input[])
{
    int counter;
    for (int counter = 0; strlen(input); counter++)
    {
        input[counter] = input[counter] - seed;
    }
}


int main(int argc, char *argv[])
{
    int fd;
    printf("Enter the string to encrypt: \n");
    scanf("%s", input);
    fd = open("/dev/encryptdecrypt", O_RDWR);
    if (strlen(input)>0)
    {
        printf("\nWritten method called: %ld\n ", write(fd, input, strlen(input)));
    }

    read(fd,input,strlen(input));

    printf("\nOutput %s\n",input);
    
    close(fd);

    return 0;
}
