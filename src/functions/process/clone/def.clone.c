
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
#include <stdio.h>
//silver_chain_scope_end

int callback_config(void *arg){

  CallbackProcess *struct_arg = (CallbackProcess *)arg;

  MemoryShared *memory_argument_0 = (MemoryShared *)struct_arg->memory;
  private_memory_data_attach(memory_argument_0);

  pull_memory(memory_argument_0->memory_shared);

  int(*function_callback)(MemoryShared *memory, ArgumentsCallback *arguments);
  function_callback = struct_arg->function_callback;
  int var_return = function_callback(memory_argument_0, struct_arg->args);

  private_close_memory(memory_argument_0);

  return var_return;
}

int private_clone_process(Process *process, CallbackProcess *callback, int *flags){

  pid_t pid_process = clone(callback_config, process->stack + process->size_stack - 1, flags?*flags:SIGCHLD, callback);

  if(pid_process == -1){
    return -1;
  }

  process->process = pid_process;

  return 1;

}




