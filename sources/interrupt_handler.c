#include "interrupt_handler.h"

#include <pthread.h>
#include <signal.h>
#include <stdlib.h>

pthread_mutex_t interrupt_handler_interrupted_mutex;
unsigned char interrupt_handler_interrupted = 0;

interrupt_handler_on_interrupt_function* interrupt_handler_on_interrupt_functions;
unsigned int interrupt_handler_on_interrupt_functions_length;

void interrupt_handler_initialize() {
  interrupt_handler_on_interrupt_functions = malloc(
    sizeof(interrupt_handler_on_interrupt_function) *
    interrupt_handler_on_interrupt_functions_length
  );

  struct sigaction signal_action;
  signal_action.sa_handler = interrupt_handler_on_interrupt;
  sigaction(SIGINT, &signal_action, (void*)0);
}

void interrupt_handler_initialize_thread_safe() {
  pthread_mutex_init(
    &interrupt_handler_interrupted_mutex,
    (void*)0
  );

  interrupt_handler_on_interrupt_functions = malloc(
    sizeof(interrupt_handler_on_interrupt_function) *
    interrupt_handler_on_interrupt_functions_length
  );

  struct sigaction signal_action;
  signal_action.sa_handler = interrupt_handler_on_interrupt_thread_safe;
  sigaction(SIGINT, &signal_action, (void*)0);
}

void interrupt_handler_interrupt_function_add(
  interrupt_handler_on_interrupt_function on_interrupt_function
) {
  interrupt_handler_on_interrupt_functions_length = (
    interrupt_handler_on_interrupt_functions_length +
    1
  );

  interrupt_handler_on_interrupt_functions = realloc(
    interrupt_handler_on_interrupt_functions,
    sizeof(interrupt_handler_on_interrupt_function) * interrupt_handler_on_interrupt_functions_length
  );

  interrupt_handler_on_interrupt_functions[
    interrupt_handler_on_interrupt_functions_length -
    1
  ] = on_interrupt_function;
}

void interrupt_handler_interrupt_function_remove(
  interrupt_handler_on_interrupt_function on_interrupt_function
) {
  for (
    unsigned int interrupt_handler_on_interrupt_function_index = 0;
    interrupt_handler_on_interrupt_function_index < interrupt_handler_on_interrupt_functions_length;
    ++interrupt_handler_on_interrupt_function_index
  ) {
    if (
      interrupt_handler_on_interrupt_functions[
        interrupt_handler_on_interrupt_function_index
      ] == on_interrupt_function
    ) {
      for (
        unsigned int interrupt_handler_on_interrupt_function_shift_index = interrupt_handler_on_interrupt_function_index;
        interrupt_handler_on_interrupt_function_shift_index < interrupt_handler_on_interrupt_functions_length - 1;
        ++interrupt_handler_on_interrupt_function_shift_index
      ) {
        interrupt_handler_on_interrupt_functions[
          interrupt_handler_on_interrupt_function_index
        ] = interrupt_handler_on_interrupt_functions[
          interrupt_handler_on_interrupt_function_index + 1
        ];
      }

      interrupt_handler_on_interrupt_functions_length = (
        interrupt_handler_on_interrupt_functions_length - 1
      );
      
      interrupt_handler_on_interrupt_functions = realloc(
        interrupt_handler_on_interrupt_functions,
        sizeof(interrupt_handler_on_interrupt_function) * interrupt_handler_on_interrupt_functions_length
      );
      
      interrupt_handler_interrupt_function_remove(on_interrupt_function);

      break;
    }
  }
}

void interrupt_handler_on_interrupt(int interrupt_code) {
  interrupt_handler_interrupted = interrupt_code;

  for (
    unsigned int interrupt_handler_on_interrupt_function_index = 0;
    interrupt_handler_on_interrupt_function_index < interrupt_handler_on_interrupt_functions_length;
    ++interrupt_handler_on_interrupt_function_index
  ) {
    interrupt_handler_on_interrupt_functions[
      interrupt_handler_on_interrupt_function_index
    ](interrupt_code);
  }
}

void interrupt_handler_on_interrupt_thread_safe(int interrupt_code) {
  pthread_mutex_lock(&interrupt_handler_interrupted_mutex);
  interrupt_handler_on_interrupt(interrupt_code);
  pthread_mutex_unlock(&interrupt_handler_interrupted_mutex);
}

void interrupt_handler_destroy() {
  free(
    interrupt_handler_on_interrupt_functions
  );
}

void interrupt_handler_destroy_thread_safe() {
  interrupt_handler_destroy();

  pthread_mutex_destroy(&interrupt_handler_interrupted_mutex);
}
