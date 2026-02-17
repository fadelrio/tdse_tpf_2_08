/********************** inclusions *******************************************/
/* Project includes */
#include "main.h"

/* Demo includes */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes */
#include "board.h"
#include "app.h"
#include "menu_attribute.h"

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
	menu_ev_t	queue[MAX_EVENTS];
} queue_menu;

/********************** external data declaration ****************************/

/********************** external functions definition ************************/
void init_queue_event_menu(void)
{
	uint32_t i;

	queue_menu.head = 0;
	queue_menu.tail = 0;
	queue_menu.count = 0;
	queue_menu.enable = true;

	for (i = 0; i < MAX_EVENTS; i++)
		queue_menu.queue[i] = EVENT_UNDEFINED;
}

void put_event_menu(menu_ev_t event)
{
	if (queue_menu.enable){
		queue_menu.count++;
		queue_menu.queue[queue_menu.head++] = event;

		if (MAX_EVENTS == queue_menu.head)
			queue_menu.head = 0;

	}

}

menu_ev_t get_event_menu(void)
{
	menu_ev_t event;

	queue_menu.count--;
	event = queue_menu.queue[queue_menu.tail];
	queue_menu.queue[queue_menu.tail++] = EVENT_UNDEFINED;

	if (MAX_EVENTS == queue_menu.tail)
		queue_menu.tail = 0;

	return event;
}

bool any_event_menu(void)
{
  return (queue_menu.head != queue_menu.tail);
}

void enable_queue_menu(void){
	queue_menu.enable = true;
}

void disable_queue_menu(void){
	if(any_event_menu()){
		uint32_t i;
		for (i = 0; i < MAX_EVENTS; i++)
				queue_menu.queue[i] = EVENT_UNDEFINED;

		queue_menu.head = 0;
		queue_menu.tail = 0;
		queue_menu.count = 0;

	}
	queue_menu.enable = false;
}

/********************** end of file ******************************************/
