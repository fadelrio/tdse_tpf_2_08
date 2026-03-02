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

#ifndef TASK_INC_TASK_SENSOR_ANALOGICO_ATTRIBUTE_H_
#define TASK_INC_TASK_SENSOR_ANALOGICO_ATTRIBUTE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include "control/control_riego_attribute.h"
#include "control/control_luz_attribute.h"
/********************** macros ***********************************************/

/********************** typedef **********************************************/

/* Events to excite Task Sensor */
typedef enum task_sensor_analogico_ev {EV_SENSOR_ANALOGICO_START_MEASUREMENT,EV_SENSOR_ANALOGICO_NADA, EV_SENSOR_ANALOGICO_MEASURE_READY} task_sensor_analogico_ev_t;

/* States of Task Sensor */
typedef enum task_sensor_analogico_st {ST_SENSOR_ANALOGICO_IDLE, ST_SENSOR_ANALOGICO_MEASURING} task_sensor_analogico_st_t;

/* Identifier of Task Sensor */
typedef enum task_sensor_analogico_id {ID_SENSOR_LUZ, ID_SENSOR_RIEGO} task_sensor_analogico_id_t;

typedef struct
{
	task_sensor_analogico_id_t	identifier;
	uint32_t					tick_max;
	ADC_HandleTypeDef			*handler;
	control_riego_ev_t			measure_ready_riego;
	control_luz_ev_t			measure_ready_luz;
} task_sensor_analogico_cfg_t;

typedef struct
{
	uint32_t					tick;
	task_sensor_analogico_st_t	state;
	task_sensor_analogico_ev_t	event;
	bool 						flag;
	uint32_t 					measure;
} task_sensor_analogico_dta_t;

/********************** external data declaration ****************************/
extern task_sensor_analogico_dta_t task_sensor_analogico_dta_list[];

/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_SENSOR_ATTRIBUTE_H_ */

/********************** end of file ******************************************/
