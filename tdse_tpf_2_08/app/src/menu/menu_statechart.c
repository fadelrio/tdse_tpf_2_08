/********************** inclusions *******************************************/

#include "main.h"
#include "logger.h"
#include "dwt.h"
#include "system/task_system_attribute.h"
#include "menu/menu_attribute.h"
#include "menu/menu_interface.h"
#include "display.h"
/********************** macros ***********************************************/

#define G_TASK_SYS_CNT_INI			0ul
#define G_TASK_SYS_TICK_CNT_INI		0ul

#define DEL_SYS_MIN					0ul
#define DEL_SYS_MED					50ul
#define DEL_SYS_MAX					500ul

#define RIEGO_MAX					4095
#define RIEGO_MIN					0
#define DELTA_RIEGO					41

#define LUZ_MAX						4095
#define LUZ_MIN						0
#define DELTA_LUZ					41

#define TEMP_MAX					40
#define TEMP_MIN					25
#define DELTA_TEMP					1

#define HUMEDAD_MAX					100
#define HUMEDAD_MIN					30
#define DELTA_HUM					5
/********************** typedef **********************************************/

menu_dta_t menu_dta = {DEL_SYS_MIN, TEMPERATURA_SELECT, NEXT, false};


int countDigits(int n);
void displayWriteValue(int n);
int int12bits_a_porcentaje(uint32_t i);


void init_menu_statechart(){

	init_queue_event_menu();

}

