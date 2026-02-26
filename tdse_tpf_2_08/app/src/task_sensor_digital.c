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
#include "task_system_attribute.h"
#include "task_system_interface.h"
#include "control_temperatura_attribute.h"
#include "control_humedad_attribute.h"
#include "control_temperatura_interface.h"
#include "control_humedad_interface.h"
#include "task_sensor_digital_attribute.h"
#include "task_sensor_digital_interface.h"
#include "control_temperatura_attribute.h"
#include "control_humedad_attribute.h"
#include "control_temperatura_interface.h"
#include "control_humedad_interface.h"

/********************** macros and definitions *******************************/
#define G_TASK_SEN_CNT_INIT			0ul
#define G_TASK_SEN_TICK_CNT_INI		0ul

#define DEL_I2C_XX_MIN				0ul
#define DEL_I2C_XX_MED				25ul
#define DEL_I2C_XX_MAX				50ul

#define SHT30_ADDRESS 0x45

/********************** external data declaration ****************************/
extern I2C_HandleTypeDef hi2c1;

/********************** internal data declaration ****************************/
const task_sensor_digital_cfg_t task_sensor_digital_cfg_list[] = {
		{ID_SHT30, SHT30_ADDRESS << 1u, &hi2c1, DEL_I2C_XX_MAX,
				SENSE_HUM_READY, SENSE_TEMP_READY}
};

#define SENSOR_CFG_QTY	(sizeof(task_sensor_cfg_list)/sizeof(task_sensor_cfg_t))

task_sensor_digital_dta_t task_sensor_digital_dta_list[] = {
		{DEL_I2C_XX_MIN, ST_SENSOR_DIGITAL_IDLE, EV_NADA_SENSOR_DIGITAL}
};

#define SENSOR_DTA_QTY	(sizeof(task_sensor_digital_dta_list)/sizeof(task_sensor_digital_dta_t))

uint8_t command_buffer[2];

uint8_t rx_buffer[6];


typedef enum
{
	SHT3X_COMMAND_MEASURE_HIGHREP_STRETCH = 0x2c06,
	SHT3X_COMMAND_CLEAR_STATUS = 0x3041,
	SHT3X_COMMAND_SOFT_RESET = 0x30A2,
	SHT3X_COMMAND_HEATER_ENABLE = 0x306d,
	SHT3X_COMMAND_HEATER_DISABLE = 0x3066,
	SHT3X_COMMAND_READ_STATUS = 0xf32d,
	SHT3X_COMMAND_FETCH_DATA = 0xe000,
	SHT3X_COMMAND_MEASURE_HIGHREP_10HZ = 0x2737,
	SHT3X_COMMAND_MEASURE_LOWREP_10HZ = 0x272a
} sht3x_command_t;

/********************** internal functions declaration ***********************/
void task_sensor_digital_statechart(void);
static uint8_t calculate_crc(const uint8_t *data, size_t length);
static uint16_t uint8_to_uint16(uint8_t msb, uint8_t lsb);

/********************** internal data definition *****************************/
const char *p_task_sensor_digital 		= "Task Sensor Digital (Sensor digital Statechart)";
const char *p_task_sensor_digital_ 		= "Non-Blocking & Update By Time Code";

/********************** external data declaration ****************************/
uint32_t g_task_sensor_digital_cnt;
volatile uint32_t g_task_sensor_digital_tick_cnt;

