/********************** inclusions *******************************************/

#include "main.h"
#include "logger.h"
#include "dwt.h"
#include "task_system_attribute.h"
#include "menu_attribute.h"
#include "menu_interface.h"
#include "display.h" //TODO implementar el display
/********************** macros ***********************************************/

#define G_TASK_SYS_CNT_INI			0ul
#define G_TASK_SYS_TICK_CNT_INI		0ul

#define DEL_SYS_MIN					0ul
#define DEL_SYS_MED					50ul
#define DEL_SYS_MAX					500ul
/********************** typedef **********************************************/

menu_dta_t menu_dta = {DEL_SYS_MIN, TEMPERATURA_SELECT, NEXT, false};

//TODO armar las variables de control

int TEMP_MAX;
int TEMP_MIN;

int LUZ_MAX;
int LUZ_MIN;

int HUMEDAD_MAX;
int HUMEDAD_MIN;

int RIEGO_MAX;
int RIEGO_MIN;



void init_menu_statechart(){

	init_queue_event_menu();

}

void update_menu_statechart(task_system_cfg_t *p_task_system_cfg){

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
				}else if(p_menu_dta->event == UP && p_task_system_cfg->t_0 < TEMP_MAX){
					p_task_system_cfg->t_0+=1;
				}else if(p_menu_dta->event == DOWN && p_task_system_cfg->t_0 > TEMP_MIN){
					p_task_system_cfg->t_0-=1;
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
				}else if(p_menu_dta->event == UP && p_task_system_cfg->l_0 < LUZ_MAX){
					p_task_system_cfg->l_0+=1;
				}else if(p_menu_dta->event == DOWN && p_task_system_cfg->l_0 > LUZ_MIN){
					p_task_system_cfg->l_0 -=1;
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
				}else if(p_menu_dta->event == UP && p_task_system_cfg->h_0 < HUMEDAD_MAX){
					p_task_system_cfg->h_0+=1;
				}else if(p_menu_dta->event == DOWN && p_task_system_cfg->h_0 > HUMEDAD_MIN){
					p_task_system_cfg->h_0-=1;
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
				}else if(p_menu_dta->event == UP && p_task_system_cfg->r_0 < RIEGO_MAX){
					p_task_system_cfg->r_0+=1;
				}else if(p_menu_dta->event == DOWN && p_task_system_cfg->r_0 > RIEGO_MIN){
					p_task_system_cfg->r_0-=1;
				}
				p_menu_dta->flag = false;
			}
			break;

	}

}
