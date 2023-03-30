#include "particalDischarge_file.h"


uint16_t ch1_PD_data[PD_DATA_LEN];//通道1的原始数据 幅值+相位为一组 
uint16_t ch2_PD_data[PD_DATA_LEN];//通道2的原始数据 幅值+相位为一组 
uint16_t ch3_PD_data[PD_DATA_LEN];//通道3的原始数据 幅值+相位为一组 

prpd_t ch1_prpd_analysisBuf[PD_ANALYSIS_DATA_LEN];//通道1的计算后的存放BUF PRPD 
prpd_t ch2_prpd_analysisBuf[PD_ANALYSIS_DATA_LEN];//通道2的计算后的存放BUF PRPD
prpd_t ch3_prpd_analysisBuf[PD_ANALYSIS_DATA_LEN];//通道3的计算后的存放BUF PRPD

prps_t ch1_prps_analysisBuf[PD_ANALYSIS_DATA_LEN];//通道1的计算后的存放BUF PRPS
prps_t ch2_prps_analysisBuf[PD_ANALYSIS_DATA_LEN];//通道2的计算后的存放BUF PRPS
prps_t ch3_prps_analysisBuf[PD_ANALYSIS_DATA_LEN];//通道2的计算后的存放BUF PRPS

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
int analysis_pd_data_prpd(char *filename, uint16_t *srcdata, prpd_t *desdata, int len)
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

	file_write(filename, (char*)desdata, groupNum*2*2);
}


int analysis_pd_data_prps(char *filename, uint16_t *srcdata, prps_t *desdata, int len)
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

    file_write(filename, (char*)desdata, groupNum*2*3);
}


int prpd_analysis_buf_clear(prpd_t *desdata)
{
    int i;
    for(i=0;i<PD_ANALYSIS_DATA_LEN;i++)
    {
        desdata[i].amplitude = 0;
        desdata[i].phase = 0;
    }
    return 1;
}

int prps_analysis_buf_clear(prps_t *desdata)
{
    int i;
    for(i=0;i<PD_ANALYSIS_DATA_LEN;i++)
    {
        desdata[i].amplitude = 0;
        desdata[i].phase = 0;
        desdata[i].consult = 0;
    }
    return 1;
}

int abc = 1;
uint16_t testbufbuf [100] = {0xA600, 0xE101, 0xB002, 0xBA01, 0xB604, 0x9401, 0x9F06, 0x6D01, 0xAC08, 0x4701,
                             0xAA0A, 0x1F01, 0xA80B, 0xF901, 0xB00D, 0xD201, 0xA40F, 0xAC01, 0xB011, 0x8601,
                             0xB613, 0x5F01, 0xBE15, 0x3901, 0xB017, 0x1301, 0xB018, 0xED01, 0xAA1A, 0xC601,
                             0xAC1C, 0x9F01, 0xA81E, 0x7901, 0xAC20, 0x5201, 0xAC22, 0x2B01, 0xAE24, 0x0401,
                             0xAE25, 0xDE01, 0xAE27, 0xB701, 0xAE29, 0x9001, 0xB82B, 0x6A01, 0xB42D, 0x4301,
                             0xA82F, 0x1D01, 0xA830, 0xF601, 0xA032, 0xD001, 0xAA34, 0xAA01, 0xA036, 0x8301,
                             0xBA38, 0x5D01, 0xAC3A, 0x3701, 0xA03C, 0x1001, 0xA63D, 0xEA01, 0xB03F, 0xC301,
                             0x9B41, 0x9D01, 0x9B43, 0x7601, 0xA045, 0x4F01, 0xAE47, 0x2801, 0xA849, 0x0201,
                             0xAE4A, 0xDB01, 0xAA4C, 0xB501, 0xA04E, 0x8E01, 0xB050, 0x6801, 0xAA52, 0x4101,
                             0xAE54, 0x1B01, 0xAC55, 0xF401, 0x9B57, 0xCD01, 0xAE59, 0xA701, 0xA65B, 0x8101};

void PD_testPro(void)
{
    if(abc == 1)
    {
        abc = 0;
        prpd_analysis_buf_clear(ch1_prpd_analysisBuf);
        prps_analysis_buf_clear(ch1_prps_analysisBuf);
        analysis_pd_data_prpd(PDPR_CH1_FILE, testbufbuf, ch1_prpd_analysisBuf, 100);
        analysis_pd_data_prps(PDPS_CH1_FILE, testbufbuf, ch1_prps_analysisBuf, 100);
        printf("analysis done \n");
    }
}

