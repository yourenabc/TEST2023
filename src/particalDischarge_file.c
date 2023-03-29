#include "particalDischarge_file.h"


uint16_t channel_1_PD[PD_DATA_LEN];
uint16_t channel_2_PD[PD_DATA_LEN];
uint16_t channel_3_PD[PD_DATA_LEN];

prpd_t channel_1_prpdBuf[100];
prpd_t channel_2_prpdBuf[100];
prpd_t channel_3_prpdBuf[100];

prps_t channel_1_prpsBuf[100];

int read_pd_file_data(modbus_t *ctx, uint16_t fileNum, uint16_t startRecordNum, uint16_t len, uint16_t *desBuf)
{
    if(len <= 0x7b)
    {
        modbus_read_file_record(ctx, fileNum, startRecordNum, len, desBuf);
        return 1;
    }
    else
    {
        int i = 0;
        int times = 0;
        int left = 0;
        times = len / 0x7b;
        left  = len % 0x7b;

        if(left != 0)
        {
            times +=1; 
        }

        for(i = 0; i < times; i++)
        {
            if((i < times) && (i != times - 1))
            {
                modbus_read_file_record(ctx, fileNum, startRecordNum*i + 0x7b, 0x7b, &desBuf[0x7b*i]);
            }
            else if(i == (times-1))
            {
                modbus_read_file_record(ctx, fileNum, startRecordNum*i + 0x7b, left, &desBuf[0x7b*i]);
            }
        }
        return 1;
    }
    return 0;
}

//需要记住每次读取文件时的每个通道的长度 
int analysis_pd_data_prpd(uint16_t *srcdata, prpd_t *desdata, int len)
{
    int i, groupNum;
    uint16_t amplitude, phase;
    float remainder;

    groupNum = len/2;

    for(i=0; i<groupNum; i++)
    {
        amplitude = srcdata[(2*i)];
        phase     = srcdata[(2*i) + 1];

        remainder = phase % PhaseStacking;

        desdata[i].amplitude = amplitude; 
        desdata[i].phase = (remainder / PhaseStacking) * 360;
    }

	// file_write("../acu/pdpr.txt", (char*)desdata, 200);
}


int analysis_pd_data_prps(uint16_t *srcdata, prps_t *desdata, int len)
{
    int i, groupNum;
    uint16_t amplitude, phase, consult;
    float remainder;

    groupNum = len/2;

    for(i=0; i<groupNum; i++)
    {
        amplitude = srcdata[(2*i)];
        phase     = srcdata[(2*i) + 1];

        consult   = phase / PhaseStacking;
        remainder = phase % PhaseStacking;

        desdata[i].amplitude = amplitude; 
        desdata[i].phase = (remainder / PhaseStacking) * 360;
        desdata[i].consult = consult;
    }

    // file_write("../acu/pdps.txt", (char*)desdata, 300);
}






