
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end




bool create_process(CerradoSyn *main_process, CallbackProcess *callback, int *flags) {
  if (getpid() != main_process->pid_father) {
      return false;
  }

  if(!main_process || !callback){
    return false;
  }

  Process *new_process = private_new_process(_SIZE_STACK_PROCESS_1MB_);

  private_clone_process(new_process, callback, flags);
  if(new_process->process == -1){
    private_free_process(new_process);
    return false;
  }

  main_process->size_process++;

  main_process->process_list = (Process **)realloc(main_process->process_list, (main_process->size_process + 1) * sizeof(Process *));

  main_process->process_list[main_process->size_process - 1] = new_process;

  return true;
}




