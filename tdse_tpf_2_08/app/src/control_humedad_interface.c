/********************** inclusions *******************************************/
/* Project includes */
#include "main.h"

/* Demo includes */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes */
#include "board.h"
#include "app.h"
#include "control_humedad_attribute.h"

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
	control_humedad_ev_t	queue[MAX_EVENTS];
} queue_control_humedad;

/********************** external data declaration ****************************/

/********************** external functions definition ************************/
void init_queue_event_control_humedad(void)
{
	uint32_t i;

	queue_control_humedad.head = 0;
	queue_control_humedad.tail = 0;
	queue_control_humedad.count = 0;
	queue_control_humedad.enable = true;

	for (i = 0; i < MAX_EVENTS; i++)
		queue_control_humedad.queue[i] = EVENT_UNDEFINED;
}

void put_event_control_humedad(control_humedad_ev_t event)
{
	if (queue_control_humedad.enable){
		queue_control_humedad.count++;
		queue_control_humedad.queue[queue_control_humedad.head++] = event;

		if (MAX_EVENTS == queue_control_humedad.head)
			queue_control_humedad.head = 0;

	}

}

control_humedad_ev_t get_event_control_humedad(void)
{
	control_humedad_ev_t event;

	queue_control_humedad.count--;
	event = queue_control_humedad.queue[queue_control_humedad.tail];
	queue_control_humedad.queue[queue_control_humedad.tail++] = EVENT_UNDEFINED;

	if (MAX_EVENTS == queue_control_humedad.tail)
		queue_control_humedad.tail = 0;

	return event;
}

bool any_event_control_humedad(void)
{
  return (queue_control_humedad.head != queue_control_humedad.tail);
}

void enable_queue_control_humedad(void){
	queue_control_humedad.enable = true;
}

void disable_queue_control_humedad(void){
	if(any_event_control_humedad()){
		uint32_t i;
		for (i = 0; i < MAX_EVENTS; i++)
				queue_control_humedad.queue[i] = EVENT_UNDEFINED;

		queue_control_humedad.head = 0;
		queue_control_humedad.tail = 0;
		queue_control_humedad.count = 0;

	}
	queue_control_humedad.enable = false;
}

/********************** end of file ******************************************/
