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
extern void init_queue_event_task_sensor_analogico(void);
extern void put_event_task_sensor_analogico(task_sensor_analogico_ev_t event);
extern task_sensor_analogico_ev_t get_event_task_sensor_analogico(void);
extern bool any_event_task_sensor_analogico(void);
extern float get_riego_measure();
extern float get_luz_measure();
extern void set_riego_measure(float riego);
extern void set_luz_measure(float luz);


/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif
