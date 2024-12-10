
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end

int callback_config(void *arg){


  CallbackProcess *struct_arg = (CallbackProcess *)arg;

  MemoryShared *memory_argument_0 = (MemoryShared *)struct_arg->args->arguments[0]->arg;
  memory_argument_0->memory = private_memory_data_attach(memory_argument_0);

  int(*function_callback)(ArgumentsCallback *arguments);
  function_callback = struct_arg->function_callback;
  int var_return = function_callback(struct_arg->args);

  private_close_memory(memory_argument_0->memory);

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




