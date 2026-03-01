#ifndef CONTROL_INC_CONTROL_LUZ_INTERFACE_H_
#define CONTROL_INC_CONTROL_LUZ_INTERFACE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/
extern void init_queue_event_control_luz(void);
extern void put_event_control_luz(control_luz_ev_t event);
extern control_luz_ev_t get_event_control_luz(void);
extern bool any_event_control_luz(void);
extern void enable_queue_control_luz(void);
extern void disable_queue_control_luz(void);


/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif
