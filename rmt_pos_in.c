
#include "actr_struct.h"
#include "rmt_pos_in_port.h"



ActrPos_s sRmtPosInput;
ActrPos_s *p_sRmtPosInput = &sRmtPosInput;

static void prvCalcPos(u16 var,u16 minvar,u16 maxvar,float *pos);
static void prvRmtPosFilter();
static void prvGetRmtPos();
static void prvGetRmtCurrent();

//远方输入位置初始化
void RmtPosInput_Init()
{
	/* 读取Flash中的输入电流范围值 */
	p_sRmtPosInput->range.low = ReadRmtPos_LowVar();
	p_sRmtPosInput->range.high = ReadRmtPos_HighVar();

	/* 采样滤波函数 */
	p_sRmtPosInput->VarSample = SampleRmtPosVar;
	p_sRmtPosInput->PosSampleAndFilter = prvRmtPosSampleFilt;//需 50Hz/100Hz 执行
	
	p_sRmtPosInput->GetPos_pct = prvGetRmtPos_pct;
	p_sRmtPosInput->GetCurrent_mA = prvGetRmtCurrent_mA;
}

/* 模拟量计算开度函数 */
static void prvCalcPos(u16 var,u16 minvar,u16 maxvar,float *pos)
{
	float pos_ori;
	
	/* 计算输入开度 0.0%-100.0%  保留1位小数 */
	pos_ori = 100.0 * ((float)(var - minvar)) / ((fabs)((float)maxvar - (float)minvar));
	pos_ori = RESERVE_ONE_DECIMAL(pos_ori);
	
	*pos = pos_ori;
	
}

/* 输入开度采样函数 */
static float prvCalcRmtPos(u16 var)
{
	u16 adcval;
	u16 lowval;
	u16 highval;
	float pos_ori;

	lowval = p_sRmtPosInput->range.min;
	highval = p_sRmtPosInput->range.max;
	
	/* 读取ADC采样值 */
	adcval = var;
	
	/* 计算开度 */
	prvCalcPos(adcval,lowval,highval,&pos_ori);
	
	/* 开度限幅 */
    pos_ori = pos_ori >= 0.0? pos_ori:0.0;
    pos_ori = pos_ori <= 100.0? pos_ori:100.0;
	
	return pos_ori;

}

/* 远方输入开度滤波函数 */
static float prvRmtPosFilter(float pos)
{
	float pos_ori;
    float pos_filter;
	
	float pos_array[6];
	float max,min;
	float sum=0;
	
	static u8 pos_array_cnt;
	u8 i;

	pos_ori = pos;
	pos_filter = pos;
	
	if(pos_array_cnt>5)
	{
		pos_array_cnt = 0;
	}

	pos_array[pos_array_cnt++] = pos_ori;
	
	max = pos_array[0];
	min = pos_array[0];
	
	/* 寻找最大最小值 */
	for(i=0;i<6;i++)
	{
		if(pos_array[i]>max)
		{
			max = pos_array[i];
		}
		
		if(pos_array[i]<min)
		{
			min = pos_array[i];
		}
		
		sum += pos_array[i];
		
	}
	
	pos_filter = (float)(sum-max-min)/4.0f;
	
	return pos_filter;
}

static float prvRmtPosSampleFilt()
{
	
	float pos_samp;
	float pos_filt;
	
	/* 采样 */
	p_sRmtPosInput->var = p_sRmtPosInput->VarSample();
	
	/* 计算输入位置 */
	pos_samp = prvCalcRmtPos(p_sRmtPosInput->var);
	
	/* 输入位置滤波 */
	pos_filt = prvRmtPosFilter(pos_samp);
	
	p_sRmtPosInput->pos_pct = pos_filt;
	
}

static float prvGetRmtOpening_pct()
{
	return p_sRmtPosInput->pos_pct;
}

static float prvGetRmtCurrent_mA()
{
	float curr_mA;
/* 	u16 adcval,lowvar,highvar;
	
	lowvar 	= p_sRmtCurrInput->range.min;
	highvar = p_sRmtCurrInput->range.max;
	adcval 	= p_sRmtCurrInput->var;
	curr_mA = (16.0f/fabs(highvar-lowvar)) * (float)(adcval-lowvar) + 4.0f; */
	
	curr_mA = (16.0f/100.0f) * p_sRmtPosInput->pos_pct + 4.0f;

	curr_mA = RESERVE_ONE_DECIMAL(curr_mA);
	
	return curr_mA;

}


















