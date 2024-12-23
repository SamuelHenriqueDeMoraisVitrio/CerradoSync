
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end




int CerradoSync_create_process(CerradoSync *main_process, CerradoSync_CallbackProcess *callback, int *flags) {
  if (getpid() != main_process->pid_father) {
      return -1;
  }

  if(!main_process || !callback){
    return -1;
  }

  CerradoSync_Process *new_process = private_CerradoSync_new_process(_SIZE_STACK_PROCESS_1MB_);
  if(!new_process){
    return -1;
  }

  private_CerradoSync_clone_process(new_process, callback, flags);
  if(new_process->process == -1){
    private_CerradoSync_free_process(new_process);
    return -1;
  }

  main_process->size_process++;

  main_process->process_list = (CerradoSync_Process **)realloc(main_process->process_list, (main_process->size_process + 1) * sizeof(CerradoSync_Process *));
  if(!main_process->process_list){
    return -1;
  }

  main_process->process_list[main_process->size_process - 1] = new_process;

  return 1;
}




