# interrupt_handler

a_utility_library_for_catching_interrupts

## usage

### initialization

```c
interrupt_handler_initialize();
```

#### thread_safe

```c
interrupt_handler_initialize_thread_safe();
```

- will lock `interrupt_handler_not_interrupted_mutex` during initialization and unlock after locking `interrupt_handler_interrupted_mutex` during interruption
- will lock `interrupt_handler_interrupted_mutex` before calling functions added with `interrupt_handler_interrupt_function_add` then unlock the mutex once all functions have been called

### callbacks

#### add

```c
interrupt_handler_interrupt_function_add(
  on_interrupt_function
);
```

##### with_data

```c
interrupt_handler_interrupt_function_add_with_data(
  on_interrupt_function,
  on_interrupt_function_data
);
```

do not pass `(void*) 0` values as the data parameter or your function will be considered and cast as `interrupt_handler_on_interrupt_function` rather than `interrupt_handler_on_interrupt_function_with_data`
if there is a possibility that the the data you are passing may be `(void*) 0` then you should make use of a conditional statement and use the `interrupt_handler_interrupt_function_add` function instead

##### with_data_conditionally

```c
void on_interrupt_function(
  int code_interrupt
) {
  on_interrupt_function_with_data(
    code_interrupt,
    (void*) 0
  );
}

void on_interrupt_function_with_data(
  int code_interrupt,
  void* data
) {
  if (
    data == (void*) 0
  ) {
    // will only branch this direction from `interrupt_handler_interrupt_function_add` with `on_interrupt_function`

    // functionality implementation
  } else {
    // will only branch this direction from `interrupt_handler_interrupt_function_add_with_data` with `on_interrupt_function_with_data`
    struct some_structure* some_structure = (
      data
    );

    // functionality implementation
  }
}

int main() {
  struct some_structure* some_structure = (
    some_function_call()
  );

  if (
    some_structure == (void*) 0
  ) {
    interrupt_handler_interrupt_function_add(
      on_interrupt_function
    );
  } else {
    interrupt_handler_interrupt_function_add_with_data(
      on_interrupt_function_with_data,
      some_structure
    );
  }

  // functionality implementation
}
```

#### remove

```c
interrupt_handler_interrupt_function_remove(
  on_interrupt_function
);
```

### status

```c
if (
  interrupt_handler_interrupted == 0
) {
  // not_interrupted_yet
} else {
  // has_been_interrupted
  // `interrupt_handler_interrupted`:contains_interrupt_code
}
```

## development

### `make` targets

```zsh
# build library
make interrupt_handler
## clean library
make clean
```

### `make` flags

- `debug=1`:adds->{`debugging_symbols`}:disables->{`optimizations`};

## copyright|copyleft

> © copyright:|copyleft:alic3dev[2025|2026]|all_rights_reserved:all_lefts_reserved
