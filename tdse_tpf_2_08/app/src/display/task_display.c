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
 * @file   : task_display.c
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */
/**************************DISCLAIMER*****************************************/
/*
 * Este statechart existe con la intención de ralizar una adaptación no exhaustiva del
 * driver display.h. Se utilizan funciones del mismo para imprimir las lineas del display
 * de forma "no bloqueante" escribiendo un caracter por ciclo de la aplicación. Es probable
 * que se pueda disminuir el tiempo de ciclo que consume este statechart reimplrmrntando el
 * driver desde cero con una maquina de estados acorde, pero sacrificando el tiempo total
 * de escritura en el display.
 * Si este archivo con el disclaimer actual se encuentra en el pryecto final entregado es porque
 * la reimplementación con el driver display.h cumple con los requerimientos impuestos.
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
#include "display/task_display_attribute.h"
#include "display/task_display_interface.h"
#include "display.h"

/********************** macros and definitions *******************************/
#define G_TASK_SEN_CNT_INIT			0ul
#define G_TASK_SEN_TICK_CNT_INI		0ul

#define DEL_DISPLAY_XX_MIN				0ul
#define DEL_DISPLAY_XX_MED				25ul
#define DEL_DISPLAY_XX_MAX				50ul

/********************** internal data declaration ****************************/
task_display_dta_t task_display_dta_list[] = {
		{DEL_DISPLAY_XX_MIN, IDLE_TASK_DISPLAY, ""}
};

#define DISPLAY_DTA_QTY	(sizeof(task_display_dta_list)/sizeof(task_display_dta_t))

/********************** internal functions declaration ***********************/
void task_display_statechart(void);

/********************** internal data definition *****************************/
const char *p_task_display 		= "Task Display (Display Statechart)";
const char *p_task_display_ 		= "Non-Blocking & Update By Time Code";

/********************** external data declaration ****************************/
uint32_t g_task_display_cnt;
volatile uint32_t g_task_display_tick_cnt;

/********************** external functions definition ************************/
void task_display_init(void *parameters)
{
	uint32_t index;
	task_display_dta_t *p_task_display_dta;
	task_display_st_t state;

	init_queue_event_task_display();

	/* Print out: Task Initialized */
	LOGGER_INFO(" ");
	LOGGER_INFO("  %s is running - %s", GET_NAME(task_display_init), p_task_display);
	LOGGER_INFO("  %s is a %s", GET_NAME(task_display), p_task_display_);

	/* Init & Print out: Task execution counter */
	g_task_display_cnt = G_TASK_SEN_CNT_INIT;
	g_task_display_tick_cnt = G_TASK_SEN_CNT_INIT;
	LOGGER_INFO("   %s = %lu", GET_NAME(g_task_display_cnt), g_task_display_cnt);

	for (index = 0; DISPLAY_DTA_QTY > index; index++)
	{
		/* Update Task SDISPLAYData Pointer */
		p_task_display_dta = &task_display_dta_list[index];

		/* Init & Print out: Index & Task execution FSM */
		state = IDLE_TASK_DISPLAY;
		p_task_display_dta->state = state;


		LOGGER_INFO(" ");
		LOGGER_INFO("   %s = %lu   %s = %lu   %s = %lu",
				    GET_NAME(index), index,
					GET_NAME(state), (uint32_t)state);
	}
}

void task_display_update(void *parameters)
{
	bool b_time_update_required = false;

	/* Protect shared resource */
	__asm("CPSID i");	/* disable interrupts */
    if (G_TASK_SEN_TICK_CNT_INI < g_task_display_tick_cnt)
    {
		/* Update Tick Counter */
    	g_task_display_tick_cnt--;
    	b_time_update_required = true;
    }
    __asm("CPSIE i");	/* enable interrupts */

    while (b_time_update_required)
    {
		/* Update Task Counter */
		g_task_display_cnt++;

		/* Run Task Statechart */
    	task_display_statechart();

    	/* Protect shared resource */
		__asm("CPSID i");	/* disable interrupts */
		if (G_TASK_SEN_TICK_CNT_INI < g_task_display_tick_cnt)
		{
			/* Update Tick Counter */
			g_task_display_tick_cnt--;
			b_time_update_required = true;
		}
		else
		{
			b_time_update_required = false;
		}
		__asm("CPSIE i");	/* enable interrupts */
    }
}


void task_display_statechart(void)
{
	uint32_t index;
	task_display_dta_t *p_task_display_dta;

	for (index = 0; DISPLAY_DTA_QTY > index; index++)
	{
		/* Update Task display Configuration & Data Pointer */
		p_task_display_dta = &task_display_dta_list[index];



		switch (p_task_display_dta->state)
		{
			case IDLE_TASK_DISPLAY:
				if(any_string_task_display()){
					p_task_display_dta->state = PRINT_LINE_TASK_DISPLAY;
					task_display_buff_string_t *p_task_buff_str = get_buffered_string_task_display();
					p_task_display_dta->str = p_task_buff_str->string; //XXX strcpy entiendo que tiene un for, lo que sería bloqueante, hay que ver que tanto me jode
					displayCharPositionWrite(0,p_task_buff_str->line);
					p_task_display_dta->current_position = 0;

				}
				break;

			case PRINT_LINE_TASK_DISPLAY:
				if (p_task_display_dta->str[p_task_display_dta->current_position]){
					displayCharWrite(p_task_display_dta->str[p_task_display_dta->current_position]);
					p_task_display_dta->current_position++;
				}else {
					p_task_display_dta->state = IDLE_TASK_DISPLAY;
				}
				break;

			default:

				p_task_display_dta->current_position  = DEL_DISPLAY_XX_MIN;
				p_task_display_dta->state = IDLE_TASK_DISPLAY;

				break;
		}
	}
}

/********************** end of file ******************************************/
