#ifndef _EVENT_H
#define _EVENT_H

typedef void (*event_t)(void);

void event_init(void);
void event_loop(void);
void event_schedule(event_t event_handler);

#endif /* _EVENT_H */
