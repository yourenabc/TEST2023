/*
 * @Author: laoweijie laoweijie@cyg.com
 * @Date: 2023-03-29 08:54:53
 * @LastEditors: laoweijie laoweijie@cyg.com
 * @LastEditTime: 2023-03-31 09:34:52
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
#include <pthread.h>

#define PhaseStacking 1000   //相位叠加系数

#define  PD_DATA_LEN   2048
#define  PD_ANALYSIS_DATA_LEN   100

#define PDPR_CH1_FILE   "../acu/pdpr_channel1.dat"
#define PDPR_CH2_FILE   "../acu/pdpr_channel2.dat"
#define PDPR_CH3_FILE   "../acu/pdpr_channel3.dat"

#define PDPS_CH1_FILE   "../acu/pdps_channel1.dat"
#define PDPS_CH2_FILE   "../acu/pdps_channel2.dat"
#define PDPS_CH3_FILE   "../acu/pdps_channel3.dat"


extern uint16_t ch1_PD_data[];
extern uint16_t ch2_PD_data[];
extern uint16_t ch3_PD_data[];

typedef struct
{
	uint16_t amplitude;//幅值
	uint16_t phase;    //叠加相位
}prpd_t;

typedef struct
{
	uint16_t amplitude;//幅值  
	uint16_t phase;    //叠加相位	
	uint16_t consult;  //商 -> 周期数

}prps_t;

extern prpd_t ch1_prpd_analysisBuf[];
extern prpd_t ch2_prpd_analysisBuf[];
extern prpd_t ch3_prpd_analysisBuf[];

extern prps_t ch1_prps_analysisBuf[];
extern prps_t ch2_prps_analysisBuf[];
extern prps_t ch3_prps_analysisBuf[];

int read_pd_file_data(modbus_t *ctx, uint16_t fileNum, uint16_t startRecordNum, uint16_t len, uint16_t *desBuf);
int analysis_pd_data_prpd(char *filename, uint16_t *srcdata, prpd_t *desdata, int len);
int analysis_pd_data_prps(char *filename, uint16_t *srcdata, prps_t *desdata, int len);

int prpd_analysis_buf_clear(prpd_t *desdata);
int prps_analysis_buf_clear(prps_t *desdata);

void PD_Pro(void);
void *PD_FILE_TASK_entry(void *param);
void PD_FILE_TASK_Init(void);

#endif


