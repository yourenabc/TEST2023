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

modbus_t *ctx = NULL;
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
        memset(tab_reg, 0, sizeof(tab_reg));
        //读寄存器设置：寄存器地址、数量、数据缓冲
        regs = modbus_read_registers(ctx, 12304, 2, tab_reg);
        if (regs == -1) 
        {
            fprintf(stderr, "Connexion failed: %s\n", "read");
            continue;
        }
        
        for (i = 0; i < 2; ++i)
        {
            printf("<%#x>", tab_reg[i]);
        }
        printf("\n");
        sleep(2);

	}
	return 0;
}



