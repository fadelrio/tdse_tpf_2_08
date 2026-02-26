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
 * @file   : task_actuator.c
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
#include <task_actuator_analogico_attribute.h>
#include <task_actuator_analogico_interface.h>

/********************** macros and definitions *******************************/
#define G_TASK_ACT_CNT_INIT			0ul
#define G_TASK_ACT_TICK_CNT_INI		0ul

#define DEL_LED_XX_PUL				250ul
#define DEL_LED_XX_BLI				500ul
#define DEL_ACT_XX_MIN				0ul
#define PERIOD						0xFFFF

/********************** internal data declaration ****************************/


extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

const task_actuator_analogico_cfg_t task_actuator_analogico_cfg_list[] = {
	{ID_PWM_LED, &htim2, TIM_CHANNEL_1, PERIOD},
	{ID_PWM_HEATER, &htim3, TIM_CHANNEL_2, PERIOD}
};

#define ACTUATOR_CFG_QTY	(sizeof(task_actuator_analogico_cfg_list)/sizeof(task_actuator_analogico_cfg_t))

task_actuator_analogico_dta_t task_actuator_analogico_dta_list[] = {
	{DEL_ACT_XX_MIN, ST_ACT_ANALOGICO_OFF, EV_ACT_ANALOGICO_NADA, 0, false},
	{DEL_ACT_XX_MIN, ST_ACT_ANALOGICO_OFF, EV_ACT_ANALOGICO_NADA, 0, false}
};

#define ACTUATOR_DTA_QTY	(sizeof(task_actuator_analogico_dta_list)/sizeof(task_actuator_analogico_dta_t))

/********************** internal functions declaration ***********************/
void task_actuator_analogico_statechart(void);
void setPWM(TIM_HandleTypeDef *timer, uint32_t channel,
            uint16_t period, uint16_t pulse);

/********************** internal data definition *****************************/
const char *p_task_actuator_analogico 		= "Task Actuator (Actuator Statechart)";
const char *p_task_actuator_analogico_ 		= "Non-Blocking & Update By Time Code";

/********************** external data declaration ****************************/
uint32_t g_task_actuator_analogico_cnt;
volatile uint32_t g_task_actuator_analogico_tick_cnt;

/********************** external functions definition ************************/
void task_actuator_analogico_init(void *parameters)
{
	uint32_t index;
	task_actuator_analogico_dta_t *p_task_actuator_dta;
	task_actuator_analogico_st_t state;
	task_actuator_analogico_ev_t event;
	bool b_event;

	/* Print out: Task Initialized */
	LOGGER_INFO(" ");
	LOGGER_INFO("  %s is running - %s", GET_NAME(task_actuator_analogico_init), p_task_actuator_analogico);
	LOGGER_INFO("  %s is a %s", GET_NAME(task_actuator), p_task_actuator_analogico_);

	/* Init & Print out: Task execution counter */
	g_task_actuator_analogico_cnt = G_TASK_ACT_CNT_INIT;
	LOGGER_INFO("   %s = %lu", GET_NAME(g_task_actuator_analogico_cnt), g_task_actuator_analogico_cnt);

	for (index = 0; ACTUATOR_DTA_QTY > index; index++)
	{
		/* Update Task Actuator Configuration & Data Pointer */
		p_task_actuator_dta = &task_actuator_analogico_dta_list[index];

		/* Init & Print out: Index & Task execution FSM */
		state = ST_ACT_ANALOGICO_OFF;
		p_task_actuator_dta->state = state;

		event = EV_ACT_ANALOGICO_NADA;
		p_task_actuator_dta->event = event;

		b_event = false;
		p_task_actuator_dta->flag = b_event;

		LOGGER_INFO(" ");
		LOGGER_INFO("   %s = %lu   %s = %lu   %s = %lu   %s = %s",
					 GET_NAME(index), index,
					 GET_NAME(state), (uint32_t)state,
					 GET_NAME(event), (uint32_t)event,
					 GET_NAME(b_event), (b_event ? "true" : "false"));
	}
}

void task_actuator_analogico_update(void *parameters)
{
	bool b_time_update_required = false;

	/* Protect shared resource */
	__asm("CPSID i");	/* disable interrupts*/
    if (G_TASK_ACT_TICK_CNT_INI < g_task_actuator_analogico_tick_cnt)
    {
		/* Update Tick Counter */
    	g_task_actuator_analogico_tick_cnt--;
    	b_time_update_required = true;
    }
    __asm("CPSIE i");	/* enable interrupts */

    while (b_time_update_required)
    {
		/* Update Task Counter */
		g_task_actuator_analogico_cnt++;

		/* Run Task Statechart */
    	task_actuator_analogico_statechart();

    	/* Protect shared resource */
		__asm("CPSID i");	/* disable interrupts */
		if (G_TASK_ACT_TICK_CNT_INI < g_task_actuator_analogico_tick_cnt)
		{
			/* Update Tick Counter */
			g_task_actuator_analogico_tick_cnt--;
			b_time_update_required = true;
		}
		else
		{
			b_time_update_required = false;
		}
		__asm("CPSIE i");	/* enable interrupts */
    }
}

void task_actuator_analogico_statechart(void)
{
	uint32_t index;
	const task_actuator_analogico_cfg_t *p_task_actuator_cfg;
	task_actuator_analogico_dta_t *p_task_actuator_dta;

	for (index = 0; ACTUATOR_DTA_QTY > index; index++)
	{
		/* Update Task Actuator Configuration & Data Pointer */
		p_task_actuator_cfg = &task_actuator_analogico_cfg_list[index];
		p_task_actuator_dta = &task_actuator_analogico_dta_list[index];

		switch (p_task_actuator_dta->state)
		{
			case ST_ACT_ANALOGICO_OFF:
				if(p_task_actuator_dta->flag){
					if(p_task_actuator_dta->event == EV_ACT_ANALOGICO_PWM_ON){
						p_task_actuator_dta->state = ST_ACT_ANALOGICO_ON;
						setPWM(p_task_actuator_cfg->handler, p_task_actuator_cfg->channel, p_task_actuator_cfg->period, p_task_actuator_dta->pulse);
					}
					p_task_actuator_dta->flag = false;
				}

				break;
			case ST_ACT_ANALOGICO_ON:
				if(p_task_actuator_dta->flag){
					if(p_task_actuator_dta->event == EV_ACT_ANALOGICO_PWM_OFF){
						p_task_actuator_dta->state = ST_ACT_ANALOGICO_OFF;
						setPWM(p_task_actuator_cfg->handler, p_task_actuator_cfg->channel, p_task_actuator_cfg->period, p_task_actuator_dta->pulse);
					}
					p_task_actuator_dta->flag = false;
				}
				break;


			default:

				p_task_actuator_dta->tick  = DEL_ACT_XX_MIN;
				p_task_actuator_dta->state = ST_ACT_ANALOGICO_OFF;
				p_task_actuator_dta->event = EV_ACT_ANALOGICO_NADA;
				p_task_actuator_dta->flag = false;
				p_task_actuator_dta->pulse = 0;

				break;
		}
	}
}

void setPWM(TIM_HandleTypeDef *timer, uint32_t channel,
            uint16_t period, uint16_t pulse) {
  HAL_TIM_PWM_Stop(timer, channel);
  TIM_OC_InitTypeDef sConfigOC;
  timer->Init.Period = period;
  HAL_TIM_PWM_Init(timer);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = pulse;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_PWM_ConfigChannel(timer, &sConfigOC, channel);
  HAL_TIM_PWM_Start(timer,channel);
}



/********************** end of file ******************************************/
