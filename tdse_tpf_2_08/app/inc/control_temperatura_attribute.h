#ifndef CONTROL_INC_CONTROL_TEMPERATURA_ATTRIBUTE_H_
#define CONTROL_INC_CONTROL_TEMPERATURA_ATTRIBUTE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/

typedef enum control_temperatura_ev {IDLE_TEMP,TICK,SENSE_TEMP_READY,ENTRY} control_temperatura_ev_t;

typedef enum control_temperatura_st {NADA_TEMP,CHECK_TEMP,ENFRIAR,SENSE_FRIO,CALENTAR,SENSE_CALOR} control_temperatura_st_t;


typedef struct
{
	uint32_t			tick;
	control_temperatura_st_t	state;
	control_temperatura_ev_t	event;
	bool				flag;
} control_temperatura_dta_t;

/********************** external data declaration ****************************/
extern control_temperatura_dta_t control_temperatura_dta;

/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_SYSTEM_ATTRIBUTE_H_ */

/********************** end of file ******************************************/
