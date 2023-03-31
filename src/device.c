/**
 * @file device.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-03-13
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "device.h"

#include "string.h"
#include <stdlib.h>
#include "stdio.h"
#include <signal.h>
#include <sys/time.h>
#include "particalDischarge_file.h"

#define RS485_1_USB "/dev/ttyUSB0"
#define RS485_2_USB "/dev/ttyUSB1"

struct itimerval new_timer;

pthread_t rs485_1_task;
pthread_t rs485_2_task;

uint16_t rs485_1_Databuf[256];
uint16_t rs485_2_Databuf[256];
modbus_t *ctx_rs485_1 = NULL;
modbus_t *ctx_rs485_2 = NULL;

int RS485_1_ENABLE_FLAG = 1;
int RS485_2_ENABLE_FLAG = 1;

int rs485_1_Init(void)
{
    //打开端口: 端口，波特率，校验位，数据位，停止位
    ctx_rs485_1 = modbus_new_rtu(RS485_1_USB, 9600, 'N', 8, 1);
    //设置从机地址
    modbus_set_slave(ctx_rs485_1, 1);
    //设置串口模式(可选)
    modbus_rtu_set_serial_mode(ctx_rs485_1, MODBUS_RTU_RS485);
    //建立连接
    if (modbus_connect(ctx_rs485_1) == -1) {
        fprintf(stderr, "Connexion failed: %s\n", "conncet 4851");
        modbus_free(ctx_rs485_1);
        RS485_1_ENABLE_FLAG = 0;
        return -1;
    }
}

int rs485_2_Init(void)
{
    //打开端口: 端口，波特率，校验位，数据位，停止位
    ctx_rs485_2 = modbus_new_rtu(RS485_2_USB, 9600, 'N', 8, 1);
    //设置从机地址
    modbus_set_slave(ctx_rs485_2, 1);
    //设置串口模式(可选)
    modbus_rtu_set_serial_mode(ctx_rs485_2, MODBUS_RTU_RS485);
    //建立连接
    if (modbus_connect(ctx_rs485_2) == -1) {
        fprintf(stderr, "Connexion failed: %s\n", "conncet 4852");
        modbus_free(ctx_rs485_2);
        RS485_2_ENABLE_FLAG = 0;
        return -1;
    }
}

/**
 * @brief get AI
 * 
 * @param AI 
 * @return int 
 */
int get_AI(uint8_t *AI)
{
    memset(AI, 0, sizeof(AI));

    for (int i = 0; i < 256; i++)
    {
        srand((unsigned)time(NULL));
        uint8_t p = rand() % 999 - 100;
        *AI = p;
        AI++;
    }
}

/**
 * @brief get DI
 * 
 * @param DI 
 * @return int 
 */
int get_DI(uint8_t *DI)
{
    memset(DI, 0, sizeof(DI));

    for (int i = 0; i < 256; i++)
    {
        if (i % 2)
            *DI = 1;
        else
            *DI = 0;
        DI++;
    }
}

/**
 * @brief get DO
 * 
 * @param DO 
 * @return int 
 */
int get_DO(uint8_t *DO)
{
    memset(DO, 0, sizeof(DO));

    for (int i = 0; i < 256; i++)
    {
        if (i % 2)
            *DO = 1;
        else
            *DO = 0;
        DO++;
    }
}

/**
 * @brief Get the Now Time object
 * 
 * @param timestr 
 * @return 
 */
int GetNowTime(char *timestr)
{
    time_t nSeconds;
    struct tm *pTM;

    time(&nSeconds); // 同 nSeconds = time(NULL);
    pTM = localtime(&nSeconds);

    /* 系统日期和时间,格式: yyyymmddHHMMSS */
    sprintf(timestr, "%04d-%02d-%02d %02d:%02d:%02d", pTM->tm_year + 1900, pTM->tm_mon + 1,
            pTM->tm_mday, pTM->tm_hour, pTM->tm_min, pTM->tm_sec);
}

