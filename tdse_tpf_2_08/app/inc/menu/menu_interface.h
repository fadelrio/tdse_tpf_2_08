#ifndef MENU_INC_MENU_INTERFACE_H_
#define MENU_INC_MENU_INTERFACE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/
extern void init_queue_event_menu(void);
extern void put_event_menu(menu_ev_t event);
extern menu_ev_t get_event_menu(void);
extern bool any_event_menu(void);
extern void enable_queue_menu(void);
extern void disable_queue_menu(void);


/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif
