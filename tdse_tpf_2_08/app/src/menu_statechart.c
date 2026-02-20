/********************** inclusions *******************************************/

#include "main.h"
#include "logger.h"
#include "dwt.h"

#include "menu_attribute.h"
#include "menu_interface.h"
/********************** macros ***********************************************/

#define G_TASK_SYS_CNT_INI			0ul
#define G_TASK_SYS_TICK_CNT_INI		0ul

#define DEL_SYS_MIN					0ul
#define DEL_SYS_MED					50ul
#define DEL_SYS_MAX					500ul
/********************** typedef **********************************************/

menu_dta_t menu_dta = {DEL_SYS_MIN, TEMPERATURA_SELECT, NEXT, false};

//TODO armar las variables de control
int T_0;
int TEMP_MAX;
int TEMP_MIN;
int L_0_MENU;
int LUZ_MAX;
int LUZ_MIN;
int H_0;
int HUMEDAD_MAX;
int HUMEDAD_MIN;
int R_0;
int RIEGO_MAX;
int RIEGO_MIN;



void init_menu_statechart(){

	init_queue_event_menu();

}

void update_menu_statechart(){

	menu_dta_t *p_menu_dta;

	/* Update Task System Data Pointer */
	p_menu_dta = &menu_dta;


	if (true == any_event_menu())
	{
		p_menu_dta->flag = true;
		p_menu_dta->event = get_event_menu();
	}

	switch (p_menu_dta->state)
	{
		case TEMPERATURA_SELECT:
			if(p_menu_dta->flag == true){
				if(p_menu_dta->event == NEXT){
					p_menu_dta->state = LUZ_SELECT;
				}else if(p_menu_dta->event == ENTER){
					p_menu_dta->state = TEMPERATURA_MENU;
				}
				p_menu_dta->flag = false;
			}
			break;
		case TEMPERATURA_MENU:
			if(p_menu_dta->flag == true){
				if(p_menu_dta->event == ESCAPE){
					p_menu_dta->state = TEMPERATURA_SELECT;
				}else if(p_menu_dta->event == UP && T_0 < TEMP_MAX){
					T_0+=1;
				}else if(p_menu_dta->event == DOWN && T_0 > TEMP_MIN){
					T_0-=1;
				}
				p_menu_dta->flag = false;
			}
			break;
		case LUZ_SELECT:
			if(p_menu_dta->flag == true){
				if(p_menu_dta->event == NEXT){
					p_menu_dta->state = HUMEDAD_SELECT;
				}else if(p_menu_dta->event == ENTER){
					p_menu_dta->state = LUZ_MENU;
				}
				p_menu_dta->flag = false;
			}
			break;
		case LUZ_MENU:
			if(p_menu_dta->flag == true){
				if(p_menu_dta->event == ESCAPE){
					p_menu_dta->state = LUZ_SELECT;
				}else if(p_menu_dta->event == UP && L_0_MENU < LUZ_MAX){
					L_0_MENU+=1;
				}else if(p_menu_dta->event == DOWN && L_0_MENU > LUZ_MIN){
					L_0_MENU -=1;
				}
				p_menu_dta->flag = false;
			}
			break;
		case HUMEDAD_SELECT:
			if(p_menu_dta->flag == true){
				if(p_menu_dta->event == NEXT){
					p_menu_dta->state = RIEGO_SELECT;
				}else if(p_menu_dta->event == ENTER){
					p_menu_dta->state = HUMEDAD_MENU;
				}
				p_menu_dta->flag = false;
			}
			break;
		case HUMEDAD_MENU:
			if(p_menu_dta->flag == true){
				if(p_menu_dta->event == ESCAPE){
					p_menu_dta->state = HUMEDAD_SELECT;
				}else if(p_menu_dta->event == UP && H_0 < HUMEDAD_MAX){
					H_0+=1;
				}else if(p_menu_dta->event == DOWN && H_0 > HUMEDAD_MIN){
					H_0-=1;
				}
				p_menu_dta->flag = false;
			}
			break;
		case RIEGO_SELECT:
			if(p_menu_dta->flag == true){
				if(p_menu_dta->event == NEXT){
					p_menu_dta->state = TEMPERATURA_SELECT;
				}else if(p_menu_dta->event == ENTER){
					p_menu_dta->state = RIEGO_MENU;
				}
				p_menu_dta->flag = false;
			}
			break;
		case RIEGO_MENU:
			if(p_menu_dta->flag == true){
				if(p_menu_dta->event == ESCAPE){
					p_menu_dta->state = RIEGO_SELECT;
				}else if(p_menu_dta->event == UP && R_0 < RIEGO_MAX){
					R_0+=1;
				}else if(p_menu_dta->event == DOWN && R_0 > RIEGO_MIN){
					R_0-=1;
				}
				p_menu_dta->flag = false;
			}
			break;

	}

}
