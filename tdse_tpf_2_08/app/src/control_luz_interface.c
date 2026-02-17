/********************** inclusions *******************************************/
/* Project includes */
#include "main.h"

/* Demo includes */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes */
#include "board.h"
#include "app.h"
#include "control_luz_attribute.h"

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
	control_luz_ev_t	queue[MAX_EVENTS];
} queue_control_luz;

/********************** external data declaration ****************************/

/********************** external functions definition ************************/
void init_queue_event_control_luz(void)
{
	uint32_t i;

	queue_control_luz.head = 0;
	queue_control_luz.tail = 0;
	queue_control_luz.count = 0;
	queue_control_luz.enable = true;

	for (i = 0; i < MAX_EVENTS; i++)
		queue_control_luz.queue[i] = EVENT_UNDEFINED;
}

void put_event_control_luz(control_luz_ev_t event)
{
	if (queue_control_luz.enable){
		queue_control_luz.count++;
		queue_control_luz.queue[queue_control_luz.head++] = event;

		if (MAX_EVENTS == queue_control_luz.head)
			queue_control_luz.head = 0;

	}

}

control_luz_ev_t get_event_control_luz(void)
{
	control_luz_ev_t event;

	queue_control_luz.count--;
	event = queue_control_luz.queue[queue_control_luz.tail];
	queue_control_luz.queue[queue_control_luz.tail++] = EVENT_UNDEFINED;

	if (MAX_EVENTS == queue_control_luz.tail)
		queue_control_luz.tail = 0;

	return event;
}

bool any_event_control_luz(void)
{
  return (queue_control_luz.head != queue_control_luz.tail);
}

void enable_queue_control_luz(void){
	queue_control_luz.enable = true;
}

void disable_queue_control_luz(void){
	if(any_event_control_luz()){
		uint32_t i;
		for (i = 0; i < MAX_EVENTS; i++)
				queue_control_luz.queue[i] = EVENT_UNDEFINED;

		queue_control_luz.head = 0;
		queue_control_luz.tail = 0;
		queue_control_luz.count = 0;

	}
	queue_control_luz.enable = false;
}

/********************** end of file ******************************************/
