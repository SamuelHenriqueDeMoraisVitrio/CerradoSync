
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end




bool create_process(CerradoSyn *main_process, int (*function_process)(void *arg), void *arg, int *flags) {
  if (getpid() != main_process->pid_father) {
      return false;
  }

  void *stack = malloc(_SIZE_STACK_PROCESS_1MB_);

  pid_t process = clone(function_process, (char *)stack + _SIZE_STACK_PROCESS_1MB_, flags?*flags:SIGCHLD, NULL);
  if(process == -1){
    return false;
  }

  main_process->size_process++;

  Process *new_struct_process = private_new_process(process, stack);

  main_process->process_list = (Process *)realloc(main_process->process_list, (main_process->size_process + 1) * sizeof(Process));

  return true;
}




