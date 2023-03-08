/*
 * CFG_Init.c
 *
 *  Created on: Mar 8, 2023
 *      Author: LWJ
 */
#include "CFG_Init.h"
#include "file_port.h"

module_config_t module_config_buf[40];
set_param_t module_set_para;
int module_config_num_g = 0;//实例接口的数量

char json_string_cfg_read[8192];
char json_string_set_read[2048];

/**
 * 获取config文件
 * @param  module_config  读取的数据解析后需要存放的位置
 * @return
 */
void get_module_config(module_config_t *module_config)
{
	memset(json_string_cfg_read, 0x00, sizeof(json_string_cfg_read));
	file_read("config.txt", json_string_cfg_read, sizeof(json_string_cfg_read));

	cJSON* cjson_config = cJSON_Parse(json_string_cfg_read);
	if(cjson_config == NULL)
	{
		printf("cjson_config error...\r\n");
		return;
	}
	else
	{
		printf("%s\n",cJSON_Print(cjson_config));
	}

	cJSON* MODULE = cJSON_GetObjectItem(cjson_config,"module_config");
	int MODULE_size = cJSON_GetArraySize(MODULE);
	module_config_num_g= MODULE_size;
	cJSON* MODULE_item = MODULE->child;
	int ii = 0;
	for(ii = 0; ii < MODULE_size; ii++)
	{
		cJSON *json_tmp;

		json_tmp = cJSON_GetObjectItem(MODULE_item ,"channel");
		if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
		{
		    module_config[ii].channel = json_tmp->valueint;
		    printf("%d ",module_config[ii].channel);
		}

		json_tmp = cJSON_GetObjectItem(MODULE_item ,"module_type");
		if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
		{
			module_config[ii].module_type = json_tmp->valueint;
			printf("%d ",module_config[ii].module_type);
		}

		json_tmp = cJSON_GetObjectItem(MODULE_item ,"data_type");
		if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
		{
			module_config[ii].data_type = json_tmp->valueint;
			printf("%d ",module_config[ii].data_type);
		}

		json_tmp = cJSON_GetObjectItem(MODULE_item ,"coe");
		if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
		{
			module_config[ii].coe = json_tmp->valuedouble;
			printf("%f ",json_tmp->valuedouble);
		}

		json_tmp = cJSON_GetObjectItem(MODULE_item ,"enable_flag");
		if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
		{
			module_config[ii].enabled_flag = json_tmp->valueint;
			printf("%d ",module_config[ii].enabled_flag);
		}

		json_tmp = cJSON_GetObjectItem(MODULE_item ,"slave_addr");
		if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
		{
			module_config[ii].slave_addr = json_tmp->valueint;
			printf("%d ",module_config[ii].slave_addr);
		}

		json_tmp = cJSON_GetObjectItem(MODULE_item ,"storage_type");
		if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
		{
			module_config[ii].storage_type = json_tmp->valueint;
			printf("%d ",module_config[ii].storage_type);
		}

		json_tmp = cJSON_GetObjectItem(MODULE_item ,"data_addr");
		if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
		{
			module_config[ii].data_addr = json_tmp->valueint;
			printf("%d ",module_config[ii].data_addr);
		}

        json_tmp = cJSON_GetObjectItem(MODULE_item ,"device_sn");
        if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
        {
        	memset(module_config[ii].device_sn , 0x00, 32);
            memcpy( module_config[ii].device_sn, json_tmp->valuestring, strlen(json_tmp->valuestring));
            printf("%s  ", module_config[ii].device_sn);
        }

        json_tmp = cJSON_GetObjectItem(MODULE_item ,"device_type");
        if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
        {
            module_config[ii].device_type = json_tmp->valueint;
            printf("%d \r\n",module_config[ii].device_type);
        }
		MODULE_item = MODULE_item ->next;
	}
	cJSON_Delete(cjson_config);//清除结构体
}

/**
 * 更新config文件
 * @param  module_config  需要更新的结构体数据到配置文件
 * @return
 */
