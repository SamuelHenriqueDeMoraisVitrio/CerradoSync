# CerradoSyn Library Documentation

## Overview

**CerradoSyn** is a C library that provides an abstraction over the `clone` syscall for handling multiprocessing or shared threads. It allows safe shared memory management using semaphores, supporting both asynchronous and synchronous operations. This library is ideal for developers who need to build systems with precise control over shared memory and inter-process communication.

---

## Features

- **Process and Thread Management**: Leverage `clone` to create isolated or shared processes.
- **Shared Memory Control**: Manage shared memory segments securely with semaphore mechanisms.
- **Traffic Signal System**: Coordinate processes using a traffic signal system (`RED_TRAFFIC` and `GREEN_TRAFFIC`).
- **Asynchronous and Synchronous Modes**: Flexibility to use either asynchronous or synchronous workflows.

---

## Installation

To use CerradoSyn in your projects:

1. Include the library headers in your source code:
   ```c
   #include "imports/imports.def.h"
   ```
2. Link the library during compilation.

```bash
gcc -o my_program my_program.c -lCerradoSyn
```

---

## API Reference

### Structures

#### `CerradoSyn`
- Represents the main structure for handling processes and shared memory.
- **Constructor**: `new_CerradoSynStruct(const char *name, size_t max_memory_size)`

#### `MemoryShared`
- Manages shared memory between processes.

#### `CallbackProcess`
- Represents a callback function for processes.

### Functions

#### Process and Memory Management

- `int create_pointer_traffic(CerradoSyn *cerrado, const char *className, int initial_signal)`
  - Create a traffic signal associated with a memory class.

- `int create_process(CerradoSyn *cerrado, CallbackProcess *callback, void *user_data)`
  - Create a new process.

- `void config_memory(MemoryShared *memory, void *data, size_t size)`
  - Configure memory with new data.

- `void pull_memory(MemoryShared *memory)`
  - Load the current shared memory state.

- `void push_memory(MemoryShared *memory)`
  - Save changes to shared memory.

#### Callback Management

- `CallbackProcess* new_CallbackProcess(CerradoSyn *cerrado, int (*callback_function)(MemoryShared*, ArgumentsCallback*))`
  - Create a new callback process.

- `ArgumentCallback* new_argument(const char *name, void *data, size_t size)`
  - Create a new argument for a callback.

- `void add_argument(CallbackProcess *callback, ArgumentCallback *arg)`
  - Add an argument to a callback.

---

## Example Usage

```c
#include "imports/imports.def.h"

int process_print_name(MemoryShared *memory, ArgumentsCallback *args) {
    const char *name = (const char *)args->arguments[0]->arg;
    printf("\n\tname: %s\n", name);

    signal_traffic(memory, "className", RED_TRAFFIC);

    pull_memory(memory->memory_shared);
    int *previous_value = (int *)memory->memory_shared->memory;
    printf("\n\tProcess son with memory: %d\n", *previous_value);

    char *full_name = "Samuel Henrique De Morias Vitrio";
    config_memory(memory->memory_shared, full_name, strlen(full_name) + 1);
    push_memory(memory->memory_shared);

    signal_traffic(memory, "className", GREEN_TRAFFIC);
    printf("\n\tNew Value: %s\n", (const char *)memory->memory_shared->memory);

    return 0;
}

int main() {
    CerradoSyn *main = new_CerradoSynStruct("main", _DEFAULT_MAX_SIZE_MEMORY_TRAFFIC_);
    if (!main) return 1;

    MemoryShared *memory = main->memory;
    pull_memory(memory->memory_shared);
    int current_value = 450;
    config_memory(memory->memory_shared, &current_value, sizeof(int));
    push_memory(memory->memory_shared);

    printf("\n\tParent: %d\n", (int)*((int *)memory->memory_shared->memory));

    create_pointer_traffic(main, "className", RED_TRAFFIC);

    CallbackProcess *callback = new_CallbackProcess(main, process_print_name);

    const char *name = "Samuel Henrique";
    ArgumentCallback *name_arg = new_argument("name", (void *)name, strlen(name));
    add_argument(callback, name_arg);

    create_process(main, callback, NULL);
    free_callback(callback);

    wait_traffic(memory, "className", GREEN_TRAFFIC);
    pull_memory(memory->memory_shared);

    printf("\n\tParent after: %s\n", (const char *)memory->memory_shared->memory);

    printf("\n\tHello World!\n");
    for (int i = 0; i < main->size_process; i++) {
        pid_t temp_process = main->process_list[i]->process;
        int status;
        waitpid(temp_process, &status, 0);
        printf("\n\tStatus: %d of process: %d\n", WIFEXITED(status), i);
    }
    printf("\n\tBye World\n");

    free_CerradoSyn(main);
    return 0;
}
```

---

## License

This project is licensed under [MIT License](LICENSE).

## Contact

For questions, feature requests, or issues, please reach out at:
- **Email**: samuel.henrique@example.com
- **GitHub**: [GitHub Profile](https://github.com/samuelhenrique)

---

Happy coding with CerradoSyn!