int check_data_type (int check)//查看数据类型 是几个字节的数据
{
    switch (check)
    {
        case DTAT_TYPE_U8:
        case DTAT_TYPE_U16_AB:
        case DTAT_TYPE_U16_BA:
            return 1;
            break;

        case DTAT_TYPE_U32_ABCD:
        case DTAT_TYPE_U32_CDAB:
        case DTAT_TYPE_U32_BADC:
        case DTAT_TYPE_U32_DCBA:
        case DTAT_TYPE_FLOAT_ABCD:
        case DTAT_TYPE_FLOAT_CDAB:
        case DTAT_TYPE_FLOAT_BADC:
        case DTAT_TYPE_FLOAT_DCBA:
            return 2;
            break;
    
         default:
            break;
    }
    return 1;
}

int check_addr_consecutive (int check)//查看是否连续
{
    int len =0;
    len = check_data_type(g_module_config[check].data_type);
    if(((g_module_config[check].data_addr + len) == g_module_config[check+1].data_addr) && (g_module_config[check].storage_type == g_module_config[check+1].storage_type))
    {
        if(g_module_config[check].storage_type == 2 || g_module_config[check].storage_type == 1)//遥信量就不要连续去读了 modbus_read_bits()  modbus_read_input_bits() 
        {
            return 0;
        }
        return 1;
    }
    return 0;
}

int send_modbus_cmd (int funcode, modbus_t *ctx, int addr, int nb, uint16_t *dest)//根据功能码进行发送读取指令
{
    int result = 0;
    if(funcode == 1)
    {
        result = modbus_read_bits(ctx, addr, nb, (uint8_t*)dest);
    }
    else if(funcode == 2)
    {
        result = modbus_read_input_bits(ctx, addr, nb, (uint8_t*)dest);
    }
    else if(funcode == 3)
    {
        result = modbus_read_registers(ctx, addr, nb, dest);
    }
    else if(funcode == 4)
    {
        result = modbus_read_input_registers(ctx, addr, nb, dest);
    }
    else
    {
        result = modbus_read_registers(ctx, addr, nb, dest);
    }
    usleep(100000);
    return result;
}

