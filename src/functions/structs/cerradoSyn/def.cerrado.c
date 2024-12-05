
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end

CerradoSyn *new_CerradoSynStruct(const char *class){
  CerradoSyn *self = NULL;

  self->pid_father = getpid();

  self->process_list = (Process *)malloc(sizeof(Process));
  self->size_process = 0;

  self->name_class = class;
  self->class_list = NULL;
  
  self->memory = NULL;

  return self;
}

void free_CerradoSyn(CerradoSyn *self){
  if(self != NULL){

    if(self->process_list != NULL){
      for(int i = 0; i < self->size_process + 1; i++){//O tamanho do process_list sempre vai ser uma unidade maior que o size_process por motivos de seguraça;
        private_free_process(&self->process_list[i]);
      }
      free(self->process_list);
    }

    free(self);
  }
  return;
}




