/********************** inclusions *******************************************/

#include "main.h"
#include "logger.h"
#include "dwt.h"

#include "task_system_attribute.h"
#include "control_temperatura_attribute.h"
#include "control_temperatura_interface.h"
#include "task_sensor_digital_attribute.h"
#include "task_sensor_digital_interface.h"
/********************** macros ***********************************************/

#define G_TASK_SYS_CNT_INI			0ul
#define G_TASK_SYS_TICK_CNT_INI		0ul

#define DEL_SYS_MIN					0ul
#define DEL_SYS_MED					50ul
#define DEL_SYS_MAX					500ul
#define delta                       0ul
#define TIMER_CAMB_TEMP             10ul
#define TIMER_TEMP                  5ul
/********************** typedef **********************************************/

control_temperatura_dta_t control_temperatura_dta = {DEL_SYS_MIN, NADA_TEMP, IDLE_TEMP, 0, false};

void init_control_temperatura_statechart(){

	init_queue_event_control_temperatura();

}

void update_control_temperatura_statechart(const task_system_cfg_t p_task_system_cfg){

	control_temperatura_dta_t *p_control_temperatura_dta;

	/* Update Task System Data Pointer */
	p_control_temperatura_dta = &control_temperatura_dta;


	if (true == any_event_control_temperatura())
	{
		p_control_temperatura_dta->flag = true;
		p_control_temperatura_dta->event = get_event_control_temperatura();
	}

	switch (p_control_temperatura_dta->state)
	{
	case NADA_TEMP:{
		if (p_control_temperatura_dta->tick == 0u){
					p_control_temperatura_dta->state = CHECK_TEMP;
					put_event_task_sensor_digital(EV_START_MEASUREMENT_DIGITAL);
				}
		if (p_control_temperatura_dta->flag && (p_control_temperatura_dta->event == TICK)){
			if (p_control_temperatura_dta->tick > 0u){
						p_control_temperatura_dta->tick--;
					}
					p_control_temperatura_dta->state = NADA_TEMP;
				}
		p_control_temperatura_dta->flag = false;
		break;
	}
	case CHECK_TEMP:{
		if (p_control_temperatura_dta->flag && (p_control_temperatura_dta->event == SENSE_TEMP_READY)){
			p_control_temperatura_dta->temperatura = get_temperature_measure();
			if (p_control_temperatura_dta->temperatura > p_task_system_cfg.t_0 + delta){
				p_control_temperatura_dta->state = ENFRIAR;
				p_control_temperatura_dta->tick = TIMER_CAMB_TEMP;
			}else if(p_control_temperatura_dta->temperatura < p_task_system_cfg.t_0 - delta){
				p_control_temperatura_dta->state = CALENTAR;
				p_control_temperatura_dta->tick =TIMER_CAMB_TEMP;
			}else{
				p_control_temperatura_dta->state = NADA_TEMP;
				p_control_temperatura_dta->tick = TIMER_TEMP;
			}
		}
		p_control_temperatura_dta->flag = false;
		break;
	}
	case ENFRIAR:{
		if (p_control_temperatura_dta->flag ){
				if(p_control_temperatura_dta->event == ENTRY){
					p_control_temperatura_dta->state = ENFRIAR;
					//enfriador_on();TODO

				}else if (p_control_temperatura_dta->event == TICK){
					if (p_control_temperatura_dta->tick == 0u){
						p_control_temperatura_dta->state = SENSE_FRIO;
						put_event_task_sensor_digital(EV_START_MEASUREMENT_DIGITAL);
						//enfriador_off();TODO
					}else if (p_control_temperatura_dta->tick>0){
						p_control_temperatura_dta->state = ENFRIAR;
						p_control_temperatura_dta->tick--;
					}

				}
		}
		p_control_temperatura_dta->flag = false;
		break;
	}
	case SENSE_FRIO:{
		if(p_control_temperatura_dta->flag && (p_control_temperatura_dta->event == SENSE_TEMP_READY)){
			p_control_temperatura_dta->temperatura = get_temperature_measure();
			if (p_control_temperatura_dta->temperatura < p_task_system_cfg.t_0 +delta){
				p_control_temperatura_dta->state = CHECK_TEMP;
				put_event_task_sensor_digital(EV_START_MEASUREMENT_DIGITAL);
				//check_error_temp();TODO
			}else if(p_control_temperatura_dta->temperatura > p_task_system_cfg.t_0 + delta){
				p_control_temperatura_dta->state = ENFRIAR;
				p_control_temperatura_dta->tick = TIMER_CAMB_TEMP;
				//check_error_temp();TODO
			}
		}
		p_control_temperatura_dta->flag= false;
		break;
	}
	case CALENTAR:{
		if (p_control_temperatura_dta->flag){
			if( p_control_temperatura_dta->event == ENTRY){
					p_control_temperatura_dta->state = CALENTAR;
					//calentador_on();TODO
					p_control_temperatura_dta->flag= false;
			}else if (p_control_temperatura_dta->event == TICK){
				if(p_control_temperatura_dta->tick == 0u){
					p_control_temperatura_dta->state = SENSE_CALOR;
					put_event_task_sensor_digital(EV_START_MEASUREMENT_DIGITAL);
					//calentador_off();TODO
				}else if (p_control_temperatura_dta->tick > 0u){
					p_control_temperatura_dta->state = CALENTAR;
					p_control_temperatura_dta->tick--;

					}
				}
			}
		p_control_temperatura_dta->flag= false;
		break;
		}
	case SENSE_CALOR:
	{
		if(p_control_temperatura_dta->flag){
			if(p_control_temperatura_dta->event == SENSE_TEMP_READY){
				p_control_temperatura_dta->temperatura = get_temperature_measure();
				if(p_control_temperatura_dta->temperatura > p_task_system_cfg.t_0 - delta){
					p_control_temperatura_dta->state = CHECK_TEMP;
					put_event_task_sensor_digital(EV_START_MEASUREMENT_DIGITAL);
					//check_error_temp();TODO
				}else if(p_control_temperatura_dta->temperatura < p_task_system_cfg.t_0 - delta){
					p_control_temperatura_dta->state = CALENTAR;
					p_control_temperatura_dta->tick = TIMER_CAMB_TEMP;
					//check_error_temp();TODO
				}
			}
		}
		p_control_temperatura_dta->flag= false;
		break;
	}
	default:{
		p_control_temperatura_dta->state = NADA_TEMP; // TODO: ver qué caso poner acá igual esto no es seguro
		break;
	}
	}
}
