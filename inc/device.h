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


int rs485_1_Init(void);
void timerTestInit(void);
int check_data_type (int check);

extern int get_AI(uint8_t*);
extern int get_DI(uint8_t*);
extern int get_DO(uint8_t*);
extern int get_rs485_1(uint16_t *data);
extern int get_rs485_2(uint16_t *data);

extern int GetNowTime(char *);

#endif