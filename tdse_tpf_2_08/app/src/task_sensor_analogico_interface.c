/********************** inclusions *******************************************/
/* Project includes */
#include "main.h"

/* Demo includes */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes */
#include "board.h"
#include "app.h"
#include "task_sensor_analogico_attribute.h"

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
	task_sensor_analogico_ev_t	queue[MAX_EVENTS];
	float luz;
	float riego;
} queue_task_sensor_analogico;

/********************** external data declaration ****************************/

/********************** external functions definition ************************/
void init_queue_event_task_sensor_analogico(void)
{
	uint32_t i;

	queue_task_sensor_analogico.head = 0;
	queue_task_sensor_analogico.tail = 0;
	queue_task_sensor_analogico.count = 0;
	queue_task_sensor_analogico.luz = 0;
	queue_task_sensor_analogico.riego = 0;

	for (i = 0; i < MAX_EVENTS; i++)
		queue_task_sensor_analogico.queue[i] = EVENT_UNDEFINED;
}

void put_event_task_sensor_analogico(task_sensor_analogico_ev_t event)
{
	queue_task_sensor_analogico.count++;
	queue_task_sensor_analogico.queue[queue_task_sensor_analogico.head++] = event;

	if (MAX_EVENTS == queue_task_sensor_analogico.head)
		queue_task_sensor_analogico.head = 0;

}

task_sensor_analogico_ev_t get_event_task_sensor_analogico(void)
{
	task_sensor_analogico_ev_t event;

	queue_task_sensor_analogico.count--;
	event = queue_task_sensor_analogico.queue[queue_task_sensor_analogico.tail];
	queue_task_sensor_analogico.queue[queue_task_sensor_analogico.tail++] = EVENT_UNDEFINED;

	if (MAX_EVENTS == queue_task_sensor_analogico.tail)
		queue_task_sensor_analogico.tail = 0;

	return event;
}

bool any_event_task_sensor_analogico(void)
{
  return (queue_task_sensor_analogico.head != queue_task_sensor_analogico.tail);
}


float get_luz_measure(){
	return queue_task_sensor_analogico.luz;
}


float get_riego_measure(){
	return queue_task_sensor_analogico.riego;
}


void set_luz_measure(float luz){
	queue_task_sensor_analogico.luz = luz;
}


void set_riego_measure(float riego){
	queue_task_sensor_analogico.riego = riego;
}




/********************** end of file ******************************************/
