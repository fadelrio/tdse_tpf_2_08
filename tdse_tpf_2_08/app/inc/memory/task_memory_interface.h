#ifndef CONTROL_INC_TASK_SENSOR_DIGITAL_INTERFACE_H_
#define CONTROL_INC_TASK_SENSOR_DIGITAL_INTERFACE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/
extern bool write_task_memory(task_memory_id_t id, uint16_t adress, void *data, uint16_t size);
extern bool read_task_memory(task_memory_id_t id, uint16_t adress, void *data, uint16_t size);
extern task_memory_st_t get_state_task_memory(task_memory_id_t id);
/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif
