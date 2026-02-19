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
extern void init_queue_event_task_sensor_digital(void);
extern void put_event_task_sensor_digital(task_sensor_digital_ev_t event);
extern task_sensor_digital_ev_t get_event_task_sensor_digital(void);
extern bool any_event_task_sensor_digital(void);
extern float get_humidity_measure();
extern float get_temperature_measure();
extern void set_temperature_measure(float temperature);
extern void set_humidity_measure(float humidity);


/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif
