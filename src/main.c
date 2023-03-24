/*
 * @Author: laoweijie laoweijie@cyg.com
 * @Date: 2023-03-20 13:10:10
 * @LastEditors: laoweijie laoweijie@cyg.com
 * @LastEditTime: 2023-03-24 10:24:41
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

int main()
{
    int i = 0;
    int regs = 0;

    get_module_config(module_config_buf);
    rs485_1_Init();
    rs485_2_Init();
    timerTestInit();
    RS485_1_TASK_Init();
    RS485_2_TASK_Init();

	while(1)
    {
        sleep(2);
	}
	return 0;
}



