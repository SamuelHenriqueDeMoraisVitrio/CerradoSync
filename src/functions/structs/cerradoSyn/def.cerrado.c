
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end

CerradoSyn *new_CerradoSynStruct(const char *class_name){
  CerradoSyn *self = malloc(sizeof(CerradoSyn));

  self->pid_father = getpid();

  self->process_list = (Process **)malloc(sizeof(Process *));
  self->size_process = 0;

  char name[strlen(class_name) + 1];
  strcpy(name, class_name);
  self->name_class = name;
  self->class_list = NULL;
  
  self->memory = NULL;

  return self;
}

void free_CerradoSyn(CerradoSyn *self){

  if(self != NULL){

    if(self->process_list != NULL){
      for(int i = 0; i < self->size_process; i++){//O tamanho do process_list sempre vai ser uma unidade maior que o size_process por motivos de seguraça;
        if(self->process_list[i] != NULL){
          Process *process_temp = self->process_list[i];
          private_free_process(process_temp);
        }
      }
      free(self->process_list);
    }

    free(self);
  }

  return;
}




