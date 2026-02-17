/********************** inclusions *******************************************/
/* Project includes */
#include "main.h"

/* Demo includes */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes */
#include "board.h"
#include "app.h"
#include "control_temperatura_attribute.h"

/********************** macros and definitions *******************************/
#define EVENT_UNDEFINED	(255)
#define MAX_EVENTS		(16)

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
struct
{
	uint32_t	head;
	uint32_t	tail;
	uint32_t	count;
	bool enable;
	control_temperatura_ev_t	queue[MAX_EVENTS];
} queue_control_temperatura;

/********************** external data declaration ****************************/

/********************** external functions definition ************************/
void init_queue_event_control_temperatura(void)
{
	uint32_t i;

	queue_control_temperatura.head = 0;
	queue_control_temperatura.tail = 0;
	queue_control_temperatura.count = 0;
	queue_control_temperatura.enable = true;

	for (i = 0; i < MAX_EVENTS; i++)
		queue_control_temperatura.queue[i] = EVENT_UNDEFINED;
}

void put_event_control_temperatura(control_temperatura_ev_t event)
{
	if (queue_control_temperatura.enable){
		queue_control_temperatura.count++;
		queue_control_temperatura.queue[queue_control_temperatura.head++] = event;

		if (MAX_EVENTS == queue_control_temperatura.head)
			queue_control_temperatura.head = 0;

	}

}

control_temperatura_ev_t get_event_control_temperatura(void)
{
	control_temperatura_ev_t event;

	queue_control_temperatura.count--;
	event = queue_control_temperatura.queue[queue_control_temperatura.tail];
	queue_control_temperatura.queue[queue_control_temperatura.tail++] = EVENT_UNDEFINED;

	if (MAX_EVENTS == queue_control_temperatura.tail)
		queue_control_temperatura.tail = 0;

	return event;
}

bool any_event_control_temperatura(void)
{
  return (queue_control_temperatura.head != queue_control_temperatura.tail);
}

void enable_queue_control_temperatura(void){
	queue_control_temperatura.enable = true;
}

void disable_queue_control_temperatura(void){
	if(any_event_control_temperatura()){
		uint32_t i;
		for (i = 0; i < MAX_EVENTS; i++)
				queue_control_temperatura.queue[i] = EVENT_UNDEFINED;

		queue_control_temperatura.head = 0;
		queue_control_temperatura.tail = 0;
		queue_control_temperatura.count = 0;

	}
	queue_control_temperatura.enable = false;
}

/********************** end of file ******************************************/
