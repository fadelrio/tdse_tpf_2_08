/********************** inclusions *******************************************/
/* Project includes */
#include "main.h"

/* Demo includes */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes */
#include "board.h"
#include "app.h"
#include "control_riego_attribute.h"

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
	control_riego_ev_t	queue[MAX_EVENTS];
} queue_control_riego;

/********************** external data declaration ****************************/

/********************** external functions definition ************************/
void init_queue_event_control_riego(void)
{
	uint32_t i;

	queue_control_riego.head = 0;
	queue_control_riego.tail = 0;
	queue_control_riego.count = 0;
	queue_control_riego.enable = true;

	for (i = 0; i < MAX_EVENTS; i++)
		queue_control_riego.queue[i] = EVENT_UNDEFINED;
}

void put_event_control_riego(control_riego_ev_t event)
{
	if (queue_control_riego.enable){
		queue_control_riego.count++;
		queue_control_riego.queue[queue_control_riego.head++] = event;

		if (MAX_EVENTS == queue_control_riego.head)
			queue_control_riego.head = 0;

	}

}

control_riego_ev_t get_event_control_riego(void)
{
	control_riego_ev_t event;

	queue_control_riego.count--;
	event = queue_control_riego.queue[queue_control_riego.tail];
	queue_control_riego.queue[queue_control_riego.tail++] = EVENT_UNDEFINED;

	if (MAX_EVENTS == queue_control_riego.tail)
		queue_control_riego.tail = 0;

	return event;
}

bool any_event_control_riego(void)
{
  return (queue_control_riego.head != queue_control_riego.tail);
}

void enable_queue_control_riego(void){
	queue_control_riego.enable = true;
}

void disable_queue_control_riego(void){
	if(any_event_control_riego()){
		uint32_t i;
		for (i = 0; i < MAX_EVENTS; i++)
				queue_control_riego.queue[i] = EVENT_UNDEFINED;

		queue_control_riego.head = 0;
		queue_control_riego.tail = 0;
		queue_control_riego.count = 0;

	}
	queue_control_riego.enable = false;
}

/********************** end of file ******************************************/