void set_up_menu_display(){
	displayCharPositionWrite(0,0);
	displayStringWrite("      MENU      ");
	displayCharPositionWrite(0,1);
	displayStringWrite("   TEMPERATURA  ");
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
					displayCharPositionWrite(0,0);
					displayStringWrite("      MENU      ");
					displayCharPositionWrite(0,1);
					displayStringWrite("       LUZ      ");
					p_menu_dta->state = LUZ_SELECT;
				}else if(p_menu_dta->event == ENTER){
					p_menu_dta->state = TEMPERATURA_MENU;
					displayCharPositionWrite(0,0);
					displayStringWrite("   TEMPERATURA  ");
					displayWriteValue(p_task_system_cfg->t_0);
				}
				p_menu_dta->flag = false;
			}
			break;
		case TEMPERATURA_MENU:
			if(p_menu_dta->flag == true){
				if(p_menu_dta->event == ESCAPE){
					displayCharPositionWrite(0,0);
					displayStringWrite("      MENU      ");
					displayCharPositionWrite(0,1);
					displayStringWrite("   TEMPERATURA  ");
					p_menu_dta->state = TEMPERATURA_SELECT;
				}else if(p_menu_dta->event == UP && p_task_system_cfg->t_0 < TEMP_MAX){
					p_task_system_cfg->t_0+=DELTA_TEMP;
					displayCharPositionWrite(0,0);
					displayStringWrite("   TEMPERATURA  ");
					displayWriteValue(p_task_system_cfg->t_0);
				}else if(p_menu_dta->event == DOWN && p_task_system_cfg->t_0 > TEMP_MIN){
					p_task_system_cfg->t_0-=DELTA_TEMP;
					displayCharPositionWrite(0,0);
					displayStringWrite("   TEMPERATURA  ");
					displayWriteValue(p_task_system_cfg->t_0);
				}
				p_menu_dta->flag = false;
			}
			break;
		case LUZ_SELECT:
			if(p_menu_dta->flag == true){
				if(p_menu_dta->event == NEXT){
					displayCharPositionWrite(0,0);
					displayStringWrite("      MENU      ");
					displayCharPositionWrite(0,1);
					displayStringWrite("     HUMEDAD    ");
					p_menu_dta->state = HUMEDAD_SELECT;
				}else if(p_menu_dta->event == ENTER){
					p_menu_dta->state = LUZ_MENU;
					displayCharPositionWrite(0,0);
					displayStringWrite("      LUZ      ");
					displayWriteValue(int12bits_a_porcentaje(p_task_system_cfg->l_0));
				}
				p_menu_dta->flag = false;
			}
			break;
		case LUZ_MENU:
			if(p_menu_dta->flag == true){
				if(p_menu_dta->event == ESCAPE){
					displayCharPositionWrite(0,0);
					displayStringWrite("      MENU      ");
					displayCharPositionWrite(0,1);
					displayStringWrite("      LUZ      ");
					p_menu_dta->state = LUZ_SELECT;
				}else if(p_menu_dta->event == UP && p_task_system_cfg->l_0 < LUZ_MAX){
					p_task_system_cfg->l_0+=DELTA_LUZ;
					displayCharPositionWrite(0,0);
					displayStringWrite("      LUZ      ");
					displayWriteValue(int12bits_a_porcentaje(p_task_system_cfg->l_0));
				}else if(p_menu_dta->event == DOWN && p_task_system_cfg->l_0 > LUZ_MIN){
					p_task_system_cfg->l_0 -=DELTA_LUZ;
					displayCharPositionWrite(0,0);
					displayStringWrite("      LUZ      ");
					displayWriteValue(int12bits_a_porcentaje(p_task_system_cfg->l_0));
				}
				p_menu_dta->flag = false;
			}
			break;
		case HUMEDAD_SELECT:
			if(p_menu_dta->flag == true){
				if(p_menu_dta->event == NEXT){
					displayCharPositionWrite(0,0);
					displayStringWrite("      MENU      ");
					displayCharPositionWrite(0,1);
					displayStringWrite("      RIEGO     ");
					p_menu_dta->state = RIEGO_SELECT;
				}else if(p_menu_dta->event == ENTER){
					p_menu_dta->state = HUMEDAD_MENU;
					displayCharPositionWrite(0,0);
					displayStringWrite("     HUMEDAD    ");
					displayWriteValue(p_task_system_cfg->h_0);
				}
				p_menu_dta->flag = false;
			}
			break;
		case HUMEDAD_MENU:
			if(p_menu_dta->flag == true){
				if(p_menu_dta->event == ESCAPE){
					displayCharPositionWrite(0,0);
					displayStringWrite("      MENU      ");
					displayCharPositionWrite(0,1);
					displayStringWrite("     HUMEDAD    ");
					p_menu_dta->state = HUMEDAD_SELECT;
				}else if(p_menu_dta->event == UP && p_task_system_cfg->h_0 < HUMEDAD_MAX){
					p_task_system_cfg->h_0+=DELTA_HUM;
					displayCharPositionWrite(0,0);
					displayStringWrite("     HUMEDAD    ");
					displayWriteValue(p_task_system_cfg->h_0);
				}else if(p_menu_dta->event == DOWN && p_task_system_cfg->h_0 > HUMEDAD_MIN){
					p_task_system_cfg->h_0-=DELTA_HUM;
					displayCharPositionWrite(0,0);
					displayStringWrite("     HUMEDAD    ");
					displayWriteValue(p_task_system_cfg->h_0);
				}
				p_menu_dta->flag = false;
			}
			break;
		case RIEGO_SELECT:
			if(p_menu_dta->flag == true){
				if(p_menu_dta->event == NEXT){
					displayCharPositionWrite(0,0);
					displayStringWrite("      MENU      ");
					displayCharPositionWrite(0,1);
					displayStringWrite("   TEMPERATURA  ");
					p_menu_dta->state = TEMPERATURA_SELECT;
				}else if(p_menu_dta->event == ENTER){
					p_menu_dta->state = RIEGO_MENU;
					displayCharPositionWrite(0,0);
					displayStringWrite("      RIEGO     ");
					displayWriteValue(int12bits_a_porcentaje(p_task_system_cfg->r_0));
				}
				p_menu_dta->flag = false;
			}
			break;
		case RIEGO_MENU:
			if(p_menu_dta->flag == true){
				if(p_menu_dta->event == ESCAPE){
					displayCharPositionWrite(0,0);
					displayStringWrite("      MENU      ");
					displayCharPositionWrite(0,1);
					displayStringWrite("      RIEGO     ");
					p_menu_dta->state = RIEGO_SELECT;
				}else if(p_menu_dta->event == UP && p_task_system_cfg->r_0 < RIEGO_MAX){
					p_task_system_cfg->r_0+=DELTA_RIEGO;
					displayCharPositionWrite(0,0);
					displayStringWrite("      RIEGO     ");
					displayWriteValue(int12bits_a_porcentaje(p_task_system_cfg->r_0));
				}else if(p_menu_dta->event == DOWN && p_task_system_cfg->r_0 > RIEGO_MIN){
					p_task_system_cfg->r_0-=DELTA_RIEGO;
					displayCharPositionWrite(0,0);
					displayStringWrite("      RIEGO     ");
					displayWriteValue(int12bits_a_porcentaje(p_task_system_cfg->r_0));
				}
				p_menu_dta->flag = false;
			}
			break;

	}

}

int countDigits(int n) {
    if (n == 0) {
        return 1; // Special case for zero
    }
    // For negative numbers, use the absolute value
    if (n < 0) {
        n = -n;
    }
    int count = 0;
    while (n != 0) {
        n /= 10;   // Remove the last digit
        count++;   // Increment the count
    }
    return count;
}

void displayWriteValue(int n){
	int digitos = countDigits(n);
	char dato[17]; //pongo lugar para el cero
	displayCharPositionWrite(0,1);
	if(digitos == 1){
		snprintf(dato, sizeof(dato), "        %d       ", n);
		displayStringWrite(dato);
	}else if(digitos == 2){
		snprintf(dato, sizeof(dato), "       %d       ", n);
		displayStringWrite(dato);
	}else if(digitos == 3){
		snprintf(dato, sizeof(dato), "      %d       ", n);
	    displayStringWrite(dato);
	}
}

int int12bits_a_porcentaje(uint32_t i){
	return i*100/4096;
}
