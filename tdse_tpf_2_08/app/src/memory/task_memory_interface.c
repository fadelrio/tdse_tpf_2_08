/********************** inclusions *******************************************/
/* Project includes */
#include "main.h"

/* Demo includes */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes */
#include "board.h"
#include "app.h"
#include <memory/task_memory_attribute.h>

/********************** macros and definitions *******************************/

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/

/********************** external data declaration ****************************/

/********************** external functions definition ************************/
bool write_task_memory(task_memory_id_t id, uint16_t adress, void *data, uint16_t size){
	task_memory_dta_t *p_task_memory_dta = &task_memory_dta_list[id];
	if(p_task_memory_dta->state != ST_MEMORY_WRITING){
		p_task_memory_dta->event = EV_MEMORY_WRITE;
		p_task_memory_dta->flag = true;
		p_task_memory_dta->write_data.data = data;
		p_task_memory_dta->write_data.mem_adress = adress;
		p_task_memory_dta->write_data.size = size;
		return true;
	}
	return false;
}
bool read_task_memory(task_memory_id_t id, uint16_t adress, void *data, uint16_t size){
	task_memory_dta_t *p_task_memory_dta = &task_memory_dta_list[id];
	if(p_task_memory_dta->state != ST_MEMORY_READING){
		p_task_memory_dta->event = EV_MEMORY_READ;
		p_task_memory_dta->flag = true;
		p_task_memory_dta->read_data.data = data;
		p_task_memory_dta->read_data.mem_adress = adress;
		p_task_memory_dta->read_data.size = size;
		return true;
	}
	return false;
}
task_memory_st_t get_state_task_memory(task_memory_id_t id){
	task_memory_dta_t *p_task_memory_dta = &task_memory_dta_list[id];
	return p_task_memory_dta->state;
}
/********************** end of file ******************************************/
