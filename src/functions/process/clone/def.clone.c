
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end



void private_clone_process(Process *process, int(*function)(void *arg), void *arg, int *flags){

  pid_t pid_process = clone(function, process->stack + process->size_stack - 1, flags?*flags:SIGCHLD, NULL);

  process->process = pid_process;
  printf("session:\nsize_stack: %d\nprocess:%d\n", process->size_stack, process->process);


}




