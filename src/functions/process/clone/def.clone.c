
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end

int private_CerradoSync_callback_config(void *arg){

  CerradoSync_CallbackProcess *struct_arg = (CerradoSync_CallbackProcess *)arg;

  CerradoSync_MemoryShared *memory_argument_0 = (CerradoSync_MemoryShared *)struct_arg->memory;
  private_CerradoSync_memory_data_attach(memory_argument_0);

  CerradoSync_pull_memory(memory_argument_0->memory_shared);

  int(*function_callback)(CerradoSync_MemoryShared *memory, CerradoSync_ArgumentsCallback *arguments);
  function_callback = struct_arg->function_callback;
  int var_return = function_callback(memory_argument_0, struct_arg->args);

  private_CerradoSync_close_memory(memory_argument_0);

  return var_return;
}

int private_CerradoSync_clone_process(CerradoSync_Process *process, CerradoSync_CallbackProcess *callback, int *flags){

  pid_t pid_process = clone(private_CerradoSync_callback_config, process->stack + process->size_stack - 1, flags?*flags:SIGCHLD, callback);

  if(pid_process == -1){
    return -1;
  }

  process->process = pid_process;

  return 1;

}




