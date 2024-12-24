
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end




int CerradoSync_wait_class_process_ended(CerradoSync *self){
  for (int i = 0; i < self->size_process; i++) {
    CerradoSync_Process *process = self->process_list[i];
    if(process->is_a_thread){
      continue;
    }
    pid_t retorno = waitpid(process->process, &process->status_process, 0);
    if(retorno == -1){
      return CERRADOSYNC_ERROR_A_GET_STATUS;
    }
  }

  return 1;
}

int CerradoSync_class_process_ended(CerradoSync *self){
  
  for (int i = 0; i < self->size_process; i++) {
    CerradoSync_Process *process = self->process_list[i];
    if(process->is_a_thread){
      continue;
    }
    pid_t result = waitpid(process->process, &process->status_process, WNOHANG);
    if(result == 0){
      return CERRADOSYNC_PROCESS_WORKING;
    }
    if(result == -1){
      return CERRADOSYNC_ERROR_A_GET_STATUS;
    }
  }

  return 1;
}

int CerradoSync_wait_process_ended(CerradoSync *self, int index_process){

  if(index_process < 0 || index_process >= self->size_process){
    return CERRADOSYNC_PROCESS_NOT_EXIST;
  }

  CerradoSync_Process *process = self->process_list[index_process];
  if(process->is_a_thread){
    return CERRADOSYNC_PROCESS_NOT_EXIST;
  }
  pid_t result = waitpid(process->process, &process->status_process, 0);
  if(result > 0){
    if(WIFEXITED(process->status_process)){
      return WEXITSTATUS(process->status_process);
    }
    return CERRADOSYNC_PROCESS_FINISHED_IN_ERROR;
  }

  return CERRADOSYNC_ERROR_A_GET_STATUS;
  
}

int CerradoSync_process_ended(CerradoSync *self, int index_process){
  
  if(!self->process_list[index_process]){
    return CERRADOSYNC_PROCESS_NOT_EXIST;
  }

  CerradoSync_Process *process = self->process_list[index_process];
  if(process->is_a_thread){
    return CERRADOSYNC_PROCESS_NOT_EXIST;
  }
  pid_t result = waitpid(process->process, &process->status_process, WNOHANG);
  if(result == 0){
    return CERRADOSYNC_PROCESS_WORKING;
  }
  if(result > 0){
    if(WIFEXITED(process->status_process)){
      return WEXITSTATUS(process->status_process);
    }
    return CERRADOSYNC_PROCESS_FINISHED_IN_ERROR;
  }

  return CERRADOSYNC_ERROR_A_GET_STATUS;
}


