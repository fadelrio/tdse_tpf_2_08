/********************** inclusions *******************************************/
/* Project includes */
#include "main.h"

/* Demo includes */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes */
#include "board.h"
#include "app.h"
#include "sensors/task_sensor_analogico_attribute.h"

/********************** macros and definitions *******************************/

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data declaration ****************************/

/********************** external functions definition ************************/
void start_riego_measurement_task_sensor_analogico(void){
	task_sensor_analogico_dta_t *p_task_sensor_dta;
	p_task_sensor_dta = &task_sensor_analogico_dta_list[ID_SENSOR_RIEGO];
	p_task_sensor_dta->event = EV_SENSOR_ANALOGICO_START_MEASUREMENT;
	p_task_sensor_dta->flag = true;
}
void start_luz_measurement_task_sensor_analogico(void){
	task_sensor_analogico_dta_t *p_task_sensor_dta;
	p_task_sensor_dta = &task_sensor_analogico_dta_list[ID_SENSOR_LUZ];
	p_task_sensor_dta->event = EV_SENSOR_ANALOGICO_START_MEASUREMENT;
	p_task_sensor_dta->flag = true;
}
uint32_t get_riego_task_sensor_analogico(void){
	task_sensor_analogico_dta_t *p_task_sensor_dta;
	p_task_sensor_dta = &task_sensor_analogico_dta_list[ID_SENSOR_RIEGO];
	return p_task_sensor_dta->measure;
}
uint32_t get_luz_task_sensor_analogico(void){
	task_sensor_analogico_dta_t *p_task_sensor_dta;
	p_task_sensor_dta = &task_sensor_analogico_dta_list[ID_SENSOR_LUZ];
	return p_task_sensor_dta->measure;
}

/********************** end of file ******************************************/
