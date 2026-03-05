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
#include "memory/task_memory_attribute.h"
#include "memory/task_memory_interface.h"
#include "system/task_system_interface.h"

/********************** macros and definitions *******************************/
#define G_TASK_SEN_CNT_INIT			0ul
#define G_TASK_SEN_TICK_CNT_INI		0ul

#define DEL_I2C_XX_MIN				0ul
#define DEL_I2C_XX_MED				25ul
#define DEL_I2C_XX_MAX				50ul

#define M24C256_ADDRESS 0x50

#define TIMER_MEM_BUSY 10

/********************** external data declaration ****************************/
extern I2C_HandleTypeDef hi2c1;

/********************** internal data declaration ****************************/
const task_memory_cfg_t task_memory_cfg_list[] = {
		{ID_24C256, M24C256_ADDRESS << 1u, &hi2c1, DEL_I2C_XX_MAX,
				EV_SYS_MEM_READ_READY}
};

#define MEMORY_CFG_QTY	(sizeof(task_memory_cfg_list)/sizeof(task_memory_cfg_t))

task_memory_dta_t task_memory_dta_list[] = {
		{DEL_I2C_XX_MIN, ST_MEMORY_IDLE, EV_MEMORY_READ, false}
};

#define MEMORY_DTA_QTY	(sizeof(task_memory_dta_list)/sizeof(task_memory_dta_t))

/********************** internal functions declaration ***********************/
void task_memory_statechart(void);

/********************** internal data definition *****************************/
const char *p_task_memory 		= "Task memory (memory Statechart)";
const char *p_task_memory_ 		= "Non-Blocking & Update By Time Code";

/********************** external data declaration ****************************/
uint32_t g_task_memory_cnt;
volatile uint32_t g_task_memory_tick_cnt;

/********************** external functions definition ************************/
void task_memory_init(void *parameters)
{
	uint32_t index;
	task_memory_dta_t *p_task_memory_dta;
	task_memory_st_t state;
	task_memory_ev_t event;

	/* Print out: Task Initialized */
	LOGGER_INFO(" ");
	LOGGER_INFO("  %s is running - %s", GET_NAME(task_memory_init), p_task_memory);
	LOGGER_INFO("  %s is a %s", GET_NAME(task_memory), p_task_memory_);

	/* Init & Print out: Task execution counter */
	g_task_memory_cnt = G_TASK_SEN_CNT_INIT;
	LOGGER_INFO("   %s = %lu", GET_NAME(g_task_memory_cnt), g_task_memory_cnt);

	for (index = 0; MEMORY_DTA_QTY > index; index++)
	{
		/* Update Task memory Data Pointer */
		p_task_memory_dta = &task_memory_dta_list[index];

		/* Init & Print out: Index & Task execution FSM */
		state = ST_MEMORY_IDLE;
		p_task_memory_dta->state = state;

		event = EV_MEMORY_READ;
		p_task_memory_dta->event = event;

		LOGGER_INFO(" ");
		LOGGER_INFO("   %s = %lu   %s = %lu   %s = %lu",
				    GET_NAME(index), index,
					GET_NAME(state), (uint32_t)state,
					GET_NAME(event), (uint32_t)event);
	}
}

void task_memory_update(void *parameters)
{
	bool b_time_update_required = false;

	/* Protect shared resource */
	__asm("CPSID i");	/* disable interrupts */
    if (G_TASK_SEN_TICK_CNT_INI < g_task_memory_tick_cnt)
    {
		/* Update Tick Counter */
    	g_task_memory_tick_cnt--;
    	b_time_update_required = true;
    }
    __asm("CPSIE i");	/* enable interrupts */

    while (b_time_update_required)
    {
		/* Update Task Counter */
		g_task_memory_cnt++;

		/* Run Task Statechart */
    	task_memory_statechart();

    	/* Protect shared resource */
		__asm("CPSID i");	/* disable interrupts */
		if (G_TASK_SEN_TICK_CNT_INI < g_task_memory_tick_cnt)
		{
			/* Update Tick Counter */
			g_task_memory_tick_cnt--;
			b_time_update_required = true;
		}
		else
		{
			b_time_update_required = false;
		}
		__asm("CPSIE i");	/* enable interrupts */
    }
}

