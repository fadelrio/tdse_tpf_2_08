/********************** inclusions *******************************************/

#include "main.h"
#include "logger.h"
#include "dwt.h"

#include "control_riego_attribute.h"
#include "control_riego_interface.h"
/********************** macros ***********************************************/

#define G_TASK_SYS_CNT_INI			0ul
#define G_TASK_SYS_TICK_CNT_INI		0ul

#define DEL_SYS_MIN					0ul
#define DEL_SYS_MED					50ul
#define DEL_SYS_MAX					500ul
/********************** typedef **********************************************/

control_riego_dta_t control_riego_dta = {DEL_SYS_MIN, IDLE_RIEGO, false};

void init_control_riego_statechart(){

	init_queue_event_control_riego();

}

void update_control_riego_statechart(){

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
				sense_riego();
				p_control_riego_dta->state = CHECK_RIEGO;
			}
			p_control_riego_dta->flag = false;
		break;

		case CHECK_RIEGO:
			if (p_control_riego_dta->flag && p_control_riego_dta->event == SENSE_RIEGO_READY) {
				if(leer_humedad() < R_0) {//Riego < R_0
					p_control_riego_dta->tick = timer_cambio_riego();
					regador_on();
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
				sense_riego();
				regador_off();
				p_control_riego_dta->state = SENSE_RIEGO;
			}
			p_control_riego_dta->flag = false;
		break;

		case SENSE_RIEGO:
			if(p_control_riego_dta->flag && p_control_riego_dta->event == SENSE_RIEGO_READY) {
				check_error_rieg();
				if(leer_humedad() < R_0) {
					p_control_riego_dta->tick = timer_cambio_riego();
					regador_on();
					p_control_riego_dta->state = REGAR_RIEGO;
				} else {
					p_control_riego_dta->tick = timer_riego();
					p_control_riego_dta->state = IDLE_RIEGO;
				}
				p_control_riego_dta->flag = false;
			}
			break;
	}

}
