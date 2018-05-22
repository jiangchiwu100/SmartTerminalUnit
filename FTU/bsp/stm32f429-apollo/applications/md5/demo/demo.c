/* 
 * main.c 
 * 
 *  Created on: 2014-11-6 
 *      Author: HZY 
 */  
  
  
#include <stdio.h>  
#include <string.h>  
  
#include "md5.h"  
  
int main(int c, char** v)
{  
    char buffer[128];  
    getStringMD5("hello world", buffer);  
    printf("%s\n", buffer);  
    getFileMD5("hello.pdf", buffer);  
    printf("%s\n", buffer);  
    
    getchar();
    return 0;  
}  