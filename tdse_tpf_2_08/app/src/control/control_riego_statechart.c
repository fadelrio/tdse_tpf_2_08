/********************** inclusions *******************************************/

#include "main.h"
#include "logger.h"
#include "dwt.h"

#include "system/task_system_attribute.h"
#include "control/control_riego_attribute.h"
#include "control/control_riego_interface.h"
#include "system/task_system_interface.h"
/********************** macros ***********************************************/

#define G_TASK_SYS_CNT_INI			0ul
#define G_TASK_SYS_TICK_CNT_INI		0ul

#define DEL_SYS_MIN					0ul
#define DEL_SYS_MED					50ul
#define DEL_SYS_MAX					500ul

#define DELTA_RIEGO					40
#define CONTROL_RIEGO_ERROR_CNT_MAX 5
/********************** typedef **********************************************/

void check_error_riego();

control_riego_dta_t control_riego_dta = {DEL_SYS_MIN, IDLE_RIEGO, false};

void init_control_riego_statechart(){

	init_queue_event_control_riego();

}

void update_control_riego_statechart(const task_system_cfg_t p_task_system_cfg){

	control_riego_dta_t *p_control_riego_dta;

	/* Update Task System Data Pointer */
	p_control_riego_dta = &control_riego_dta;


	if (any_event_control_riego())
	{
		p_control_riego_dta->flag = true;
		p_control_riego_dta->event = get_event_control_riego();
	}

	switch (p_control_riego_dta->state)
	{
		case IDLE_RIEGO:
			if(p_control_riego_dta->tick > 0) {
				p_control_riego_dta->tick--;
			} else {//timer == 0
				sense_riego(); //todo
				p_control_riego_dta->state = CHECK_RIEGO;
			}
			p_control_riego_dta->flag = false;
		break;

		case CHECK_RIEGO:
			if (p_control_riego_dta->flag && p_control_riego_dta->event == SENSE_RIEGO_READY) {
				p_control_riego_dta->riego_ant = p_control_riego_dta->riego;
				p_control_riego_dta->riego = 0; //TODO: leer_humedad()
				if(p_control_riego_dta->riego < p_task_system_cfg.r_0 - DELTA_RIEGO) {
					p_control_riego_dta->tick = timer_cambio_riego();
					regador_on(); // todo
					p_control_riego_dta->state = REGAR_RIEGO;
				} else { //Riego > R_0
					p_control_riego_dta->tick = timer_riego();
					p_control_riego_dta->state = IDLE_RIEGO;
				}
				p_control_riego_dta->flag = false;
			}
			break;

		case REGAR_RIEGO:
			if(p_control_riego_dta->tick > 0) { //timer > 0
				p_control_riego_dta->tick--;
			} else { //timer == 0
				sense_riego(); //todo
				regador_off(); //todo
				p_control_riego_dta->state = SENSE_RIEGO;
			}
			p_control_riego_dta->flag = false;
		break;

		case SENSE_RIEGO:
			if(p_control_riego_dta->flag && p_control_riego_dta->event == SENSE_RIEGO_READY) {
				p_control_riego_dta->riego_ant = p_control_riego_dta->riego;
				p_control_riego_dta->riego = 0; //TODO: leer_humedad()
				if(p_control_riego_dta->riego < p_task_system_cfg.r_0 - DELTA_RIEGO) {
					p_control_riego_dta->tick = timer_cambio_riego();
					regador_on(); //todo
					p_control_riego_dta->state = REGAR_RIEGO;
#ifndef TEST_0
					check_error_riego();
#endif
				} else {
					p_control_riego_dta->tick = timer_riego();
					p_control_riego_dta->state = IDLE_RIEGO;
#ifndef TEST_0
					check_error_riego();
#endif
				}
				p_control_riego_dta->flag = false;
			}
			break;
	}

}
void check_error_riego(){
	//asumimos que el comportamiento esperado es creciente
	control_riego_dta_t *p_control_riego_dta = &control_riego_dta;
	if ((p_control_riego_dta->riego - p_control_riego_dta->riego_ant) > 0 + DELTA_RIEGO){
		p_control_riego_dta->error_cnt = 0;
		return;
	}else{
		p_control_riego_dta->error_cnt++;
		if (p_control_riego_dta->error_cnt == CONTROL_RIEGO_ERROR_CNT_MAX){
			put_event_task_system(EV_SYS_ERROR);
		}
		return;
	}
}
