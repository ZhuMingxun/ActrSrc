#ifndef _ACTR_STRUCT_H
#define _ACTR_STRUCT_H

#define     RESERVE_ONE_DECIMAL(varf)  ((float)((int)((varf+0.05f)*10.0f))/10.0f)

typedef struct
{
	u8 rang_from;
	u8 rang_to;
	u8 item_count;				//选项总数
	u8 selected;				//当前选择的菜单项
	char *menu_item[17];		//菜单项目名数组
	struct Menu_s **sub_menus;	//子菜单
	struct Menu_s *parent_menu;	//上级菜单
	void (**func)();			//相应选项的执行函数

}Menu_s;

typedef struct
{
	u16 low;
	u16 high;

}VarRange_s;

typedef struct
{
	u16 is_zero_mid;//编码器是否过零
	volatile u16 var;
	VarRange_s range;

	volatile float pos_pct;//0%-100%
	volatile float current_mA;//4-20mA
	
	u16   (*VarSample)();//采样函数
	void  (*PosSampleAndFilter)();//滤波函数
	float (*GetPos_pct)();
	float (*GetCurrent_mA)();
	
}ActrPos_s;



typedef struct
{
	volatile u16 var;
	VarRange_s range;

	u16 (*CalcOutVar)(float opening_pct);
	
}CurrentOutput_s;

#endif



