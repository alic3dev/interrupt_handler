#ifndef __interrupt_handler_h
#define __interrupt_handler_h

#include <pthread.h>

typedef void (*interrupt_handler_on_interrupt_function)(int);

extern pthread_mutex_t interrupt_handler_interrupted_mutex;
extern pthread_mutex_t interrupt_handler_not_interrupted_mutex;

extern unsigned char interrupt_handler_interrupted;

extern interrupt_handler_on_interrupt_function* interrupt_handler_on_interrupt_functions;
extern unsigned int interrupt_handler_on_interrupt_functions_length;

void interrupt_handler_initialize();
void interrupt_handler_initialize_thread_safe();

void interrupt_handler_interrupt_function_add(
  interrupt_handler_on_interrupt_function
);

void interrupt_handler_interrupt_function_remove(
  interrupt_handler_on_interrupt_function
);

void interrupt_handler_on_interrupt(
  int
);

void interrupt_handler_on_interrupt_thread_safe(
  int
);

void interrupt_handler_destroy();
void interrupt_handler_destroy_thread_safe();

#endif
