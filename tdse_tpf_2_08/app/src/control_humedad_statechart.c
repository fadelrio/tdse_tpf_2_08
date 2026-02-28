/********************** inclusions *******************************************/

#include "main.h"
#include "logger.h"
#include "dwt.h"

#include "app.h"
#include "task_system_attribute.h"
#include "control_humedad_attribute.h"
#include "control_humedad_interface.h"
#include "task_sensor_digital_attribute.h"
#include "task_sensor_digital_interface.h"
#include "task_system_interface.h"
/********************** macros ***********************************************/

#define G_TASK_SYS_CNT_INI			0ul
#define G_TASK_SYS_TICK_CNT_INI		0ul

#define DEL_SYS_MIN					0ul
#define DEL_SYS_MED					50ul
#define DEL_SYS_MAX					500ul

#define CONTROL_HUMEDAD_ERROR_CNT_MAX 5
/********************** typedef **********************************************/

/********************** VARIABLES TEMPORARIAS **********************************************/

unsigned int timer_humedad = 1000;
float delta_hum = .5;
unsigned int timer_camb_hum = 1000;
//TODO reemplazarlos por lo que corresponda

/********************** VARIABLES TEMPORARIAS **********************************************/

void check_error_hum();
void check_error_sec();


control_humedad_dta_t control_humedad_dta = {DEL_SYS_MIN, NADA_HUM, IDLE_HUM, false, 0, 0, 0};

void init_control_humedad_statechart(){

	init_queue_event_control_humedad();

}

void update_control_humedad_statechart(const task_system_cfg_t p_task_system_cfg){

	control_humedad_dta_t *p_control_humedad_dta;

	/* Update Task System Data Pointer */
	p_control_humedad_dta = &control_humedad_dta;


	if (true == any_event_control_humedad())
	{
		p_control_humedad_dta->flag = true;
		p_control_humedad_dta->event = get_event_control_humedad();
	}

	switch (p_control_humedad_dta->state)//NADA_HUM, SENSE_HUM_READY
	{
		case IDLE_HUM:
			if (p_control_humedad_dta->tick == 0){
				p_control_humedad_dta->state = CHECK_HUM;
				put_event_task_sensor_digital(EV_START_MEASUREMENT_DIGITAL);
			}else{
				p_control_humedad_dta->tick--;
			}
			break;
		case CHECK_HUM:
			if (p_control_humedad_dta->flag == true && p_control_humedad_dta->event == SENSE_HUM_READY){
				p_control_humedad_dta->humedad_ant = p_control_humedad_dta->humedad;
				p_control_humedad_dta->humedad = get_humidity_measure();
				if (p_control_humedad_dta->humedad > p_task_system_cfg.h_0 + delta_hum){
					p_control_humedad_dta->state = SECAR;
					p_control_humedad_dta->tick = timer_camb_hum;
					//put_event_task_actuator_digital(SECAR_ON); TODO
				}else if (p_control_humedad_dta->humedad < p_task_system_cfg.h_0 - delta_hum){
					p_control_humedad_dta->state = HUMEDECER;
					p_control_humedad_dta->tick = timer_camb_hum;
					//put_event_task_actuator_digital(HUMEDECER_ON); TODO
				}else {
					p_control_humedad_dta->state = IDLE_HUM;
					p_control_humedad_dta->tick = timer_humedad;
				}
				p_control_humedad_dta->flag = false;
			}
			break;
		case SECAR:
			if (p_control_humedad_dta->tick == 0){
				p_control_humedad_dta->state = SENSE_SEC;
				put_event_task_sensor_digital(EV_START_MEASUREMENT_DIGITAL);
				//put_event_task_actuator_digital(SECAR_OFF); TODO
			}else{
				p_control_humedad_dta->tick--;
			}
			break;
		case SENSE_SEC:
			if (p_control_humedad_dta->flag == true && p_control_humedad_dta->event == SENSE_HUM_READY){
				p_control_humedad_dta->humedad_ant = p_control_humedad_dta->humedad;
				p_control_humedad_dta->humedad = get_humidity_measure();
			if (p_control_humedad_dta->humedad > p_task_system_cfg.h_0 + delta_hum){
				p_control_humedad_dta->state = SECAR;
				p_control_humedad_dta->tick = timer_camb_hum;
				//put_event_task_actuator_digital(SECAR_ON); TODO
#ifndef TEST_0
				check_error_sec();
#endif
			}else if (p_control_humedad_dta->humedad < p_task_system_cfg.h_0 - delta_hum){
				p_control_humedad_dta->state = CHECK_HUM;
				put_event_task_sensor_digital(EV_START_MEASUREMENT_DIGITAL);
#ifndef TEST_0
				check_error_sec();
#endif
			}
			p_control_humedad_dta->flag = false;
			}
			break;
		case HUMEDECER:
			if (p_control_humedad_dta->tick == 0){
				p_control_humedad_dta->state = SENSE_HUM;
				put_event_task_sensor_digital(EV_START_MEASUREMENT_DIGITAL);
				//put_event_task_actuator_digital(HUMEDECER_OFF); TODO
			}else{
				p_control_humedad_dta->tick--;
			}
			break;
		case SENSE_HUM:
			if (p_control_humedad_dta->flag == true && p_control_humedad_dta->event == SENSE_HUM_READY){
				p_control_humedad_dta->humedad_ant = p_control_humedad_dta->humedad;
				p_control_humedad_dta->humedad = get_humidity_measure();
			if (p_control_humedad_dta->humedad > p_task_system_cfg.h_0 + delta_hum){
				p_control_humedad_dta->state = CHECK_HUM;
				put_event_task_sensor_digital(EV_START_MEASUREMENT_DIGITAL);
#ifndef TEST_0
				check_error_hum();
#endif
			}else if (p_control_humedad_dta->humedad < p_task_system_cfg.h_0 - delta_hum){
				p_control_humedad_dta->state = HUMEDECER;
				p_control_humedad_dta->tick = timer_camb_hum;
				//put_event_task_actuator_digital(HUMEDECER_ON); TODO
#ifndef TEST_0
				check_error_hum();
#endif
				}
			p_control_humedad_dta->flag = false;
			}
			break;
	}

}

void check_error_hum(){
	//asumimos que el comportamiento esperado es creciente
	control_humedad_dta_t *p_control_humedad_dta = &control_humedad_dta;
	if ((p_control_humedad_dta->humedad - p_control_humedad_dta->humedad_ant) > 0 + delta_hum){
		p_control_humedad_dta->error_cnt = 0;
		return;
	}else{
		p_control_humedad_dta->error_cnt++;
		if (p_control_humedad_dta->error_cnt == CONTROL_HUMEDAD_ERROR_CNT_MAX){
			put_event_task_system(EV_SYS_ERROR);
		}
		return;
	}
}

void check_error_sec(){
	//asumimos que el comportamiento esperado es decreciente
	control_humedad_dta_t *p_control_humedad_dta = &control_humedad_dta;
	if ((p_control_humedad_dta->humedad - p_control_humedad_dta->humedad_ant) < 0 - delta_hum){
		p_control_humedad_dta->error_cnt = 0;
		return;
	}else{
		p_control_humedad_dta->error_cnt++;
		if (p_control_humedad_dta->error_cnt == CONTROL_HUMEDAD_ERROR_CNT_MAX){
			put_event_task_system(EV_SYS_ERROR);
		}
		return;
	}
}
