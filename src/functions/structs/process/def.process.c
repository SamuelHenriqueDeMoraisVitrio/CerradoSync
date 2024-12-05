
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end



Process *private_new_process(pid_t process, void *stack){

  Process *self = malloc(sizeof(Process));

  self->process = process;
  self->stack = stack;

  return self;
}


void private_free_process(Process *self){

  if(self != NULL){
  
    if(self->stack != NULL){
      free(self->stack);
    }
    free(self);

  }
  
  return;
}



