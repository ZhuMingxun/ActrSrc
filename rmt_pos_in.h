#ifndef _ACTR_INPUT_H
#define _ACTR_INPUT_H

#define     RESERVE_ONE_DECIMAL(varf)  ((float)((int)((varf+0.05f)*10.0f))/10.0f)

typedef struct
{
	u16 min;
	u16 max;

}VarRange_s;

typedef struct
{
	volatile u16 var;
	VarRange_s range;

	volatile float opening_pct;//0%-100%
	volatile float current_mA;//4-20mA
	
	u16   (*VarSample)();//采样函数
	void  (*OpeningSampleAndFilter)();//滤波函数
	float (*GetOpening_pct)();
	float (*GetCurrent_mA)();
	
}AnalogInput_s;



typedef struct
{
	u16 is_zero_mid;
	volatile u16 var;
	VarRange_s range;
	
	volatile float opening_pct;//0%-100%
	volatile float current_mA;//4-20mA
	
	u16   (*Sample)();//采样函数
	void  (*OpeningFilter)();//滤波函数
	float (*GetOpening_pct)();
	float (*GetCurrent_mA)();
	
}EncoderInput_s;












#endif




