/********************** inclusions *******************************************/
/* Project includes */
#include "main.h"

/* Demo includes */
#include "logger.h"
#include "dwt.h"

/* Application & Tasks includes */
#include "board.h"
#include "app.h"
#include "display/task_display_attribute.h"

/********************** macros and definitions *******************************/
#define EVENT_UNDEFINED	(255)
#define MAX_STRINGS		(4)

/********************** internal data declaration ****************************/

/********************** internal functions declaration ***********************/

/********************** internal data definition *****************************/
struct
{
	uint32_t	head;
	uint32_t	tail;
	uint32_t	count;
	task_display_buff_string_t	queue[MAX_STRINGS];
} queue_task_display;

task_display_buff_string_t empty_buff_string = {"                ", 0};

/********************** external data declaration ****************************/

/********************** external functions definition ************************/
void init_queue_event_task_display(void)
{
	uint32_t i;

	queue_task_display.head = 0;
	queue_task_display.tail = 0;
	queue_task_display.count = 0;

	for (i = 0; i < MAX_STRINGS; i++)
		queue_task_display.queue[i] = empty_buff_string;
}

void print_string_task_display(const char str[], char line)
{
	queue_task_display.count++;
	task_display_buff_string_t temp_buff_string;
	temp_buff_string.line = line;
	strcpy(temp_buff_string.string, str);
	queue_task_display.queue[queue_task_display.head++] = temp_buff_string;

	if (MAX_STRINGS == queue_task_display.head)
		queue_task_display.head = 0;



}

task_display_buff_string_t* get_buffered_string_task_display(void)
{
	task_display_buff_string_t *p_buff_string;

	queue_task_display.count--;
	p_buff_string = &queue_task_display.queue[queue_task_display.tail];
	queue_task_display.tail++;

	if (MAX_STRINGS == queue_task_display.tail)
		queue_task_display.tail = 0;

	return p_buff_string;
}

bool any_string_task_display(void)
{
  return (queue_task_display.head != queue_task_display.tail);
}

/********************** end of file ******************************************/