/********************** external functions definition ************************/
void task_sensor_digital_init(void *parameters)
{
	uint32_t index;
	task_sensor_digital_dta_t *p_task_sensor_dta;
	task_sensor_digital_st_t state;
	task_sensor_digital_ev_t event;

	/* Print out: Task Initialized */
	LOGGER_INFO(" ");
	LOGGER_INFO("  %s is running - %s", GET_NAME(task_sensor_init), p_task_sensor_digital);
	LOGGER_INFO("  %s is a %s", GET_NAME(task_sensor), p_task_sensor_digital_);

	/* Init & Print out: Task execution counter */
	g_task_sensor_digital_cnt = G_TASK_SEN_CNT_INIT;
	LOGGER_INFO("   %s = %lu", GET_NAME(g_task_sensor_digital_cnt), g_task_sensor_digital_cnt);

	for (index = 0; SENSOR_DTA_QTY > index; index++)
	{
		/* Update Task Sensor Data Pointer */
		p_task_sensor_dta = &task_sensor_digital_dta_list[index];

		/* Init & Print out: Index & Task execution FSM */
		state = ST_SENSOR_DIGITAL_IDLE;
		p_task_sensor_dta->state = state;

		event = EV_NADA_SENSOR_DIGITAL;
		p_task_sensor_dta->event = event;

		LOGGER_INFO(" ");
		LOGGER_INFO("   %s = %lu   %s = %lu   %s = %lu",
				    GET_NAME(index), index,
					GET_NAME(state), (uint32_t)state,
					GET_NAME(event), (uint32_t)event);
	}
}

void task_sensor_digital_update(void *parameters)
{
	bool b_time_update_required = false;

	/* Protect shared resource */
	__asm("CPSID i");	/* disable interrupts */
    if (G_TASK_SEN_TICK_CNT_INI < g_task_sensor_digital_tick_cnt)
    {
		/* Update Tick Counter */
    	g_task_sensor_digital_tick_cnt--;
    	b_time_update_required = true;
    }
    __asm("CPSIE i");	/* enable interrupts */

    while (b_time_update_required)
    {
		/* Update Task Counter */
		g_task_sensor_digital_cnt++;

		/* Run Task Statechart */
    	task_sensor_digital_statechart();

    	/* Protect shared resource */
		__asm("CPSID i");	/* disable interrupts */
		if (G_TASK_SEN_TICK_CNT_INI < g_task_sensor_digital_tick_cnt)
		{
			/* Update Tick Counter */
			g_task_sensor_digital_tick_cnt--;
			b_time_update_required = true;
		}
		else
		{
			b_time_update_required = false;
		}
		__asm("CPSIE i");	/* enable interrupts */
    }
}

