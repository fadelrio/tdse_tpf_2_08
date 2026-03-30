#ifndef TASK_INC_TASK_DISPLAY_INTERFACE_H_
#define TASK_INC_TASK_DISPLAY_INTERFACE_H_

/********************** CPP guard ********************************************/
#ifdef __cplusplus
extern "C" {
#endif

/********************** inclusions *******************************************/
#include "display/task_display_attribute.h"
/********************** macros ***********************************************/

/********************** typedef **********************************************/

/********************** external data declaration ****************************/

/********************** external functions declaration ***********************/
extern void init_queue_event_task_display(void);
extern void print_string_task_display(const char* str, char line); // asumimos que el usuario mete strings de no mas de 16 caracteres xq es el tamaño de mi display
extern task_display_buff_string_t* get_buffered_string_task_display(void);  // para comunicacion interna con el task_display.c
extern bool any_string_task_display(void);


/********************** End of CPP guard *************************************/
#ifdef __cplusplus
}
#endif

#endif
