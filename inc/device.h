/*
 * @Author: laoweijie laoweijie@cyg.com
 * @Date: 2023-03-24 13:16:22
 * @LastEditors: laoweijie laoweijie@cyg.com
 * @LastEditTime: 2023-03-31 09:06:14
 * @FilePath: /acu/inc/device.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/**
 * @file device.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef DEVICE_H_
#define DEVICE_H_

#include "stdint.h"
#include <stdio.h>
#include "CFG_Init.h"
#include "modbus.h"
#include <pthread.h>

#define enable  1
#define disable 0

#define CHANNEL_RS485_1 97
#define CHANNEL_RS485_2 98

#define DTAT_TYPE_U8         1 //1是u8
#define DTAT_TYPE_U16_AB     2 //2-3是u16
#define DTAT_TYPE_U16_BA     3 
#define DTAT_TYPE_U32_ABCD   4 //4-7是u32
#define DTAT_TYPE_U32_CDAB   5 
#define DTAT_TYPE_U32_BADC   6 
#define DTAT_TYPE_U32_DCBA   7 
#define DTAT_TYPE_FLOAT_ABCD 8 //8-11是float
#define DTAT_TYPE_FLOAT_CDAB 9
#define DTAT_TYPE_FLOAT_BADC 10
#define DTAT_TYPE_FLOAT_DCBA 11

extern modbus_t *ctx_rs485_1;
extern modbus_t *ctx_rs485_2;

int rs485_1_Init(void);
int rs485_2_Init(void);


int get_rs485_1(uint16_t *data);
int set_rs485_1(module_config_t *module_config, uint8_t *data);
int get_rs485_2(uint16_t *data);
int set_rs485_2(module_config_t *module_config, uint8_t *data);
int GetNowTime(char *);

int check_data_type (int check);
int check_addr_consecutive (int check);
int send_modbus_cmd (int funcode, modbus_t *ctx, int addr, int nb, uint16_t *dest);
void sig_handler(int signal);
void Timer_485_Init(void);
void Timer_485_Delete(void);
void *RS485_1_TASK_entry(void *param);
void *RS485_2_TASK_entry(void *param);
void RS485_1_TASK_Init(void);
void RS485_2_TASK_Init(void);
void RS485_1_TASK_Delete(void);
void RS485_2_TASK_Delete(void);
void RS485_INIT(void);
void RS485_DELETE(void);

#endif
