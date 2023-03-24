/*
 * @Author: laoweijie laoweijie@cyg.com
 * @Date: 2023-03-20 13:10:10
 * @LastEditors: laoweijie laoweijie@cyg.com
 * @LastEditTime: 2023-03-24 15:03:31
 * @FilePath: /modbusTest/src/main.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AEs
 */
#include "cJSON.h"
#include "CFG_Init.h"
#include "modbus.h"
#include "device.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h>
#include <stdint.h>
#include <string.h>


uint16_t tab_reg[64] = {0};

extern int RS485_1_ENABLE_FLAG;
extern int RS485_2_ENABLE_FLAG;
int main()
{
    int i = 0;
    int regs = 0;

    get_module_config(g_module_config);
    rs485_1_Init();
    rs485_2_Init();
    Timer_485_Init();
    if(RS485_1_ENABLE_FLAG == 1)
    {
        RS485_1_TASK_Init();
    }
    if(RS485_2_ENABLE_FLAG == 1)
    {
       RS485_2_TASK_Init();
    }
    if(RS485_1_ENABLE_FLAG == 0)
    {
        printf("rs485_1 not found \n");
    }
    if(RS485_2_ENABLE_FLAG == 0)
    {
        printf("rs485_2 not found \n");
    }

	while(1)
    {
        sleep(2);
	}
	return 0;
}



