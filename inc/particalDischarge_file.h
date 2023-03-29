/*
 * @Author: laoweijie laoweijie@cyg.com
 * @Date: 2023-03-29 08:54:53
 * @LastEditors: laoweijie laoweijie@cyg.com
 * @LastEditTime: 2023-03-29 14:59:52
 * @FilePath: /modbusTest/inc/particalDischarge_file.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#ifndef PARTICALDISCHARGE_FILE_H_
#define PARTICALDISCHARGE_FILE_H_

#include "string.h"
#include <stdlib.h>
#include "stdio.h"
#include "device.h"
#include "modbus.h"
#include "file_port.h"

#define PhaseStacking 1000   //相位叠加系数

#define  PD_DATA_LEN   2048
extern uint16_t channel_1_PD[];
extern uint16_t channel_2_PD[];
extern uint16_t channel_3_PD[];

typedef struct
{
	uint16_t amplitude;//幅值
	uint16_t phase;    //叠加相位
}prpd_t;

typedef struct
{
	uint16_t amplitude;//幅值
	uint16_t phase;    //叠加相位
    uint16_t consult;  //商
}prps_t;

extern prpd_t channel_1_prpdBuf[];
extern prpd_t channel_2_prpdBuf[];
extern prpd_t channel_3_prpdBuf[];

extern prps_t channel_1_prpsBuf[];

int read_pd_file_data(modbus_t *ctx, uint16_t fileNum, uint16_t startRecordNum, uint16_t len, uint16_t *desBuf);
int analysis_pd_data_prpd(uint16_t *srcdata, prpd_t *desdata, int len);
int analysis_pd_data_prps(uint16_t *srcdata, prps_t *desdata, int len);

#endif