/*
storage_type----->

modbus_read_bits()            fun code:0x01
modbus_read_input_bits()      fun code:0x02
modbus_read_registers()       fun code:0x03
modbus_read_input_registers() fun code:0x04
*/
int get_rs485_1(uint16_t *data)
{
    int ii = 0;
    int rc = 0;
    int single_num = 0;//单独地址读取的个数
    int consecutive_sum = 0;//连续地址读取的个数
    int consecutive_addr_begin = 0;
    int data_offset = 0;

    for(ii = 0; ii<module_config_buf_len; ii++)
    {
        if(g_module_config[ii].enabled_flag == enable && g_module_config[ii].channel == CHANNEL_RS485_1 && g_module_config[ii].module_type < 512 )//当前是否使能且为RS4851
        {
            if((g_module_config[ii+1].enabled_flag == enable) && (g_module_config[ii+1].channel == CHANNEL_RS485_1))  //下一个是否使能且为RS4851
            {
                if(g_module_config[ii].slave_addr == g_module_config[ii+1].slave_addr)
                {
                    if(check_addr_consecutive(ii))//计算地址是否连续
                    {
                        if(consecutive_sum == 0)//记录下首地址
                        {
                            consecutive_addr_begin = g_module_config[ii].data_addr;
                        }
                        single_num = check_data_type(g_module_config[ii].data_type);
                        consecutive_sum = consecutive_sum + single_num;
                        //("485_1 RECORD\r\n");
                    }
                    else
                    {
                        if(consecutive_sum == 0)//如果跟下一个不连续且第一次进来 直接输出
                        {
                            single_num = check_data_type(g_module_config[ii].data_type);
                            modbus_set_slave(ctx_rs485_1, g_module_config[ii].slave_addr);
                            rc = send_modbus_cmd(g_module_config[ii].storage_type, ctx_rs485_1, g_module_config[ii].data_addr, single_num, &data[data_offset]);
                            data_offset = data_offset + single_num;
                            //printf("485_1 SINGLE\r\n");
                        }
                        else
                        {
                            single_num = check_data_type(g_module_config[ii].data_type);
                            consecutive_sum = consecutive_sum + single_num;
                            modbus_set_slave(ctx_rs485_1, g_module_config[ii].slave_addr);
                            rc = send_modbus_cmd(g_module_config[ii].storage_type, ctx_rs485_1, consecutive_addr_begin, consecutive_sum, &data[data_offset]);
                            data_offset = data_offset + consecutive_sum;
                            consecutive_sum = 0;
                            //printf("485_1 consecutive 1\r\n");
                        }
                    }
                }
                else
                {
                    if(consecutive_sum == 0)
                    {
                        single_num = check_data_type(g_module_config[ii].data_type);
                        modbus_set_slave(ctx_rs485_1, g_module_config[ii].slave_addr);
                        rc = send_modbus_cmd(g_module_config[ii].storage_type, ctx_rs485_1, g_module_config[ii].data_addr, single_num, &data[data_offset]);
                        data_offset = data_offset + single_num;
                        //printf("485_1 SALVE ADDR CHANGE\r\n");
                    }
                    else
                    {
                        single_num = check_data_type(g_module_config[ii].data_type);
                        consecutive_sum = consecutive_sum + single_num;
                        modbus_set_slave(ctx_rs485_1, g_module_config[ii].slave_addr);
                        rc = send_modbus_cmd(g_module_config[ii].storage_type, ctx_rs485_1, consecutive_addr_begin, consecutive_sum, &data[data_offset]);
                        data_offset = data_offset + consecutive_sum;
                        consecutive_sum = 0;
                        //printf("485_1 consecutive 3\r\n");  
                    }
                }
            }
            else
            {
                if(consecutive_sum == 0)
                {
                    single_num = check_data_type(g_module_config[ii].data_type);
                    modbus_set_slave(ctx_rs485_1, g_module_config[ii].slave_addr);
                    rc = send_modbus_cmd(g_module_config[ii].storage_type, ctx_rs485_1, g_module_config[ii].data_addr, single_num, &data[data_offset]);
                    data_offset = data_offset + single_num;
                    //printf("485_1 ABC\r\n");
                }
                else
                {
                    single_num = check_data_type(g_module_config[ii].data_type);
                    consecutive_sum = consecutive_sum + single_num;
                    modbus_set_slave(ctx_rs485_1, g_module_config[ii].slave_addr);
                    rc = send_modbus_cmd(g_module_config[ii].storage_type, ctx_rs485_1, consecutive_addr_begin, consecutive_sum, &data[data_offset]);
                    data_offset = data_offset + consecutive_sum;
                    consecutive_sum = 0;
                    //printf("485_1 consecutive 2\r\n");
                }
            }
        }
    }
    return 0;
}

