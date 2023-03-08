/*
 * file_port.c
 *
 *  Created on: Mar 8, 2023
 *      Author: LWJ
 */

#include "file_port.h"

void file_read (char *filename, char *buf, uint32_t len)
{
    int fd = open(filename, O_RDONLY);
    if(fd == -1)
	{
		printf("open readfile fail\n\r");
	}
	else
	{
		printf("open readfile succeed\n\r");
	}

	read(fd, buf, len);

	close(fd);
}

void file_write(char *filename, char *buf, uint32_t len)
{
    int fd = open(filename, O_WRONLY|O_TRUNC|O_CREAT, 0664);
    if(fd == -1)
	{
		printf("open writefile fail\n\r");
	}
	else
	{
		printf("open writefile succeed\n\r");
	}
    write(fd, buf, len);

    close(fd);
}


