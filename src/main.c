#include "cJSON.h"
#include "CFG_Init.h"

int main()
{
    get_module_config(module_config_buf);
    get_set_para(&module_set_para);

    //test 写函数
    // module_config_buf[0].data_type = 1;
    // module_config_buf[0].device_type = 1234;
    // set_module_config(module_config_buf);

    // memcpy(module_set_para.ip_addr, "192.168.70.1", sizeof("192.168.70.1"));
    // set_set_param(&module_set_para);

	return 0;
}