int set_rs485_1(module_config_t *module_config, uint8_t *data)
{
    uint16_t databuf[2] = {0x0000, 0x0000};
    if(module_config->enabled_flag == enable && module_config->channel == CHANNEL_RS485_1)
    {
        modbus_set_slave(ctx_rs485_1, module_config->slave_addr);
        if(module_config->data_type == DTAT_TYPE_U8 && module_config->storage_type == 1)
        {
            modbus_write_bit(ctx_rs485_1, module_config->data_addr, (int)data[0]);
        }
        else if(module_config->data_type == DTAT_TYPE_U16_AB)
        {
            modbus_write_register(ctx_rs485_1, module_config->data_addr, data[0]<<8 | data[1]);
        }
        else if(module_config->data_type == DTAT_TYPE_U16_BA)
        {
            modbus_write_register(ctx_rs485_1, module_config->data_addr, data[1]<<8 | data[0]);
        }
        else if(module_config->data_type == DTAT_TYPE_U32_ABCD)
        {
            databuf[0] = data[0]<<8 | data[1];
            databuf[1] = data[2]<<8 | data[3];
            modbus_write_registers(ctx_rs485_1, module_config->data_addr, 2, databuf);
        }
        else if(module_config->data_type == DTAT_TYPE_U32_CDAB)
        {    
            databuf[0] = data[2]<<8 | data[3];
            databuf[1] = data[1]<<8 | data[0];
            modbus_write_registers(ctx_rs485_1, module_config->data_addr, 2, databuf);
        }
        else if(module_config->data_type == DTAT_TYPE_U32_BADC)
        {
            databuf[0] = data[1]<<8 | data[0];
            databuf[1] = data[3]<<8 | data[2];
            modbus_write_registers(ctx_rs485_1, module_config->data_addr, 2, databuf);
        }
        else if(module_config->data_type == DTAT_TYPE_U32_DCBA)
        {    
            databuf[0] = data[3]<<8 | data[2];
            databuf[1] = data[1]<<8 | data[0];
            modbus_write_registers(ctx_rs485_1, module_config->data_addr, 2, databuf);
        }
        else
        {
            databuf[0] = data[0]<<8 | data[1];
            databuf[1] = data[2]<<8 | data[3];
            modbus_write_registers(ctx_rs485_1, module_config->data_addr, 2, databuf);
        }
    }
    return 0;
}

int get_rs485_2(uint16_t *data)
{
    int ii = 0;
    int rc = 0;
    int single_num = 0;//单独地址读取的个数
    int consecutive_sum = 0;//连续地址读取的个数
    int consecutive_addr_begin = 0;
    int data_offset = 0;

    for(ii = 0; ii<module_config_buf_len; ii++)
    {
        if(g_module_config[ii].enabled_flag == enable && g_module_config[ii].channel == CHANNEL_RS485_2 && g_module_config[ii].module_type < 512)//当前是否使能且为RS4851
        {
            if((g_module_config[ii+1].enabled_flag == enable) && (g_module_config[ii+1].channel == CHANNEL_RS485_2))  //下一个是否使能且为RS4851
            {
                if(g_module_config[ii].slave_addr == g_module_config[ii+1].slave_addr)
                {
                    if(check_addr_consecutive(ii))//计算地址是否连续
                    {
                        if(consecutive_sum == 0)//记录下首地址
                        {
                            consecutive_addr_begin = g_module_config[ii].data_addr;
                        }
                        single_num = check_data_type(g_module_config[ii].data_type);
                        consecutive_sum = consecutive_sum + single_num;
                        printf("485_2 RECORD\r\n");
                    }
                    else
                    {
                        if(consecutive_sum == 0)//如果跟下一个不连续且第一次进来 直接输出
                        {
                            single_num = check_data_type(g_module_config[ii].data_type);
                            modbus_set_slave(ctx_rs485_2, g_module_config[ii].slave_addr);
                            rc = send_modbus_cmd(g_module_config[ii].storage_type, ctx_rs485_2, g_module_config[ii].data_addr, single_num, &data[data_offset]);
                            data_offset = data_offset + single_num;
                            printf("485_2 SINGLE\r\n");
                        }
                        else
                        {
                            single_num = check_data_type(g_module_config[ii].data_type);
                            consecutive_sum = consecutive_sum + single_num;
                            modbus_set_slave(ctx_rs485_2, g_module_config[ii].slave_addr);
                            rc = send_modbus_cmd(g_module_config[ii].storage_type, ctx_rs485_2, consecutive_addr_begin, consecutive_sum, &data[data_offset]);
                            data_offset = data_offset + consecutive_sum;
                            consecutive_sum = 0;
                            printf("485_2 consecutive 1\r\n");
                        }
                    }
                }
                else
                {
                    if(consecutive_sum == 0)
                    {
                        single_num = check_data_type(g_module_config[ii].data_type);
                        modbus_set_slave(ctx_rs485_2, g_module_config[ii].slave_addr);
                        rc = send_modbus_cmd(g_module_config[ii].storage_type, ctx_rs485_2, g_module_config[ii].data_addr, single_num, &data[data_offset]);
                        data_offset = data_offset + single_num;
                        printf("485_2 SALVE ADDR CHANGE\r\n");
                    }
                    else
                    {
                        single_num = check_data_type(g_module_config[ii].data_type);
                        consecutive_sum = consecutive_sum + single_num;
                        modbus_set_slave(ctx_rs485_2, g_module_config[ii].slave_addr);
                        rc = send_modbus_cmd(g_module_config[ii].storage_type, ctx_rs485_2, consecutive_addr_begin, consecutive_sum, &data[data_offset]);
                        data_offset = data_offset + consecutive_sum;
                        consecutive_sum = 0;
                        printf("485_2 consecutive 3\r\n");  
                    }
                }
            }
            else
            {
                if(consecutive_sum == 0)
                {
                    single_num = check_data_type(g_module_config[ii].data_type);
                    modbus_set_slave(ctx_rs485_2, g_module_config[ii].slave_addr);
                    rc = send_modbus_cmd(g_module_config[ii].storage_type, ctx_rs485_2, g_module_config[ii].data_addr, single_num, &data[data_offset]);
                    data_offset = data_offset + single_num;
                    printf("485_2 ABC\r\n");
                }
                else
                {
                    single_num = check_data_type(g_module_config[ii].data_type);
                    consecutive_sum = consecutive_sum + single_num;
                    modbus_set_slave(ctx_rs485_2, g_module_config[ii].slave_addr);
                    rc = send_modbus_cmd(g_module_config[ii].storage_type, ctx_rs485_2, consecutive_addr_begin, consecutive_sum, &data[data_offset]);
                    data_offset = data_offset + consecutive_sum;
                    consecutive_sum = 0;
                    printf("485_2 consecutive 2\r\n");
                }
            }
        }
    }
    return 0;
}

