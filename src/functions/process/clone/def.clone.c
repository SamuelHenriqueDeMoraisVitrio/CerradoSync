
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end

int callback_config(void *arg){


  CallbackProcess *struct_arg = (CallbackProcess *)arg;

  MemoryShared *memory_argument_0 = (MemoryShared *)struct_arg->memory;
  memory_argument_0->memory_shared->memoryShared = private_memory_data_attach(memory_argument_0);

  pull_memory(memory_argument_0->memory_shared);

  int(*function_callback)(MemoryShared *memory, ArgumentsCallback *arguments);
  function_callback = struct_arg->function_callback;
  int var_return = function_callback(memory_argument_0, struct_arg->args);

  private_close_memory(memory_argument_0);

  return var_return;
}

void private_clone_process(Process *process, CallbackProcess *callback, int *flags){

  if(!process){
    perror("Error cloning process");
    exit(EXIT_FAILURE);
  }

  pid_t pid_process = clone(callback_config, process->stack + process->size_stack - 1, flags?*flags:SIGCHLD, callback);

  process->process = pid_process;


}




