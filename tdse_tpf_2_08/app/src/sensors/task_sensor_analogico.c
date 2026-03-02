/*
 * Copyright (c) 2023 Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * @file   : task_sensor.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

/********************** inclusions *******************************************/
/* Project includes */
#include "main.h"

/* Demo includes */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes */
#include "board.h"
#include "app.h"
#include "system/task_system_attribute.h"
#include "system/task_system_interface.h"
#include "sensors/task_sensor_analogico_attribute.h"
#include "sensors/task_sensor_analogico_interface.h"
#include "control/control_riego_interface.h"
#include "control/control_luz_interface.h"

/********************** macros and definitions *******************************/
#define G_TASK_SEN_CNT_INIT			0ul
#define G_TASK_SEN_TICK_CNT_INI		0ul

#define DEL_SEN_XX_MIN				0ul
#define DEL_SEN_XX_MED				25ul
#define DEL_SEN_XX_MAX				50ul

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

/********************** internal data declaration ****************************/
const task_sensor_analogico_cfg_t task_sensor_analogico_cfg_list[] = {
		{ID_SENSOR_LUZ, DEL_SEN_XX_MAX, &hadc1, SENSE_RIEGO_READY, SENSAR_LUZ_READY},
		{ID_SENSOR_RIEGO, DEL_SEN_XX_MAX, &hadc2, SENSE_RIEGO_READY, SENSAR_LUZ_READY}
};

#define SENSOR_CFG_QTY	(sizeof(task_sensor_analogico_cfg_list)/sizeof(task_sensor_analogico_cfg_t))

task_sensor_analogico_dta_t task_sensor_analogico_dta_list[] = {
		{DEL_SEN_XX_MIN, ST_SENSOR_ANALOGICO_IDLE, EV_SENSOR_ANALOGICO_NADA, false, 0},
		{DEL_SEN_XX_MIN, ST_SENSOR_ANALOGICO_IDLE, EV_SENSOR_ANALOGICO_NADA, false, 0}
};

#define SENSOR_DTA_QTY	(sizeof(task_sensor_analogico_dta_list)/sizeof(task_sensor_analogico_dta_t))

/********************** internal functions declaration ***********************/
void task_sensor_analogico_statechart(void);

/********************** internal data definition *****************************/
const char *p_task_sensor_analogico 		= "Task Sensor analogico (Sensor analogico Statechart)";
const char *p_task_sensor_analogico_ 		= "Non-Blocking & Update By Time Code";

/********************** external data declaration ****************************/
uint32_t g_task_sensor_analogico_cnt;
volatile uint32_t g_task_sensor_analogico_tick_cnt;

/********************** external functions definition ************************/
void task_sensor_analogico_init(void *parameters)
{
	uint32_t index;
	task_sensor_analogico_dta_t *p_task_sensor_dta;
	task_sensor_analogico_st_t state;
	task_sensor_analogico_ev_t event;

	/* Print out: Task Initialized */
	LOGGER_INFO(" ");
	LOGGER_INFO("  %s is running - %s", GET_NAME(task_sensor_init), p_task_sensor_analogico);
	LOGGER_INFO("  %s is a %s", GET_NAME(task_sensor), p_task_sensor_analogico_);

	/* Init & Print out: Task execution counter */
	g_task_sensor_analogico_cnt = G_TASK_SEN_CNT_INIT;
	LOGGER_INFO("   %s = %lu", GET_NAME(g_task_sensor_analogico_cnt), g_task_sensor_analogico_cnt);

	for (index = 0; SENSOR_DTA_QTY > index; index++)
	{
		/* Update Task Sensor Data Pointer */
		p_task_sensor_dta = &task_sensor_analogico_dta_list[index];

		/* Init & Print out: Index & Task execution FSM */
		state = ST_SENSOR_ANALOGICO_IDLE;
		p_task_sensor_dta->state = state;

		event = EV_SENSOR_ANALOGICO_NADA;
		p_task_sensor_dta->event = event;

		LOGGER_INFO(" ");
		LOGGER_INFO("   %s = %lu   %s = %lu   %s = %lu",
				    GET_NAME(index), index,
					GET_NAME(state), (uint32_t)state,
					GET_NAME(event), (uint32_t)event);
	}
}

