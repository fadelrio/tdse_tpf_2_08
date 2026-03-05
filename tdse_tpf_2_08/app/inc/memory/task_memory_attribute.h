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
 * @file   : task_sensor_attribute.h
 * @date   : Set 26, 2023
 * @author : Juan Manuel Cruz <jcruz@fi.uba.ar> <jcruz@frba.utn.edu.ar>
 * @version	v1.0.0
 */

#ifndef TASK_INC_TASK_MEMORY_ATTRIBUTE_H_
#define TASK_INC_TASK_MEMORY_ATTRIBUTE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

#include "main.h"
#include "stdbool.h"
#include "system/task_system_attribute.h"

/********************** macros ***********************************************/

#define MAX_BUFFER_SIZE 4

/********************** typedef **********************************************/

/* Events to excite Task Sensor */
typedef enum task_memory_ev {EV_MEMORY_READ, EV_MEMORY_WRITE} task_memory_ev_t;

/* States of Task Sensor */
typedef enum task_memory_st {ST_MEMORY_IDLE,
	ST_MEMORY_READING,
	ST_MEMORY_WRITING,
ST_MEMORY_BUSY} task_memory_st_t;

/* Identifier of Task Sensor */
typedef enum task_memory_id {ID_24C256} task_memory_id_t;

typedef struct
{
	task_memory_id_t	identifier;
	uint16_t 			address;
	I2C_HandleTypeDef *i2c_handler;
	uint32_t			tick_max;
	task_system_ev_t	mem_read_ready;
} task_memory_cfg_t;

typedef struct{
	void		*data;
	uint16_t		mem_adress;
	uint16_t		size;
}mem_data_t;

typedef struct
{
	uint32_t			tick;
	task_memory_st_t	state;
	task_memory_ev_t	event;
	bool 				flag;
	mem_data_t			write_data;
	mem_data_t			read_data;
} task_memory_dta_t;


/********************** external data declaration ****************************/
extern task_memory_dta_t task_memory_dta_list[];

/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_SENSOR_ATTRIBUTE_H_ */

/********************** end of file ******************************************/
