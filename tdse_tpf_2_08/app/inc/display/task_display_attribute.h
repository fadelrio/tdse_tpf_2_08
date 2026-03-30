#ifndef TASK_INC_TASK_DISPLAY_ATTRIBUTE_H_
#define TASK_INC_TASK_DISPLAY_ATTRIBUTE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/

/********************** macros ***********************************************/

/********************** typedef **********************************************/

typedef enum task_display_st {IDLE_TASK_DISPLAY, PRINT_LINE_TASK_DISPLAY} task_display_st_t;

typedef struct {
	char string[17];
	char line;
} task_display_buff_string_t;

typedef struct
{
	uint32_t			current_position;
	task_display_st_t	state;
	char* 				str;
} task_display_dta_t;

/********************** external data declaration ****************************/
extern task_display_dta_t task_display_dta;

/********************** external functions declaration ***********************/

/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif /* TASK_INC_TASK_SYSTEM_ATTRIBUTE_H_ */

/********************** end of file ******************************************/