int set_rs485_2(module_config_t *module_config, uint8_t *data)
{
    uint16_t databuf[2] = {0x0000, 0x0000};
    if(module_config->enabled_flag == enable && module_config->channel == CHANNEL_RS485_2)
    {
        modbus_set_slave(ctx_rs485_2, module_config->slave_addr);
        if(module_config->data_type == DTAT_TYPE_U8 && module_config->storage_type == 1)
        {
            modbus_write_bit(ctx_rs485_2, module_config->data_addr, (int)data[0]);
        }
        else if(module_config->data_type == DTAT_TYPE_U16_AB)
        {
            modbus_write_register(ctx_rs485_2, module_config->data_addr, data[0]<<8 | data[1]);
        }
        else if(module_config->data_type == DTAT_TYPE_U16_BA)
        {
            modbus_write_register(ctx_rs485_2, module_config->data_addr, data[1]<<8 | data[0]);
        }
        else if(module_config->data_type == DTAT_TYPE_U32_ABCD)
        {
            databuf[0] = data[0]<<8 | data[1];
            databuf[1] = data[2]<<8 | data[3];
            modbus_write_registers(ctx_rs485_2, module_config->data_addr, 2, databuf);
        }
        else if(module_config->data_type == DTAT_TYPE_U32_CDAB)
        {    
            databuf[0] = data[2]<<8 | data[3];
            databuf[1] = data[1]<<8 | data[0];
            modbus_write_registers(ctx_rs485_2, module_config->data_addr, 2, databuf);
        }
        else if(module_config->data_type == DTAT_TYPE_U32_BADC)
        {
            databuf[0] = data[1]<<8 | data[0];
            databuf[1] = data[3]<<8 | data[2];
            modbus_write_registers(ctx_rs485_2, module_config->data_addr, 2, databuf);
        }
        else if(module_config->data_type == DTAT_TYPE_U32_DCBA)
        {    
            databuf[0] = data[3]<<8 | data[2];
            databuf[1] = data[1]<<8 | data[0];
            modbus_write_registers(ctx_rs485_2, module_config->data_addr, 2, databuf);
        }
        else
        {
            databuf[0] = data[0]<<8 | data[1];
            databuf[1] = data[2]<<8 | data[3];
            modbus_write_registers(ctx_rs485_2, module_config->data_addr, 2, databuf);
        }
    }
    return 0;
}