void task_memory_statechart(void)
{
	uint32_t index;
	const task_memory_cfg_t *p_task_memory_cfg;
	task_memory_dta_t *p_task_memory_dta;

	for (index = 0; MEMORY_DTA_QTY > index; index++)
	{
		/* Update Task memory Configuration & Data Pointer */
		p_task_memory_cfg = &task_memory_cfg_list[index];
		p_task_memory_dta = &task_memory_dta_list[index];


		switch (p_task_memory_dta->state)
		{
			case ST_MEMORY_IDLE:
				if(p_task_memory_dta->flag){
					if(p_task_memory_dta->event == EV_MEMORY_WRITE){
						p_task_memory_dta->state = ST_MEMORY_WRITING;
						if(HAL_I2C_Mem_Write_IT(p_task_memory_cfg->i2c_handler, p_task_memory_cfg->address, p_task_memory_dta->write_data.mem_adress, sizeof(p_task_memory_dta->write_data.mem_adress), (uint8_t*)p_task_memory_dta->write_data.data,p_task_memory_dta->write_data.size) != HAL_OK){
							Error_Handler();
						}
					}else if(p_task_memory_dta->event == EV_MEMORY_READ){
						p_task_memory_dta->state = ST_MEMORY_READING;
						if(HAL_I2C_Mem_Read_IT(p_task_memory_cfg->i2c_handler, p_task_memory_cfg->address, p_task_memory_dta->read_data.mem_adress, sizeof(p_task_memory_dta->read_data.mem_adress), (uint8_t*)p_task_memory_dta->read_data.data,p_task_memory_dta->read_data.size) != HAL_OK){
							Error_Handler();
						}
					}
					p_task_memory_dta->flag = false;
				}

			case ST_MEMORY_BUSY:
				if(p_task_memory_dta->tick == 0){
					p_task_memory_dta->state = ST_MEMORY_IDLE;
				}else {
					p_task_memory_dta->tick--;
				}

			case ST_MEMORY_READING:
				if(HAL_I2C_GetState(p_task_memory_cfg->i2c_handler) == HAL_I2C_STATE_READY){
					if(HAL_I2C_GetError(p_task_memory_cfg->i2c_handler) != HAL_I2C_ERROR_AF){
						p_task_memory_dta->state = ST_MEMORY_IDLE;
						put_event_task_system(p_task_memory_cfg->mem_read_ready);
					}else{
						p_task_memory_dta->state = ST_MEMORY_IDLE;
						p_task_memory_dta->flag = true;
						p_task_memory_dta->event = EV_MEMORY_READ;
					}
				}

			case ST_MEMORY_WRITING:
				if(HAL_I2C_GetState(p_task_memory_cfg->i2c_handler) == HAL_I2C_STATE_READY){
					if(HAL_I2C_GetError(p_task_memory_cfg->i2c_handler) != HAL_I2C_ERROR_AF){
						p_task_memory_dta->state = ST_MEMORY_BUSY;
						p_task_memory_dta->tick = TIMER_MEM_BUSY;
					}else{
						p_task_memory_dta->state = ST_MEMORY_IDLE;
						p_task_memory_dta->flag = true;
						p_task_memory_dta->event = EV_MEMORY_WRITE;

					}
				}


			default:

				p_task_memory_dta->tick  = DEL_I2C_XX_MIN;
				p_task_memory_dta->state = ST_MEMORY_IDLE;
				p_task_memory_dta->event = EV_MEMORY_READ;

				break;
		}
	}
}


/********************** end of file ******************************************/