void  set_module_config(module_config_t *module_config)
{
	cJSON *cjson_config = cJSON_CreateObject();

	cJSON *MODULE = cJSON_CreateArray();

	cJSON_AddItemToObject(cjson_config, "module_config", MODULE);
	int ii = 0;
	for(ii = 0; ii < module_config_num_g; ii++)
	{
		cJSON *MODULE_LIST;
		MODULE_LIST = cJSON_CreateObject();
		cJSON_AddNumberToObject(MODULE_LIST, "channel", module_config[ii].channel);
		cJSON_AddNumberToObject(MODULE_LIST, "module_type", module_config[ii].module_type);
		cJSON_AddNumberToObject(MODULE_LIST, "data_type", module_config[ii].data_type);
		cJSON_AddNumberToObject(MODULE_LIST, "coe", module_config[ii].coe);
		cJSON_AddNumberToObject(MODULE_LIST, "enable_flag", module_config[ii].enabled_flag);
		cJSON_AddNumberToObject(MODULE_LIST, "slave_addr", module_config[ii].slave_addr);
		cJSON_AddNumberToObject(MODULE_LIST, "storage_type", module_config[ii].storage_type);
		cJSON_AddNumberToObject(MODULE_LIST, "data_addr", module_config[ii].data_addr);
        cJSON_AddStringToObject(MODULE_LIST, "device_sn", module_config[ii].device_sn);
        cJSON_AddNumberToObject(MODULE_LIST, "device_type", module_config[ii].device_type);
		cJSON_AddItemToArray(MODULE, MODULE_LIST);
	}

	char *json_data = cJSON_Print(cjson_config);	//JSON数据结构转换为JSON字符串

	file_write("config.txt", json_data, strlen(json_data));
	printf("%s\r\n",json_data);//输出字符串

	cJSON_free(json_data);
	cJSON_Delete(cjson_config);//清除结构体
}

/**
 * 获取set文件 先读取再写入实例化结构体
 * @param  set_param  读取的数据解析后需要存放的位置
 * @return
 */
void get_set_para(set_param_t *set_param)
{
	memset(json_string_set_read, 0x00, sizeof(json_string_set_read));
	file_read("set.txt", json_string_set_read, sizeof(json_string_set_read));
	
	cJSON* cjson_config = cJSON_Parse(json_string_set_read);
	if(cjson_config == NULL)
	{
		printf("cjson_config error...\r\n");
		return;
	}
	else
	{
		printf("%s\n",cJSON_Print(cjson_config));
	}
	cJSON *json_tmp;

	json_tmp = cJSON_GetObjectItem( cjson_config,"passwd");
	if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
	{
		memcpy(set_param->passwd, json_tmp->valuestring, strlen(json_tmp->valuestring));
		printf("passwd:%s\r\n",set_param->passwd);
	}

	json_tmp = cJSON_GetObjectItem( cjson_config,"key");
	if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
	{
		memcpy(set_param->key, json_tmp->valuestring, strlen(json_tmp->valuestring));
		printf("key:%s\r\n",set_param->key );
	}

	json_tmp = cJSON_GetObjectItem( cjson_config,"longitude");
	if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
	{
		set_param->longitude=json_tmp->valueint;
		printf("longitude:%d\r\n",set_param->longitude );
	}

	json_tmp = cJSON_GetObjectItem( cjson_config,"latitude");
	if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
	{
		set_param->latitude=json_tmp->valueint;
		printf("latitude:%d\r\n",set_param->latitude );
	}

	json_tmp = cJSON_GetObjectItem( cjson_config,"plant_code");
	if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
	{
		set_param->plant_code=json_tmp->valueint;
		printf("plant_code:%d\r\n",set_param->plant_code );
	}

	json_tmp = cJSON_GetObjectItem( cjson_config,"ACU_ID");
	if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
	{
		memcpy(set_param->ACU_ID, json_tmp->valuestring, strlen(json_tmp->valuestring));
		printf("ACU_ID:%s\r\n",set_param->ACU_ID );
	}

	json_tmp = cJSON_GetObjectItem( cjson_config,"ACU_originID");
	if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
	{
		memcpy(set_param->ACU_originID, json_tmp->valuestring, strlen(json_tmp->valuestring));
		printf("ACU_originID:%s\r\n",set_param->ACU_originID);
	}


	json_tmp = cJSON_GetObjectItem( cjson_config,"serverID");
	if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
	{
		memcpy(set_param->serverID, json_tmp->valuestring, strlen(json_tmp->valuestring));
		printf("serverID:%s\r\n",set_param->serverID);
	}

	json_tmp = cJSON_GetObjectItem( cjson_config,"clientID");
	if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
	{
		memcpy(set_param->clientID, json_tmp->valuestring, strlen(json_tmp->valuestring));
		printf("clientID:%s\r\n",set_param->clientID);
	}

	json_tmp = cJSON_GetObjectItem( cjson_config,"protocol_num");
	if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
	{
		set_param->protocol_num=json_tmp->valueint;
		printf("protocol_num:%d\r\n",set_param->protocol_num );
	}

	json_tmp = cJSON_GetObjectItem( cjson_config,"heartbeat_interval");
	if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
	{
		set_param->heartbeat_interval=json_tmp->valueint;
		printf("heartbeat_interval:%d\r\n",set_param->heartbeat_interval );
	}

	json_tmp = cJSON_GetObjectItem( cjson_config,"sample_interval");
	if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
	{
		set_param->sample_interval=json_tmp->valueint;
		printf("sample_interval:%d\r\n",set_param->sample_interval );
	}

	json_tmp = cJSON_GetObjectItem( cjson_config,"report_interval");
	if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
	{
		set_param->report_interval=json_tmp->valueint;
		printf("report_interval:%d\r\n",set_param->report_interval );
	}

	json_tmp = cJSON_GetObjectItem( cjson_config,"sleep_time");
	if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
	{
		set_param->sleep_time=json_tmp->valueint;
		printf("sleep_time:%d\r\n",set_param->sleep_time );
	}

	json_tmp = cJSON_GetObjectItem( cjson_config,"online_time");
	if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
	{
		set_param->online_time=json_tmp->valueint;
		printf("online_time:%d\r\n",set_param->online_time );
	}

	json_tmp = cJSON_GetObjectItem( cjson_config,"hard_reset_time");
	if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
	{
		memcpy(set_param->hard_reset_time, json_tmp->valuestring, strlen(json_tmp->valuestring));
		printf("hard_reset_time:%s\r\n",set_param->hard_reset_time);
	}

	json_tmp = cJSON_GetObjectItem( cjson_config,"ip_addr");
	if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
	{
		memcpy(set_param->ip_addr, json_tmp->valuestring, strlen(json_tmp->valuestring));
		printf("ip_addr:%s\r\n",set_param->ip_addr);
	}


	json_tmp = cJSON_GetObjectItem( cjson_config,"ip_port");
	if(json_tmp != NULL && json_tmp->type  != cJSON_NULL)
	{
		set_param->ip_port=json_tmp->valueint;
		printf("ip_port:%d\r\n",set_param->ip_port );
	}

	cJSON_Delete(cjson_config);
}

