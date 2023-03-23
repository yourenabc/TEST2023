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
    timerTestInit();

	while(1)
    {
        sleep(2);
	}
	return 0;
}