/*闹钟信号处理函数*/
int timeup_rs4851_flag = 0;
int timeup_rs4852_flag = 0;
void sig_handler(int signal) 
{
    timeup_rs4851_flag = 1;
    timeup_rs4852_flag = 1;
}

void Timer_485_Init(void)
{
    signal(SIGALRM, sig_handler);
    
    new_timer.it_interval.tv_sec = 20;//10s的定时器
    new_timer.it_interval.tv_usec = 0;
    new_timer.it_value.tv_sec = 20;//10s后开始执行
    new_timer.it_value.tv_usec = 0;
    setitimer(ITIMER_REAL, &new_timer, NULL);
}

void Timer_485_Delete(void)
{
    //TODO
}

// uint8_t setsetbuf[4] = {0x35, 0x36, 0x37, 0x38};
// uint8_t setset2buf[4] = {0x31, 0x32, 0x33, 0x34};
// int toggleflag = 0;
void *RS485_1_TASK_entry(void *param)
 {
    while (1) 
    {
        usleep(200000);
        if(timeup_rs4851_flag==1)
        {
            timeup_rs4851_flag = 0;
            int i = 0;
            memset(rs485_1_Databuf, 0x00, sizeof(rs485_1_Databuf));
            get_rs485_1(rs485_1_Databuf);

            printf("rs485_1 read done \n");
            for (i = 0; i < 16; ++i)
            {
                printf("<%#x>", rs485_1_Databuf[i]);
            }
            printf("\n");
            printf("rs485_1 print done \n");
            // if(toggleflag == 0)
            // {
            //     toggleflag = 1;
            //     set_rs485_1(&g_module_config[10], setsetbuf);
            //     printf("rs485_1 SET on done \n");
            // }
            // else if(toggleflag == 1)
            // {
            //     toggleflag = 0;
            //     set_rs485_1(&g_module_config[10], setset2buf);
            //     printf("rs485_1 SET off done \n");
            // }
        }
    }
 }

void *RS485_2_TASK_entry(void *param)
 {
    while (1) 
    {
        usleep(200000);
        if(timeup_rs4852_flag==1)
        {
            timeup_rs4852_flag = 0;
            int i = 0;
            get_rs485_2(rs485_2_Databuf);
            printf("rs485_2 read done \n");

            for (i = 0; i < 18; ++i)
            {
                printf("<%#x>", rs485_2_Databuf[i]);
            }
            printf("\n");

            printf("rs485_2 print done \n");
        }
    }
 }

void RS485_1_TASK_Init(void)
 {
    pthread_create(&rs485_1_task, NULL, RS485_1_TASK_entry, NULL);
 }

void RS485_2_TASK_Init(void)
 {
    pthread_create(&rs485_2_task, NULL, RS485_2_TASK_entry, NULL);
 }

void RS485_1_TASK_Delete(void)
 {
    pthread_cancel(rs485_1_task);
 }

 void RS485_2_TASK_Delete(void)
 {
    pthread_cancel(rs485_2_task);
 }

void RS485_INIT(void)
{
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
}

void RS485_DELETE(void)
{
    // Timer_485_Delete();
    RS485_1_TASK_Delete();
    RS485_2_TASK_Delete();
}