void task_sensor_analogico_update(void *parameters)
{
	bool b_time_update_required = false;

	/* Protect shared resource */
	__asm("CPSID i");	/* disable interrupts */
    if (G_TASK_SEN_TICK_CNT_INI < g_task_sensor_analogico_tick_cnt)
    {
		/* Update Tick Counter */
    	g_task_sensor_analogico_tick_cnt--;
    	b_time_update_required = true;
    }
    __asm("CPSIE i");	/* enable interrupts */

    while (b_time_update_required)
    {
		/* Update Task Counter */
		g_task_sensor_analogico_cnt++;

		/* Run Task Statechart */
    	task_sensor_analogico_statechart();

    	/* Protect shared resource */
		__asm("CPSID i");	/* disable interrupts */
		if (G_TASK_SEN_TICK_CNT_INI < g_task_sensor_analogico_tick_cnt)
		{
			/* Update Tick Counter */
			g_task_sensor_analogico_tick_cnt--;
			b_time_update_required = true;
		}
		else
		{
			b_time_update_required = false;
		}
		__asm("CPSIE i");	/* enable interrupts */
    }
}

void task_sensor_analogico_statechart(void)
{
	uint32_t index;
	const task_sensor_analogico_cfg_t *p_task_sensor_cfg; //TODO en algun momento hay que usarlo
	task_sensor_analogico_dta_t *p_task_sensor_dta;

	for (index = 0; SENSOR_DTA_QTY > index; index++)
	{
		/* Update Task Sensor Configuration & Data Pointer */
		p_task_sensor_cfg = &task_sensor_analogico_cfg_list[index];
		p_task_sensor_dta = &task_sensor_analogico_dta_list[index];

		switch (p_task_sensor_dta->state)
		{
			case ST_SENSOR_ANALOGICO_IDLE:
				if(p_task_sensor_dta->flag){
					if (p_task_sensor_dta->event == EV_SENSOR_ANALOGICO_START_MEASUREMENT){
						if(HAL_ADC_Start_IT(p_task_sensor_cfg->handler) != HAL_OK){
							Error_Handler();
						}
						p_task_sensor_dta->state = ST_SENSOR_ANALOGICO_MEASURING;
					}
					p_task_sensor_dta->flag = false;
				}

				break;

			case ST_SENSOR_ANALOGICO_MEASURING:
				if(p_task_sensor_dta->flag){
					if(p_task_sensor_dta->event == EV_SENSOR_ANALOGICO_MEASURE_READY){
						p_task_sensor_dta->measure = HAL_ADC_GetValue(p_task_sensor_cfg->handler);
						if(p_task_sensor_cfg->identifier == ID_SENSOR_LUZ){
							put_event_control_luz(p_task_sensor_cfg->measure_ready_luz);
						}else if(p_task_sensor_cfg->identifier == ID_SENSOR_RIEGO){
							put_event_control_riego(p_task_sensor_cfg->measure_ready_riego);
						}
						p_task_sensor_dta->state = ST_SENSOR_ANALOGICO_IDLE;
					}
					p_task_sensor_dta->flag = false;
				}
				break;

			default:

				p_task_sensor_dta->tick  = DEL_SEN_XX_MIN;
				p_task_sensor_dta->state = ST_SENSOR_ANALOGICO_IDLE;
				p_task_sensor_dta->event = EV_SENSOR_ANALOGICO_NADA;

				break;
		}
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	task_sensor_analogico_dta_t *p_task_sensor_dta;
	if(hadc->Instance == ADC1){
		p_task_sensor_dta = &task_sensor_analogico_dta_list[ID_SENSOR_LUZ];
	}else if(hadc->Instance == ADC1){
		p_task_sensor_dta = &task_sensor_analogico_dta_list[ID_SENSOR_RIEGO];
	}
	p_task_sensor_dta->flag = true;
	p_task_sensor_dta->event = EV_SENSOR_ANALOGICO_MEASURE_READY;
}

/********************** end of file ******************************************/
