#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "cJSON.h"
#include "CFG_Init.h"

int main()
{
    get_module_config(module_config_buf);
    get_set_para(&module_set_para);
	return 0;
}
