
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end



Process *private_new_process(pid_t *process){

  Process *self;

  self->process = process;
  self->stack = (char*)malloc(1024 * 1024);

  return self;
}


void private_free_process(Process *self){
  if(!(self != NULL)){
    return;
  }
  if(self->stack != NULL){
    free(self->stack);
    return;
  }
  return;
}



