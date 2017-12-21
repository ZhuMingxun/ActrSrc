


#include "actr_struct.h"
#include "actr_pos_io_port.h"

ActrPos_s sActrPosInput;
ActrPos_s *p_sActrPosInpu = &sActrPosInput;


static u8 ActrPosSrcSelect();//电位器或者编码器检测
static float prvGetActrPos_pct();
static float prvGetActrCurrent_mA();

void ActrPosInput_Init()
{
	u8 pos_src = 0;
	
	p_sActrPosInput->range.low = ReadActrPos_LowVar();
	p_sActrPosInput->range.high = ReadActrPos_HighVar();
	
	pos_src = ActrPosSrcSelect();

	if(!pos_src)
	{
		p_sActrPosInput->VarSample 			= ActrPosVarSample_Pot;
		p_sActrPosInput->PosSampleAndFilter = prvActrPosSampleFilt_Pot;
		
	}
	else
	{
		p_sActrPosInput->is_zero_mid		= ReadActrPos_ZeroMid();
		p_sActrPosInput->VarSample 			= ActrPosVarSample_Ecd;
		p_sActrPosInput->PosSampleAndFilter = prvActrPosSampleFilt_Ecd;
	}
	
	p_sActrPosInput->GetPos_pct 		= prvGetActrPos_pct;
	p_sActrPosInput->GetCurrent_mA 		= prvGetActrCurrent_mA;	
	
}

static float prvGetActrPos_pct()
{
	return p_sActrPosInput->pos_pct;

}

static float prvGetActrCurrent_mA()
{
	float curr_mA;
	
	curr_mA = (16.0f/100.0f) * p_sActrPosInput->pos_pct + 4.0f;

	curr_mA = RESERVE_ONE_DECIMAL(curr_mA);
	
	return curr_mA;

}


































