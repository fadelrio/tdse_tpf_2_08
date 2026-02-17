#ifndef CONTROL_INC_CONTROL_RIEGO_ATTRIBUTE_H_
#define CONTROL_INC_CONTROL_RIEGO_ATTRIBUTE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/

typedef enum control_riego_ev {IDLE_RIEGO/*COMPLETAR CON EVENTOS*/} control_riego_ev_t;

typedef enum control_riego_st {NADA_RIEGO/*COMPLETAR CON ESTADOS*/} control_riego_st_t;

typedef struct
{
	uint32_t			tick;
	control_riego_st_t	state;
	control_riego_ev_t	event;
	bool				flag;
} control_riego_dta_t;

/********************** external data declaration ****************************/
extern control_riego_dta_t control_riego_dta;

/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_SYSTEM_ATTRIBUTE_H_ */

/********************** end of file ******************************************/
