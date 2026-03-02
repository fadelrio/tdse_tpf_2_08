#ifndef CONTROL_INC_TASK_SENSOR_ANALOGICO_INTERFACE_H_
#define CONTROL_INC_TASK_SENSOR_ANALOGICO_INTERFACE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/
extern void start_riego_measurement_task_sensor_analogico(void);
extern void start_luz_measurement_task_sensor_analogico(void);
extern uint32_t get_riego_task_sensor_analogico(void);
extern uint32_t get_luz_task_sensor_analogico(void);

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif
