#include <interrupt_handler.h>

#include <clic3_memory.h>

#include <pthread.h>
#include <signal.h>

pthread_mutex_t interrupt_handler_interrupted_mutex;
pthread_mutex_t interrupt_handler_not_interrupted_mutex;

unsigned char interrupt_handler_interrupted = (
  0x00
);

void** interrupt_handler_on_interrupt_functions;
void** interrupt_handler_on_interrupt_data;
unsigned int interrupt_handler_on_interrupt_functions_length;

void interrupt_handler_initialize() {
  interrupt_handler_on_interrupt_functions = (
    clic3_memory_allocate_raw(
      sizeof(
        void*
      ) *
      interrupt_handler_on_interrupt_functions_length
    )
  );

  interrupt_handler_on_interrupt_data = (
    clic3_memory_allocate_raw(
      sizeof(
        void*
      ) *
      interrupt_handler_on_interrupt_functions_length
    )
  );

  struct sigaction signal_action = {
    .sa_handler = (
      interrupt_handler_on_interrupt
    )
  };

  sigaction(
    SIGINT,
    &signal_action,
    0x00
  );
}

void interrupt_handler_initialize_thread_safe() {
  pthread_mutex_init(
    &interrupt_handler_interrupted_mutex,
    0x00
  );

  pthread_mutex_init(
    &interrupt_handler_not_interrupted_mutex,
    0x00
  );

  pthread_mutex_lock(
    &interrupt_handler_not_interrupted_mutex
  );

  interrupt_handler_on_interrupt_functions = (
    clic3_memory_allocate_raw(
      sizeof(
        void*
      ) *
      interrupt_handler_on_interrupt_functions_length
    )
  );

  interrupt_handler_on_interrupt_data = (
    clic3_memory_allocate_raw(
      sizeof(
        void*
      ) *
      interrupt_handler_on_interrupt_functions_length
    )
  );

  struct sigaction signal_action = {
    .sa_handler = (
      interrupt_handler_on_interrupt_thread_safe
    )
  };

  sigaction(
    SIGINT,
    &signal_action,
    0x00
  );
}

void interrupt_handler_interrupt_function_add(
  interrupt_handler_on_interrupt_function on_interrupt_function
) {
  interrupt_handler_interrupt_function_add_with_data(
    (
      (interrupt_handler_on_interrupt_function_with_data)
      on_interrupt_function
    ),
    0x00
  );
}

void interrupt_handler_interrupt_function_add_with_data(
  interrupt_handler_on_interrupt_function_with_data on_interrupt_function,
  void* on_interrupt_function_data
) {
  interrupt_handler_on_interrupt_functions_length = (
    interrupt_handler_on_interrupt_functions_length +
    0x01
  );

  clic3_memory_reallocate_raw(
    &interrupt_handler_on_interrupt_functions,
    (
      sizeof(
        void*
      ) *
      interrupt_handler_on_interrupt_functions_length
    )
  );

  interrupt_handler_on_interrupt_functions[
    interrupt_handler_on_interrupt_functions_length -
    0x01
  ] = (
    on_interrupt_function
  );

  clic3_memory_reallocate_raw(
    &interrupt_handler_on_interrupt_data,
    (
      sizeof(
        void*
      ) *
      interrupt_handler_on_interrupt_functions_length
    )
  );

  interrupt_handler_on_interrupt_data[
    interrupt_handler_on_interrupt_functions_length -
    0x01
  ] = (
    on_interrupt_function_data
  );
}

void interrupt_handler_interrupt_function_remove(
  interrupt_handler_on_interrupt_function on_interrupt_function
) {
  for (
    unsigned int interrupt_handler_on_interrupt_function_index = (
      0x00
    );
    (
      interrupt_handler_on_interrupt_function_index <
      interrupt_handler_on_interrupt_functions_length
    );
    ++interrupt_handler_on_interrupt_function_index
  ) {
    if (
      interrupt_handler_on_interrupt_functions[
        interrupt_handler_on_interrupt_function_index
      ] ==
      on_interrupt_function
    ) {
      for (
        unsigned int interrupt_handler_on_interrupt_function_shift_index = (
          interrupt_handler_on_interrupt_function_index
        );
        (
          interrupt_handler_on_interrupt_function_shift_index <
          (
            interrupt_handler_on_interrupt_functions_length -
            0x01
          )
        );
        ++interrupt_handler_on_interrupt_function_shift_index
      ) {
        interrupt_handler_on_interrupt_functions[
          interrupt_handler_on_interrupt_function_index
        ] = (
          interrupt_handler_on_interrupt_functions[
            interrupt_handler_on_interrupt_function_index +
            0x01
          ]
        );
      }

      interrupt_handler_on_interrupt_functions_length = (
        interrupt_handler_on_interrupt_functions_length -
        0x01
      );

      clic3_memory_reallocate_raw(
        &interrupt_handler_on_interrupt_functions,
        (
          sizeof(
            void*
          ) *
          interrupt_handler_on_interrupt_functions_length
        )
      );

      clic3_memory_reallocate_raw(
        &interrupt_handler_on_interrupt_data,
        (
          sizeof(
            void*
          ) *
          interrupt_handler_on_interrupt_functions_length
        )
      );

      interrupt_handler_interrupt_function_remove(
        on_interrupt_function
      );

      break;
    }
  }
}

void interrupt_handler_on_interrupt(
  int interrupt_code
) {
  interrupt_handler_interrupted = (
    interrupt_code
  );

  for (
    unsigned int interrupt_handler_on_interrupt_function_index = (
      0x00
    );
    (
      interrupt_handler_on_interrupt_function_index <
      interrupt_handler_on_interrupt_functions_length
    );
    ++interrupt_handler_on_interrupt_function_index
  ) {
    void* interrupt_handler_function = (
      interrupt_handler_on_interrupt_functions[
        interrupt_handler_on_interrupt_function_index
      ]
    );

    void* interrupt_handler_function_data = (
      interrupt_handler_on_interrupt_data[
        interrupt_handler_on_interrupt_function_index
      ]
    );

    if (
      interrupt_handler_function_data ==
      0x00
    ) {
      (
        (interrupt_handler_on_interrupt_function)
        interrupt_handler_function
      )(
        interrupt_code
      );
    } else {
      (
        (interrupt_handler_on_interrupt_function_with_data)
        interrupt_handler_function
      )(
        interrupt_code,
        interrupt_handler_function_data
      );
    }
  }
}

void interrupt_handler_on_interrupt_thread_safe(
  int interrupt_code
) {
  pthread_mutex_lock(
    &interrupt_handler_interrupted_mutex
  );

  pthread_mutex_unlock(
    &interrupt_handler_not_interrupted_mutex
  );

  interrupt_handler_on_interrupt(
    interrupt_code
  );

  pthread_mutex_unlock(
    &interrupt_handler_interrupted_mutex
  );
}

void interrupt_handler_destroy() {
  clic3_memory_free_raw(
    interrupt_handler_on_interrupt_functions
  );

  clic3_memory_free_raw(
    interrupt_handler_on_interrupt_data
  );
}

void interrupt_handler_destroy_thread_safe() {
  interrupt_handler_destroy();

  pthread_mutex_destroy(
    &interrupt_handler_interrupted_mutex
  );

  pthread_mutex_destroy(
    &interrupt_handler_not_interrupted_mutex
  );
}
