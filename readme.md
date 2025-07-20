# interrupt_handler

a_utility_library_for_catching_interrupts

## usage

### initialization

```c
interrupt_handler_initialize();
```

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
if (interrupt_handler_interrupted == 0) {
  // not_interrupted_yet
} else {
  // has_been_interrupted
  // `interrupt_handler_interrupted`:contains_interrupt_code
}
```

## build

```sh
make
```

## clean

```sh
make clean
```

## copyright|copyleft

> ©️ copyright:|copyleft:alic3dev[2025]|all_rights_reserved:all_lefts_reserved

