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
interrupt_handler_interrupt_function_add(on_interrupt_function);
```

#### remove

```c
interrupt_handler_interrupt_function_remove(on_interrupt_function);
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

> ©️ copyright:|copyleft:alic3dev[2025]|all_rights_reserved:all_lefts_reserved
