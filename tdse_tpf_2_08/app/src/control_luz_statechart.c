/********************** inclusions *******************************************/

#include "main.h"
#include "logger.h"
#include "dwt.h"

#include "control_luz_attribute.h"
#include "control_luz_interface.h"
/********************** macros ***********************************************/

#define G_TASK_SYS_CNT_INI			0ul
#define G_TASK_SYS_TICK_CNT_INI		0ul

#define DEL_SYS_MIN					0ul
#define DEL_SYS_MED					50ul
#define DEL_SYS_MAX					500ul
/********************** typedef **********************************************/

control_luz_dta_t control_luz_dta = {DEL_SYS_MIN, IDLE_LUZ, NADA_LUZ, false};

int delta_luz;
int luz;
int L_0;
int estado_led;

void init_control_luz_statechart(){

	init_queue_event_control_luz();

}

void update_control_luz_statechart(){

	control_luz_dta_t *p_control_luz_dta;

	/* Update Task System Data Pointer */
	p_control_luz_dta = &control_luz_dta;


	if (true == any_event_control_luz())
	{
		p_control_luz_dta->flag = true;
		p_control_luz_dta->event = get_event_control_luz();
	}
	//TODO CHEQUEAR STATECHART!!!!!!

	switch (p_control_luz_dta->state)
	{
		case IDLE_LUZ:
			if(p_control_luz_dta->flag == true){
				if(p_control_luz_dta->tick > 0){ //el tick es un evento? por lo q pusimos si pero no es algo q pasa
					p_control_luz_dta->tick--;
					p_control_luz_dta->flag = false;
				}else if(p_control_luz_dta->tick == 0){
					p_control_luz_dta->state = CHECK_LUZ;
					p_control_luz_dta->flag = false;
					//sensar_luz() aca va la funcion esta pero hay que armarla todavia
				}
			}//completar con los casos que haga falta
		case CHECK_LUZ:
			if(p_control_luz_dta->flag == true && p_control_luz_dta->event == SENSAR_LUZ_READY){
				if(luz > (L_0 + delta_luz) && estado_led > 0){
					p_control_luz_dta->state = BAJAR_LUZ;
					p_control_luz_dta->flag = false;
					estado_led = delta_luz;
				}else if(luz < (L_0 - delta_luz) && estado_led < 100){
					p_control_luz_dta->state = ILUMINAR;
					p_control_luz_dta->flag = false;
				}else{
					p_control_luz_dta->state = IDLE_LUZ;
					p_control_luz_dta->flag = false;
				}
			}
		case ILUMINAR:
				if(luz < (L_0 - delta_luz) && estado_led < 100){
					p_control_luz_dta->state = SUBIR_LUZ;
					//p_control_luz_dta->flag = false;
					//check_luz_error() funcion que hay que armar
				}else if(estado_led == 100){
					p_control_luz_dta->state = IDLE_LUZ;
					//p_control_luz_dta->flag = false;
					//check_luz_error() funcin que hay que usar
				}else if(luz > L_0 - delta_luz){
					p_control_luz_dta->state = CHECK_LUZ;
					//p_control_luz_dta->flag = false;
					//sensar_luz() funcion que hay que hacer
				}
		case BAJAR_LUZ:
				if(estado_led > 0){
					p_control_luz_dta->state = CHECK_LUZ;
					//p_control_luz_dta->flag = false;
					estado_led-= delta_luz;
				}else if(estado_led == 0){
					p_control_luz_dta->state = IDLE_LUZ;
					//p_control_luz_dta->flag = false;
				}

		case SUBIR_LUZ:
				p_control_luz_dta->state = ILUMINAR;
				//p_control_luz_dta->flag = false;
				estado_led += delta_luz;
			}

}
