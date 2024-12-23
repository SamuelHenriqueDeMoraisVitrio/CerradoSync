
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end


int add_argument(CerradoSync_CallbackProcess *callback_self, CerradoSync_ArgumentCallback *add_arg){

  if(!callback_self || !add_arg || !callback_self->args){
    return -1;
  }

  CerradoSync_ArgumentsCallback *self = callback_self->args;

  self->size_arguments++;

  self->arguments = (CerradoSync_ArgumentCallback **)realloc(self->arguments, sizeof(CerradoSync_ArgumentCallback *) * (self->size_arguments + 1));
  if(!self->arguments){
    self->size_arguments--;
    return -2;
  }

  self->arguments[self->size_arguments - 1] = (CerradoSync_ArgumentCallback *)malloc(sizeof(*add_arg) + 1);
  if(!self->arguments[self->size_arguments - 1]){
    self->size_arguments--;
    return -3;
  }
  
  self->arguments[self->size_arguments - 1] = add_arg;

  return 1;
}


CerradoSync_ArgumentCallback *new_argument(const char *name_argument, void *arg, size_t arg_size){

  if(!name_argument || !arg){
    return NULL;
  }

  CerradoSync_ArgumentCallback *self = (CerradoSync_ArgumentCallback *)malloc(sizeof(CerradoSync_ArgumentCallback));
  if(!private_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->name_argument = (const char *)malloc(strlen(name_argument) + 1);
  if(!private_free_interrupted((char *)self->name_argument, (void *[]){self, NULL}, 2)){
    return NULL;
  }

  strcpy((char *)self->name_argument, name_argument);

  self->arg = (void *)malloc(arg_size + 1);
  if(!private_free_interrupted(self->arg, (void *[]){(char *)self->name_argument, self}, 2)){
    return NULL;
  }

  memcpy(self->arg, arg, arg_size + 1);

  return self;

}

void private_free_argument(CerradoSync_ArgumentCallback *self){
  if(self != NULL){
    if(self->arg != NULL){
      free(self->arg);
    }
    if(self->name_argument != NULL){
      free((char *)self->name_argument);
    }
    free(self);
  }
}

CerradoSync_ArgumentsCallback *private_new_ArgumentsCallback(){
  CerradoSync_ArgumentsCallback *self = (CerradoSync_ArgumentsCallback *)malloc(sizeof(CerradoSync_ArgumentsCallback) + 1);
  if(!private_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->arguments = (CerradoSync_ArgumentCallback **)malloc(sizeof(CerradoSync_ArgumentCallback *) + 1);
  if(!private_free_interrupted(self->arguments, (void *[]){self}, 1)){
    return NULL;
  }

  self->arguments[0] = (CerradoSync_ArgumentCallback *)malloc(0);
  if(!private_free_interrupted(self->arguments[0], (void *[]){self->arguments, self}, 2)){
    return NULL;
  }

  self->size_arguments = 0;

  return self;
}

void private_free_ArgumentsCallback(CerradoSync_ArgumentsCallback *self){
  if(self){
  
    if(self->arguments){

      for(int i=0; i < self->size_arguments + 1; i++){
        if(self->arguments[i] != NULL){
          CerradoSync_ArgumentCallback *argument = self->arguments[i];
          private_free_argument(argument);
        }
      }

      free(self->arguments);
    }

    free(self);
  }
}

CerradoSync_CallbackProcess *new_CallbackProcess(CerradoSyn *process_father, int (*function)(CerradoSync_MemoryShared *memory, CerradoSync_ArgumentsCallback *arguments)){

  if(!function || !process_father){
    return NULL;
  }

  CerradoSync_CallbackProcess *self = (CerradoSync_CallbackProcess *)malloc(sizeof(CerradoSync_CallbackProcess));
  if(!private_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->args = private_new_ArgumentsCallback();

  self->function_callback = function;

  self->memory = process_father->memory;

  return self;
}

void free_callback(CerradoSync_CallbackProcess *self){
  if(self != NULL){

    private_free_ArgumentsCallback(self->args);
    
    free(self);
  }
}



