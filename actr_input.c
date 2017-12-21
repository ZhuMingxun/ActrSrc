


#include "actr_struct.h"
#include "actr_input_port.h"

AnalogInput_s sRmtCurrInput;
AnalogInput_s *p_sRmtCurrInput = &sRmtCurrInput;

static void prvCalcOpening(u16 var,u16 minvar,u16 maxvar,float *opening);
static void prvRmtOpeningFilter();
static void prvGetRmtOpening();
static void prvGetRmtCurrent();

//远方输入电流初始化
void RmtCurrInput_Init()
{
	/* 读取Flash中的输入电流范围值 */
	p_sRmtCurrInput->range.min = ReadRmtCurrMinVar();
	p_sRmtCurrInput->range.max = ReadRmtCurrMaxVar();

	/* 采样滤波函数 */
	p_sRmtCurrInput->Sample = RmtCurrVarSample;
	p_sRmtCurrInput->OpeningSampleAndFilter = prvRmtOpeningSampleFilt;//需 50Hz/100Hz 执行
	
	p_sRmtCurrInput->GetOpening_pct = prvGetRmtOpening_pct;
	p_sRmtCurrInput->GetCurrent_mA = prvGetRmtCurrent_mA;
}

/* 模拟量计算开度函数 */
static void prvCalcOpening(u16 var,u16 minvar,u16 maxvar,float *opening)
{
	float opening_ori;
	
	/* 计算输入开度 0.0%-100.0%  保留1位小数 */
	opening_ori = 100.0 * ((float)(var - minvar)) / ((fabs)((float)maxvar - (float)minvar));
	opening_ori = RESERVE_ONE_DECIMAL(opening_ori);
	
	*opening = opening_ori;
	
}

/* 输入开度采样函数 */
static float prvRmtOpeningSample()
{
	u16 adcval;
	u16 lowval;
	u16 highval;
	float opening_ori;

	lowval = p_sRmtCurrInput->range.min;
	highval = p_sRmtCurrInput->range.max;
	
	/* 读取ADC采样值 */
	p_sRmtCurrInput->var = p_sRmtCurrInput->Sample();
	adcval = p_sRmtCurrInput->var;
	
	/* 计算开度 */
	prvCalcOpening(adcval,lowval,highval,&opening_ori);
	
	/* 开度限幅 */
    opening_ori = opening_ori >= 0.0? opening_ori:0.0;
    opening_ori = opening_ori <= 100.0? opening_ori:100.0;
	
	return opening_ori;

}

/* 远方输入开度滤波函数 */
static float prvRmtOpeningFilter(float opening)
{
	float opening_ori;
    float opening_filter;
	
	float opening_array[6];
	float max,min;
	float sum=0;
	
	static u8 opening_array_cnt;
	u8 i;

	
	opening_ori = opening;
	opening_filter = opening;
	
	if(opening_array_cnt>5)
	{
		opening_array_cnt = 0;
	}

	opening_array[opening_array_cnt++] = opening_ori;
	
	max = opening_array[0];
	min = opening_array[0];
	/* 寻找最大最小值 */
	for(i=0;i<6;i++)
	{
		if(opening_array[i]>max)
		{
			max = opening_array[i];
		}
		
		if(opening_array[i]<min)
		{
			min = opening_array[i];
		}
		
		sum += opening_array[i];
		
	}
	
	opening_filter = (float)(sum-max-min)/4.0f;
	
	return opening_filter;
}

static float prvRmtOpeningSampleFilt()
{
	float opening_samp;
	float opening_filt;
	
	opening_samp = prvRmtOpeningSample();
	opening_filt = prvRmtOpeningFilter(opening_samp);
	
	p_sRmtCurrInput->opening_pct = opening_filt;
	
}

static float prvGetRmtOpening_pct()
{
	return p_sRmtCurrInput->opening_pct;
}

static float prvGetRmtCurrent_mA()
{
	float curr_mA;
/* 	u16 adcval,lowvar,highvar;
	
	lowvar 	= p_sRmtCurrInput->range.min;
	highvar = p_sRmtCurrInput->range.max;
	adcval 	= p_sRmtCurrInput->var;
	curr_mA = (16.0f/fabs(highvar-lowvar)) * (float)(adcval-lowvar) + 4.0f; */
	
	curr_mA = (16.0f/100.0f) * p_sRmtCurrInput->opening_pct + 4.0f;

	curr_mA = RESERVE_ONE_DECIMAL(curr_mA);
	
	return curr_mA;

}


















