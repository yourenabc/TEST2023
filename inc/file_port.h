/*
 * file_port.h
 *
 *  Created on: Mar 8, 2023
 *      Author: LWJ
 */

#ifndef INC_FILE_PORT_H_
#define INC_FILE_PORT_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>

void file_read (char *filename, char *buf, uint32_t len);
void file_write(char *filename, char *buf, uint32_t len);

#endif /* INC_FILE_PORT_H_ */
