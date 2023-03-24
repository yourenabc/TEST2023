/*
 * @Author: laoweijie laoweijie@cyg.com
 * @Date: 2023-03-20 13:10:09
 * @LastEditors: laoweijie laoweijie@cyg.com
 * @LastEditTime: 2023-03-24 14:55:08
 * @FilePath: /modbusTest/inc/CFG_Init.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
 * CFG_Init.h
 *
 *  Created on: Mar 8, 2023
 *      Author: LWJ
 */

#ifndef CFGFILE_INIT_H_
#define CFGFILE_INIT_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include "cJSON.h"

typedef struct
{
	int channel;
	int module_type;
	int data_type;
	double coe;
	int enabled_flag;
	int slave_addr;
	int storage_type;
	int data_addr;
	char device_sn[32];
	int device_type;
}module_config_t;

typedef struct
{
	char passwd[4];              //装置密码
		char key[4];                 //装置密文
		uint32_t longitude;          //经度
		uint32_t latitude;           //纬度
		char plant_code;             //厂区代码
		char ACU_ID[20];             //ACU装置
		char ACU_originID[4];        //ACU原始ID
		char serverID[20];           //服务端ID
		char clientID[20];           //客户端ID
		int  protocol_num;           //上行规约序号
		uint16_t heartbeat_interval; //心跳间隔
		uint16_t sample_interval;	 //采集间隔
		uint16_t report_interval;	 //数据上报间隔
		uint16_t sleep_time;		 //休眠时长
		uint16_t online_time;		 //在线时长
		char hard_reset_time[20];    //硬件重启时间点
		char ip_addr[20];            //ip地址
		uint16_t ip_port;            //ip端口
}set_param_t;

#define module_config_buf_len 50

extern module_config_t g_module_config[];
extern set_param_t module_set_para;

void get_module_config(module_config_t *module_config);
void set_module_config(module_config_t *module_config);
void get_set_para(set_param_t *set_param);
void set_set_param(set_param_t *set_param);

#endif /* CFGFILE_INIT_H_ */
