

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
	volatile u16 var;
	VarRange_s range;

	u16 (*CalcOutVar)(float opening_pct);
	
}CurrentOutput_s;







