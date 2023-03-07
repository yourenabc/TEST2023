#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "cJSON.h"

char json_string_cfg_read[8192];

char json_string_configbuf[]="{\"module_config\":[{\"channel\":1,\"module_type\":1,\"data_type\":2,\"coe\":1.1,\"enable_flag\":1,\"slave_addr\":0,\"storage_type\":1,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":2,\"module_type\":1,\"data_type\":2,\"coe\":1.2,\"enable_flag\":1,\"slave_addr\":0,\"storage_type\":5,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":3,\"module_type\":1,\"data_type\":2,\"coe\":1.2,\"enable_flag\":1,\"slave_addr\":0,\"storage_type\":9,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":4,\"module_type\":1,\"data_type\":2,\"coe\":1.4,\"enable_flag\":0,\"slave_addr\":0,\"storage_type\":1,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":5,\"module_type\":1,\"data_type\":2,\"coe\":1.5,\"enable_flag\":0,\"slave_addr\":0,\"storage_type\":5,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":6,\"module_type\":1,\"data_type\":2,\"coe\":1.6,\"enable_flag\":0,\"slave_addr\":0,\"storage_type\":9,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":7,\"module_type\":1,\"data_type\":2,\"coe\":1.7,\"enable_flag\":1,\"slave_addr\":0,\"storage_type\":1,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":8,\"module_type\":1,\"data_type\":2,\"coe\":1.8,\"enable_flag\":1,\"slave_addr\":0,\"storage_type\":5,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":9,\"module_type\":1,\"data_type\":2,\"coe\":1.9,\"enable_flag\":0,\"slave_addr\":0,\"storage_type\":9,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":10,\"module_type\":1,\"data_type\":2,\"coe\":1.1,\"enable_flag\":0,\"slave_addr\":0,\"storage_type\":4,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":11,\"module_type\":1,\"data_type\":2,\"coe\":1.2,\"enable_flag\":0,\"slave_addr\":0,\"storage_type\":4,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":12,\"module_type\":1,\"data_type\":2,\"coe\":1.3,\"enable_flag\":1,\"slave_addr\":0,\"storage_type\":4,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":13,\"module_type\":1,\"data_type\":2,\"coe\":1.4,\"enable_flag\":0,\"slave_addr\":0,\"storage_type\":4,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":14,\"module_type\":1,\"data_type\":2,\"coe\":1.5,\"enable_flag\":0,\"slave_addr\":0,\"storage_type\":4,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":15,\"module_type\":1,\"data_type\":2,\"coe\":1.6,\"enable_flag\":0,\"slave_addr\":0,\"storage_type\":4,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":16,\"module_type\":1,\"data_type\":2,\"coe\":1.7,\"enable_flag\":0,\"slave_addr\":0,\"storage_type\":4,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":17,\"module_type\":1,\"data_type\":2,\"coe\":1.8,\"enable_flag\":1,\"slave_addr\":0,\"storage_type\":4,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":18,\"module_type\":1,\"data_type\":2,\"coe\":1.9,\"enable_flag\":1,\"slave_addr\":0,\"storage_type\":4,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":19,\"module_type\":1,\"data_type\":2,\"coe\":1.9,\"enable_flag\":1,\"slave_addr\":0,\"storage_type\":4,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":20,\"module_type\":1,\"data_type\":2,\"coe\":1.9,\"enable_flag\":1,\"slave_addr\":0,\"storage_type\":4,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":21,\"module_type\":1,\"data_type\":2,\"coe\":1.9,\"enable_flag\":1,\"slave_addr\":0,\"storage_type\":4,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":22,\"module_type\":1,\"data_type\":2,\"coe\":1.9,\"enable_flag\":1,\"slave_addr\":0,\"storage_type\":4,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":23,\"module_type\":1,\"data_type\":2,\"coe\":1.9,\"enable_flag\":1,\"slave_addr\":0,\"storage_type\":4,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":24,\"module_type\":1,\"data_type\":2,\"coe\":1.9,\"enable_flag\":1,\"slave_addr\":0,\"storage_type\":4,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":25,\"module_type\":1,\"data_type\":2,\"coe\":1.9,\"enable_flag\":1,\"slave_addr\":0,\"storage_type\":4,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":26,\"module_type\":1,\"data_type\":2,\"coe\":1.9,\"enable_flag\":1,\"slave_addr\":0,\"storage_type\":4,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":27,\"module_type\":1,\"data_type\":2,\"coe\":1.9,\"enable_flag\":1,\"slave_addr\":0,\"storage_type\":4,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":28,\"module_type\":1,\"data_type\":2,\"coe\":1.9,\"enable_flag\":1,\"slave_addr\":0,\"storage_type\":4,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":29,\"module_type\":1,\"data_type\":2,\"coe\":1.9,\"enable_flag\":1,\"slave_addr\":0,\"storage_type\":4,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":30,\"module_type\":1,\"data_type\":2,\"coe\":1.9,\"enable_flag\":1,\"slave_addr\":0,\"storage_type\":4,\"data_addr\":2021,\"device_sn\":\"1234567890\",\"device_type\":1234}, \
                                                  {\"channel\":31,\"module_type\":1,\"data_type\":2,\"coe\":2.0,\"enable_flag\":0,\"slave_addr\":0,\"storage_type\":8,\"data_addr\":2023,\"device_sn\":\"1234567890\",\"device_type\":1234}]}";



int main()
{
 	int fd1 = 0;
	fd1 = open("config.txt", O_RDWR|O_CREAT, 0664);
	if(fd1==-1)
	{
		printf("open file fail\n\r");
	}
	else
	{
		printf("open file succeed\n\r");
	}
	// write(fd1, json_string_configbuf, strlen(json_string_configbuf));

	memset(json_string_cfg_read, 0x00, 8192);
	read(fd1, json_string_cfg_read, sizeof(json_string_cfg_read));
	cJSON* cjson_config = cJSON_Parse(json_string_cfg_read);
	if(cjson_config == NULL)
	{
		printf("cjson_config error...\r\n");
	}
	else
	{
		printf("%s\n",cJSON_Print(cjson_config));
	}
	cJSON_Delete(cjson_config);//清除结构体
	close(fd1);

	return 0;
}
