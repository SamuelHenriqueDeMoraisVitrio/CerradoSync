
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end

int callback_config(void *arg){

  setbuf(stdout, NULL);

  CallbackProcess *stuct_arg = (CallbackProcess *)arg;

  int(*function_callback)(ArgumentsCallback *arguments);
  function_callback = stuct_arg->function_callback;

  int var_return = function_callback(stuct_arg->args);

  //fflush(stdout);

  return var_return;
}

void private_clone_process(Process *process, CallbackProcess *callback, int *flags){

  pid_t pid_process = clone(callback_config, process->stack + process->size_stack - 1, flags?*flags:SIGCHLD, callback);

  process->process = pid_process;


}