/**
 * 更新set文件
 * @param  set_param  需要更新的结构体数据到配置文件
 * @return
 */
void set_set_param(set_param_t *set_param)
{
	cJSON *cjson_set = cJSON_CreateObject();

	cJSON_AddStringToObject(cjson_set, "passwd", set_param->passwd);
	cJSON_AddStringToObject(cjson_set, "key", set_param->key);
	cJSON_AddNumberToObject(cjson_set, "longitude", set_param->longitude);
	cJSON_AddNumberToObject(cjson_set, "latitude", set_param->latitude);
	cJSON_AddNumberToObject(cjson_set, "plant_code", set_param->plant_code);
	cJSON_AddStringToObject(cjson_set, "ACU_ID", set_param->ACU_ID);
	cJSON_AddStringToObject(cjson_set, "ACU_originID", set_param->ACU_originID);
	cJSON_AddStringToObject(cjson_set, "serverID", set_param->serverID);
	cJSON_AddStringToObject(cjson_set, "clientID", set_param->clientID);
	cJSON_AddNumberToObject(cjson_set, "protocol_num", set_param->protocol_num);
	cJSON_AddNumberToObject(cjson_set, "heartbeat_interval", set_param->heartbeat_interval);
	cJSON_AddNumberToObject(cjson_set, "sample_interval", set_param->sample_interval);
	cJSON_AddNumberToObject(cjson_set, "report_interval", set_param->report_interval);
	cJSON_AddNumberToObject(cjson_set, "sleep_time", set_param->sleep_time);
	cJSON_AddNumberToObject(cjson_set, "online_time", set_param->online_time);
	cJSON_AddStringToObject(cjson_set, "hard_reset_time", set_param->hard_reset_time);
	cJSON_AddStringToObject(cjson_set, "ip_addr", set_param->ip_addr);
	cJSON_AddNumberToObject(cjson_set, "ip_port", set_param->ip_port);

	char *json_data = cJSON_Print(cjson_set);	//JSON数据结构转换为JSON字符串

	file_write("set.txt", json_data, strlen(json_data));
	printf("%s\r\n",json_data);//输出字符串

	cJSON_free(json_data);
	cJSON_Delete(cjson_set);//清除结构体
}