void task_sensor_digital_statechart(void)
{
	uint32_t index;
	const task_sensor_digital_cfg_t *p_task_sensor_cfg;
	task_sensor_digital_dta_t *p_task_sensor_dta;

	for (index = 0; SENSOR_DTA_QTY > index; index++)
	{
		/* Update Task Sensor Configuration & Data Pointer */
		p_task_sensor_cfg = &task_sensor_digital_cfg_list[index];
		p_task_sensor_dta = &task_sensor_digital_dta_list[index];

		if (true == any_event_task_sensor_digital())
				{
					p_task_sensor_dta->flag = true;
					p_task_sensor_dta->event = get_event_task_sensor_digital();
				}

		switch (p_task_sensor_dta->state)
		{
			case ST_SENSOR_DIGITAL_IDLE:
				if (p_task_sensor_dta->flag == true){
					if(p_task_sensor_dta->event == EV_START_MEASUREMENT_DIGITAL){
						p_task_sensor_dta->state = ST_SENSOR_DIGITAL_SEND_DIRECTIVE;
						command_buffer[0] = (SHT3X_COMMAND_MEASURE_HIGHREP_STRETCH & 0xff00u) >> 8u;
						command_buffer[1] = SHT3X_COMMAND_MEASURE_HIGHREP_STRETCH & 0xffu;
						if(HAL_I2C_Master_Transmit_IT(p_task_sensor_cfg->i2c_handler, p_task_sensor_cfg->address, command_buffer, sizeof(command_buffer)) != HAL_OK){
							Error_Handler();
						}
					}

					p_task_sensor_dta->flag = false;
				}

				break;

			case ST_SENSOR_DIGITAL_SEND_DIRECTIVE:
				if (HAL_I2C_GetState(p_task_sensor_cfg->i2c_handler) == HAL_I2C_STATE_READY){
					if(HAL_I2C_GetError(p_task_sensor_cfg->i2c_handler) != HAL_I2C_ERROR_AF){
						p_task_sensor_dta->state = ST_SENSOR_DIGITAL_READ_VALUE;
						if(HAL_I2C_Master_Receive_IT(p_task_sensor_cfg->i2c_handler, p_task_sensor_cfg->address, (uint8_t *)rx_buffer, sizeof(rx_buffer)) != HAL_OK){
							  Error_Handler();
							}
					}else {
						command_buffer[0] = (SHT3X_COMMAND_MEASURE_HIGHREP_STRETCH & 0xff00u) >> 8u;
						command_buffer[1] = SHT3X_COMMAND_MEASURE_HIGHREP_STRETCH & 0xffu;
						if(HAL_I2C_Master_Transmit_IT(p_task_sensor_cfg->i2c_handler, p_task_sensor_cfg->address, command_buffer, sizeof(command_buffer)) != HAL_OK){
							Error_Handler();
						}
					}
				}
				p_task_sensor_dta->flag = false;
				break;

			case ST_SENSOR_DIGITAL_READ_VALUE:
				if (HAL_I2C_GetState(p_task_sensor_cfg->i2c_handler) == HAL_I2C_STATE_READY){
					if(HAL_I2C_GetError(p_task_sensor_cfg->i2c_handler) != HAL_I2C_ERROR_AF){
						uint8_t temperature_crc = calculate_crc(rx_buffer, 2);
						uint8_t humidity_crc = calculate_crc(rx_buffer + 3, 2);
						if (temperature_crc != rx_buffer[2] || humidity_crc != rx_buffer[5]) {
							p_task_sensor_dta->state = ST_SENSOR_DIGITAL_IDLE;
							put_event_task_sensor_digital(EV_START_MEASUREMENT_DIGITAL);
						}else {
							uint16_t temperature_raw = uint8_to_uint16(rx_buffer[0], rx_buffer[1]);
							uint16_t humidity_raw = uint8_to_uint16(rx_buffer[3], rx_buffer[4]);
							set_temperature_measure(-45.0f + 175.0f * (float)temperature_raw / 65535.0f);
							set_humidity_measure(100.0f * (float)humidity_raw / 65535.0f);
							put_event_control_humedad(p_task_sensor_cfg->ev_humedad_ready);
							put_event_control_temperatura(p_task_sensor_cfg->ev_temperatura_ready);
							p_task_sensor_dta->state = ST_SENSOR_DIGITAL_IDLE;
						}
					}else {
						if(HAL_I2C_Master_Receive_IT(p_task_sensor_cfg->i2c_handler, p_task_sensor_cfg->address, (uint8_t *)rx_buffer, sizeof(rx_buffer)) != HAL_OK){
							Error_Handler();
						}
					}
				}
				p_task_sensor_dta->flag = false;
				break;

			default:

				p_task_sensor_dta->tick  = DEL_I2C_XX_MIN;
				p_task_sensor_dta->state = ST_SENSOR_DIGITAL_IDLE;
				p_task_sensor_dta->event = EV_NADA_SENSOR_DIGITAL;

				break;
		}
	}
}

static uint8_t calculate_crc(const uint8_t *data, size_t length)
{
	uint8_t crc = 0xff;
	for (size_t i = 0; i < length; i++) {
		crc ^= data[i];
		for (size_t j = 0; j < 8; j++) {
			if ((crc & 0x80u) != 0) {
				crc = (uint8_t)((uint8_t)(crc << 1u) ^ 0x31u);
			} else {
				crc <<= 1u;
			}
		}
	}
	return crc;

}

static uint16_t uint8_to_uint16(uint8_t msb, uint8_t lsb)
{
	return (uint16_t)((uint16_t)msb << 8u) | lsb;
}

/********************** end of file ******************************************/
