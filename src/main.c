#include "cJSON.h"
#include "CFG_Init.h"
#include "modbus.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <termios.h>
#include <stdint.h>
#include<string.h>

modbus_t *ctx = NULL;
uint16_t tab_reg[64] = {0};

int main()
{
    int i = 0;
    int regs = 0;
    //打开端口: 端口，波特率，校验位，数据位，停止位
    ctx = modbus_new_rtu("/dev/ttyUSB0", 9600, 'N', 8, 1);
    //设置从机地址
    modbus_set_slave(ctx, 1);
    //设置串口模式(可选)
    modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
    //建立连接
    if (modbus_connect(ctx) == -1) {
        fprintf(stderr, "Connexion failed: %s\n", "conncet");
        modbus_free(ctx);
        return -1;
    }

	while(1)
    {
        memset(tab_reg, 0, sizeof(tab_reg));
        //读寄存器设置：寄存器地址、数量、数据缓冲
        regs = modbus_read_registers(ctx, 12304, 4, tab_reg);
        if (regs == -1) 
        {
            fprintf(stderr, "Connexion failed: %s\n", "read");
            continue;
        }
        
        for (i = 0; i < 4; ++i)
        {
            printf("<%#x>", tab_reg[i]);
        }
        printf("\n");
        sleep(2);

	}
	return 0;
}



