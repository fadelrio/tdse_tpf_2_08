/********************** inclusions *******************************************/

#include "main.h"
#include "logger.h"
#include "dwt.h"
#include "system/task_system_attribute.h"
#include "menu/menu_attribute.h"
#include "menu/menu_interface.h"
#include "display/task_display_interface.h"
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
	print_string_task_display("      MENU      ", 0);
	print_string_task_display("   TEMPERATURA  ", 1);
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
					print_string_task_display("      MENU      ", 0);
					print_string_task_display("       LUZ      ", 1);
					p_menu_dta->state = LUZ_SELECT;
				}else if(p_menu_dta->event == ENTER){
					p_menu_dta->state = TEMPERATURA_MENU;
					print_string_task_display("   TEMPERATURA  ", 0);
					displayWriteValue(p_task_system_cfg->t_0);
				}
				p_menu_dta->flag = false;
			}
			break;
		case TEMPERATURA_MENU:
			if(p_menu_dta->flag == true){
				if(p_menu_dta->event == ESCAPE){
					print_string_task_display("      MENU      ", 0);
					print_string_task_display("   TEMPERATURA  ", 1);
					p_menu_dta->state = TEMPERATURA_SELECT;
				}else if(p_menu_dta->event == UP && p_task_system_cfg->t_0 < TEMP_MAX){
					p_task_system_cfg->t_0+=DELTA_TEMP;
					print_string_task_display("   TEMPERATURA  ", 0);
					displayWriteValue(p_task_system_cfg->t_0);
				}else if(p_menu_dta->event == DOWN && p_task_system_cfg->t_0 > TEMP_MIN){
					p_task_system_cfg->t_0-=DELTA_TEMP;
					print_string_task_display("   TEMPERATURA  ", 0);
					displayWriteValue(p_task_system_cfg->t_0);
				}
				p_menu_dta->flag = false;
			}
			break;
		case LUZ_SELECT:
			if(p_menu_dta->flag == true){
				if(p_menu_dta->event == NEXT){
					print_string_task_display("      MENU      ", 0);
					print_string_task_display("     HUMEDAD    ", 1);
					p_menu_dta->state = HUMEDAD_SELECT;
				}else if(p_menu_dta->event == ENTER){
					p_menu_dta->state = LUZ_MENU;
					print_string_task_display("      LUZ      ", 0);
					displayWriteValue(int12bits_a_porcentaje(p_task_system_cfg->l_0));
				}
				p_menu_dta->flag = false;
			}
			break;
		case LUZ_MENU:
			if(p_menu_dta->flag == true){
				if(p_menu_dta->event == ESCAPE){
					print_string_task_display("      MENU      ", 0);
					print_string_task_display("      LUZ      ", 1);
					p_menu_dta->state = LUZ_SELECT;
				}else if(p_menu_dta->event == UP && p_task_system_cfg->l_0 < LUZ_MAX){
					p_task_system_cfg->l_0+=DELTA_LUZ;
					print_string_task_display("      LUZ      ", 0);
					displayWriteValue(int12bits_a_porcentaje(p_task_system_cfg->l_0));
				}else if(p_menu_dta->event == DOWN && p_task_system_cfg->l_0 > LUZ_MIN){
					p_task_system_cfg->l_0 -=DELTA_LUZ;
					print_string_task_display("      LUZ      ", 0);
					displayWriteValue(int12bits_a_porcentaje(p_task_system_cfg->l_0));
				}
				p_menu_dta->flag = false;
			}
			break;
		case HUMEDAD_SELECT:
			if(p_menu_dta->flag == true){
				if(p_menu_dta->event == NEXT){
					print_string_task_display("      MENU      ", 0);
					print_string_task_display("      RIEGO     ", 1);
					p_menu_dta->state = RIEGO_SELECT;
				}else if(p_menu_dta->event == ENTER){
					p_menu_dta->state = HUMEDAD_MENU;
					print_string_task_display("     HUMEDAD    ", 0);
					displayWriteValue(p_task_system_cfg->h_0);
				}
				p_menu_dta->flag = false;
			}
			break;
		case HUMEDAD_MENU:
			if(p_menu_dta->flag == true){
				if(p_menu_dta->event == ESCAPE){
					print_string_task_display("      MENU      ", 0);
					print_string_task_display("     HUMEDAD    ", 1);
					p_menu_dta->state = HUMEDAD_SELECT;
				}else if(p_menu_dta->event == UP && p_task_system_cfg->h_0 < HUMEDAD_MAX){
					p_task_system_cfg->h_0+=DELTA_HUM;
					print_string_task_display("     HUMEDAD    ", 0);
					displayWriteValue(p_task_system_cfg->h_0);
				}else if(p_menu_dta->event == DOWN && p_task_system_cfg->h_0 > HUMEDAD_MIN){
					p_task_system_cfg->h_0-=DELTA_HUM;
					print_string_task_display("     HUMEDAD    ", 0);
					displayWriteValue(p_task_system_cfg->h_0);
				}
				p_menu_dta->flag = false;
			}
			break;
		case RIEGO_SELECT:
			if(p_menu_dta->flag == true){
				if(p_menu_dta->event == NEXT){
					print_string_task_display("      MENU      ", 0);
					print_string_task_display("   TEMPERATURA  ", 1);
					p_menu_dta->state = TEMPERATURA_SELECT;
				}else if(p_menu_dta->event == ENTER){
					p_menu_dta->state = RIEGO_MENU;
					print_string_task_display("      RIEGO     ", 0);
					displayWriteValue(int12bits_a_porcentaje(p_task_system_cfg->r_0));
				}
				p_menu_dta->flag = false;
			}
			break;
		case RIEGO_MENU:
			if(p_menu_dta->flag == true){
				if(p_menu_dta->event == ESCAPE){
					print_string_task_display("      MENU      ", 0);
					print_string_task_display("      RIEGO     ", 1);
					p_menu_dta->state = RIEGO_SELECT;
				}else if(p_menu_dta->event == UP && p_task_system_cfg->r_0 < RIEGO_MAX){
					p_task_system_cfg->r_0+=DELTA_RIEGO;
					print_string_task_display("      RIEGO     ", 0);
					displayWriteValue(int12bits_a_porcentaje(p_task_system_cfg->r_0));
				}else if(p_menu_dta->event == DOWN && p_task_system_cfg->r_0 > RIEGO_MIN){
					p_task_system_cfg->r_0-=DELTA_RIEGO;
					print_string_task_display("      RIEGO     ", 0);
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
	if(digitos == 1){
		snprintf(dato, sizeof(dato), "        %d       ", n);
		print_string_task_display(dato, 1);
	}else if(digitos == 2){
		snprintf(dato, sizeof(dato), "       %d       ", n);
		print_string_task_display(dato, 1);
	}else if(digitos == 3){
		snprintf(dato, sizeof(dato), "      %d       ", n);
		print_string_task_display(dato, 1);
	}
}

int int12bits_a_porcentaje(uint32_t i){
	return i*100/4096;
}
