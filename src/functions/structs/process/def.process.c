
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end



Process *private_new_process(int size_stack){

  Process *self = malloc(sizeof(Process));
  if(!private_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->size_stack = size_stack;
  self->stack = malloc(self->size_stack);
  if(!private_free_interrupted(self->stack, (void *[]){self}, 1)){
    return NULL;
  }

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



