
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end

CerradoSync *new_CerradoSyncStruct(const char *class_name, size_t size_max_memory_traffic){

  static bool primary_run = true;
  if(primary_run){
    setbuf(stdout, NULL);
  }
  primary_run = false;

  CerradoSync *self = malloc(sizeof(CerradoSync));
  if(!private_CerradoSync_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->pid_father = getpid();

  self->process_list = (CerradoSync_Process **)malloc(sizeof(CerradoSync_Process *) * 2);
  if(!private_CerradoSync_free_interrupted(self->process_list, (void *[]){self}, 1)){
    return NULL;
  }

  self->size_process = 0;

  self->name_class = (const char *)malloc(strlen(class_name) + 1);
  if(!private_CerradoSync_free_interrupted((char *)self->name_class, (void *[]){self->process_list, self}, 2)){
    return NULL;
  }

  strcpy((char *)self->name_class, class_name);

  self->key = private_CerradoSync_creat_key(self->name_class);
  if(!self->key){
    private_CerradoSync_free_interrupted(NULL, (void *[]){(char *)self->name_class, self->process_list, self}, 3);
    return NULL;
  }

  self->class_list = NULL;

  self->callbacks = (CerradoSync_CallbackProcess **)malloc(sizeof(CerradoSync_CallbackProcess *));
  if(!private_CerradoSync_free_interrupted(self->callbacks, (void *[]){(char *)self->name_class, self->process_list, self}, 3)){
    return NULL;
  }

  self->size_list_callbacks = 0;

  self->memory = private_CerradoSync_new_MemorySahred_struct(self->name_class, size_max_memory_traffic);
  if(!private_CerradoSync_free_interrupted((CerradoSync_MemoryShared *)self->memory, (void *[]){(char *)self->name_class, self->process_list, self}, 3)){
    return NULL;
  }

  return self;
}

void free_CerradoSync(CerradoSync *self){

  if(self != NULL){

    if(self->process_list != NULL){
      for(int i = 0; i < self->size_process + 1; i++){//O tamanho do process_list sempre vai ser uma unidade maior que o size_process por motivos de seguraça;
        if(self->process_list[i] != NULL){
          CerradoSync_Process *process_temp = self->process_list[i];
          private_CerradoSync_free_process(process_temp);
        }
      }
      free(self->process_list);
    }

    if(self->name_class != NULL){
      free((char *)self->name_class);
    }

    if(self->callbacks){
      for(int i = 0; i < self->size_list_callbacks + 1; i++){
        if(self->callbacks[i]){
          private_CerradoSync_free_callback(self->callbacks[i]);
        }
      }
      free(self->callbacks);
    }

    if(self->memory != NULL){
      private_CerradoSync_delet_memory(self->memory);
    }

    free(self);
  }

  return;
}




