#include <stdbool.h>
#include "cmsis_os.h"
#include "event.h"

osMessageQDef(event_msg_queue, 16, app_sched_event_handler_t);
osMessageQId event_msg_queue;
static uint32_t event_msg_queue_error_count = 0;

void event_init(void)
{
  // Create the event queue.
  event_msg_queue = osMessageCreate(osMessageQ(event_msg_queue), NULL);
}

void event_loop(void)
{
  for (;;)
  {
    osEvent event;
    event_t event_handler;

    // Wait for an event message on the queue.
    event = osMessageGet(event_msg_queue, osWaitForever);

    // Process if this is an event message.
    if (event.status == osEventMessage)
    {
      // A pointer to the event handler is passed in the event message.
      event_handler = (event_t) event.value.p;

      // Call the event handler.
      event_handler();
    }
  }
}

// Schedule an event to be processed.
void event_schedule(event_t event_handler)
{
  // Put the event into the event queue.
  if (osMessagePut(event_msg_queue, (uint32_t) event_handler, 0) != osOK)
    ++event_msg_queue_error_count;
}

