#ifndef MENU_INC_MENU_ATTRIBUTE_H_
#define MENU_INC_MENU_ATTRIBUTE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/

typedef enum menu_ev {IDLE_MENU/*COMPLETAR CON EVENTOS*/} menu_ev_t;

typedef enum menu_st {NADA_MENU/*COMPLETAR CON ESTADOS*/} menu_st_t;

typedef struct
{
	uint32_t			tick;
	menu_st_t	state;
	menu_ev_t	event;
	bool				flag;
} menu_dta_t;

/********************** external data declaration ****************************/
extern menu_dta_t menu_dta;

/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_SYSTEM_ATTRIBUTE_H_ */

/********************** end of file ******************************************/
