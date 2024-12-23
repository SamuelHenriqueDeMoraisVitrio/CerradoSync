# CerradoSync

The `CerradoSync` library is designed for shared memory management and process synchronization in Linux systems. It provides abstractions for handling shared memory, traffic signals, callbacks, and process creation, simplifying the development of parallel systems.

---

## Table of Contents

1. [Installation](#installation)  
2. [Core Structures and Functions](#core-structures-and-functions)  
3. [Basic Example](#basic-example)  
4. [Advanced Example](#advanced-example)  
5. [Function Reference](#function-reference)  
6. [Notes and Best Practices](#notes-and-best-practices)

---

## Inclusion

To use `CerradoSync`, ensure you are working on a Linux environment with support for shared memory and processes. A modern C compiler with pthread and real-time libraries is required.

1. Include the header file in your project:
   ```c
   #include "CerradoSync.h"
   ```

2. Compile your code with the necessary flags:
   ```bash
   gcc -o main main.c
   ```

---

## Core Structures and Functions

### **Structures**
- **`CerradoSync`**: The main process class structure.  
- **`CerradoSync_MemoryShared`**: Structure for managing shared memory.  
- **`CerradoSync_CallbackProcess`**: Represents a child process with callback functionality.  
- **`CerradoSync_ArgumentCallback`**: Defines arguments to be passed to callbacks.

### **Key Functions**
- **Creation and Cleanup**:
  - `new_CerradoSyncStruct(name, size)`: Creates the main process structure.
  - `free_CerradoSync(struct)`: Releases all allocated memory for a process class.

- **Shared Memory Management**:
  - `CerradoSync_pull_memory(memory)`: Copies global memory into the static memory buffer.
  - `CerradoSync_push_memory(memory)`: Copies static memory into the global memory buffer.
  - `CerradoSync_config_memory(memory, value, size)`: Configures the static memory buffer.

- **Traffic Signals**:
  - `CerradoSync_create_pointer_traffic(...)`: Creates a traffic signal point.
  - `CerradoSync_signal_traffic(...)`: Sets the traffic signal to `RED_TRAFFIC` or `GREEN_TRAFFIC`.
  - `CerradoSync_wait_traffic(...)`: Waits for a traffic signal to reach a specific state.

- **Processes and Callbacks**:
  - `CerradoSync_new_CallbackProcess(...)`: Creates a callback for a child process.
  - `CerradoSync_create_process(...)`: Spawns a new child process.
  - `CerradoSync_new_argument(...)`: Creates an argument to pass to callbacks.
  - `CerradoSync_add_argument(...)`: Attaches an argument to a callback.

---

## Basic Example

```c
#include "CerradoSync.h"

int main() {
    // Create the main structure
    CerradoSync *sync = new_CerradoSyncStruct("main", _DEFAULT_MAX_SIZE_MEMORY_TRAFFIC_);
    CerradoSync_MemoryShared *memory = sync->memory;

    // Set an initial value in shared memory
    int initialValue = 100;
    CerradoSync_config_memory(memory, &initialValue, sizeof(int));
    CerradoSync_push_memory(memory);

    printf("Initial shared memory value: %d\n", *(int *)CerradoSync_getMemoryValue(memory));

    // Create a traffic signal
    CerradoSync_create_pointer_traffic(sync, "exampleTraffic", RED_TRAFFIC);
    CerradoSync_signal_traffic(memory, "exampleTraffic", GREEN_TRAFFIC);

    // Update shared memory with a new value
    int newValue = 200;
    CerradoSync_config_memory(memory, &newValue, sizeof(int));
    CerradoSync_push_memory(memory);

    printf("Updated shared memory value: %d\n", *(int *)CerradoSync_getMemoryValue(memory));

    // Cleanup
    free_CerradoSync(sync);

    return 0;
}
```

---

## Advanced Example

```c
#include "CerradoSync.h"

// Callback function
int callback_function(CerradoSync_MemoryShared *memory, CerradoSync_ArgumentsCallback *args) {
    const char *receivedName = (const char *)args->arguments[0]->arg;

    printf("Callback executed with name: %s\n", receivedName);

    // Update static memory with a new value
    int newValue = 999;
    CerradoSync_config_memory(memory, &newValue, sizeof(int));
    CerradoSync_push_memory(memory);

    return 0;
}

int main() {
    CerradoSync *sync = new_CerradoSyncStruct("main", _DEFAULT_MAX_SIZE_MEMORY_TRAFFIC_);
    CerradoSync_MemoryShared *memory = sync->memory;

    CerradoSync_pull_memory(memory);

    // Create a callback process
    CerradoSync_CallbackProcess *callback = CerradoSync_new_CallbackProcess(sync, callback_function);

    const char *name = "John Doe";
    CerradoSync_ArgumentCallback *arg = CerradoSync_new_argument("name", (void *)name, strlen(name));
    CerradoSync_add_argument(callback, arg);

    // Spawn the child process
    CerradoSync_create_process(sync, callback, NULL);

    // Wait for the traffic signal
    CerradoSync_wait_traffic(memory, "exampleTraffic", GREEN_TRAFFIC);

    // Cleanup and finalize
    for (int i = 0; i < sync->size_process; i++) {
        int status;
        waitpid(sync->process_list[i]->process, &status, 0);
    }

    free_CerradoSync(sync);
    return 0;
}
```

## Complete Example:

```c

#include "CerradoSync.h"

int process_print_name(CerradoSync_MemoryShared *memory, CerradoSync_ArgumentsCallback *args){

  const char *name = (const char *)args->arguments[0]->arg;

  printf("\n\tname: %s\n", name);

  CerradoSync_signal_traffic(memory, "className", RED_TRAFFIC);//Sends a signal for traffic with name 'className' to be closed

  CerradoSync_pull_memory(memory);

  int *valorMemoryAnterior = (int *)CerradoSync_getMemoryValue(memory);

  printf("\n\tProcess son with memory: %d\n", *valorMemoryAnterior);

  char *meu_nomeCompleto = "Samuel Henrique De Morias Vitrio";
  CerradoSync_config_memory(memory, meu_nomeCompleto, strlen(meu_nomeCompleto) + 1); //Updates static memory with the specified value

  CerradoSync_push_memory(memory);

  CerradoSync_signal_traffic(memory, "className", GREEN_TRAFFIC); //Sends a signal for traffic with name 'className' to be opened

  printf("\n\tValor novo : %s\n", (const char *)memory->memory_shared->memory);

  return 0;
}

int main(){

  CerradoSync *main = new_CerradoSyncStruct("main", _DEFAULT_MAX_SIZE_MEMORY_TRAFFIC_); //create a class of process

  CerradoSync_MemoryShared *memory = main->memory; //The memory structure that contains global memory and static memory to work with

  CerradoSync_pull_memory(memory); //Update memory static
  int valorAtual = 450;
  CerradoSync_config_memory(memory, &valorAtual, sizeof(int)); //Assigns a value to static memory
  CerradoSync_push_memory(memory); //Update global memory with static memory

  printf("\n\tpai: %d\n", (int)*((int *)CerradoSync_getMemoryValue(memory))); //Get pointer of static memory value

  CerradoSync_create_pointer_traffic(main, "className", RED_TRAFFIC); //Creates an initially blocked traffic point

  CerradoSync_CallbackProcess *callback = CerradoSync_new_CallbackProcess(main, process_print_name);//Struct callback process

  const char *name = "Samuel Henrique";
  CerradoSync_ArgumentCallback *name_arg = CerradoSync_new_argument("teste", (void *)name, strlen(name));//A new argument for callback
  CerradoSync_add_argument(callback, name_arg);//Assign argument for callback

  CerradoSync_create_process(main, callback, NULL); //Create a process

  CerradoSync_wait_traffic(memory, "className", GREEN_TRAFFIC); //Wait pointer traffic 'className' stay open
  CerradoSync_pull_memory(memory); //Get the latest information from global memory and copy it to static memory

  printf("\n\tpai depois: %s\n", (const char *)CerradoSync_getMemoryValue(memory));

  printf("\n\tHello Word!\n");
  for (int i = 0; i < main->size_process; i++) {
    pid_t temp_process = main->process_list[i]->process;
    int status;
    waitpid(temp_process, &status, 0);
    printf("\n\tstatus: %d of process: %d\n", WIFEXITED(status), i);
  }
  printf("\n\tBye Word\n");

  free_CerradoSync(main);//Releases all allocated memory of the created process class

  return 0;
}
```

---

## Function Reference

| **Function**                        | **Description**                                                                                  |
|-------------------------------------|--------------------------------------------------------------------------------------------------|
| `new_CerradoSyncStruct`             | Creates the main process structure.                                                             |
| `free_CerradoSync`                  | Releases all allocated memory for the process class.                                            |
| `CerradoSync_pull_memory`           | Copies global memory into the static memory buffer.                                             |
| `CerradoSync_push_memory`           | Copies static memory into the global memory buffer.                                             |
| `CerradoSync_config_memory`         | Updates the static memory buffer with a given value.                                            |
| `CerradoSync_create_pointer_traffic`| Creates a named traffic signal with an initial state.                                           |
| `CerradoSync_signal_traffic`        | Updates the state of a traffic signal to `RED_TRAFFIC` or `GREEN_TRAFFIC`.                      |
| `CerradoSync_wait_traffic`          | Waits for a traffic signal to change to the specified state.                                    |
| `CerradoSync_new_CallbackProcess`   | Creates a callback process.                                                                     |
| `CerradoSync_new_argument`          | Creates an argument to pass to a callback.                                                      |
| `CerradoSync_add_argument`          | Associates an argument with a callback.                                                        |
| `CerradoSync_create_process`        | Spawns a new child process linked to a callback.                                                |

---

## Notes and Best Practices

- **Synchronization**: Always use traffic signals (`RED_TRAFFIC`, `GREEN_TRAFFIC`) to ensure safe inter-process communication.  
- **Memory Management**: Use `CerradoSync_pull_memory` and `CerradoSync_push_memory` carefully to avoid inconsistent data.  
- **Resource Cleanup**: Call `free_CerradoSync` at the end of your program to prevent memory leaks.  
- **Thread Safety**: The library relies on thread-safe operations but requires proper usage in multithreaded environments.

