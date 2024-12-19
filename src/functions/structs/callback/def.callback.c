
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end


int add_argument(CallbackProcess *callback_self, ArgumentCallback *add_arg){

  if(!callback_self || !add_arg || !callback_self->args){
    return -1;
  }

  ArgumentsCallback *self = callback_self->args;

  self->size_arguments++;

  self->arguments = (ArgumentCallback **)realloc(self->arguments, sizeof(ArgumentCallback *) * (self->size_arguments + 1));
  if(!self->arguments){
    self->size_arguments--;
    return -2;
  }

  self->arguments[self->size_arguments - 1] = (ArgumentCallback *)malloc(sizeof(*add_arg) + 1);
  if(!self->arguments[self->size_arguments - 1]){
    self->size_arguments--;
    return -3;
  }
  
  self->arguments[self->size_arguments - 1] = add_arg;

  return 1;
}


ArgumentCallback *new_argument(const char *name_argument, void *arg, size_t arg_size){

  if(!name_argument || !arg){
    return NULL;
  }

  ArgumentCallback *self = (ArgumentCallback *)malloc(sizeof(ArgumentCallback));
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

void private_free_argument(ArgumentCallback *self){
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

ArgumentsCallback *private_new_ArgumentsCallback(){
  ArgumentsCallback *self = (ArgumentsCallback *)malloc(sizeof(ArgumentsCallback) + 1);
  if(!private_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->arguments = (ArgumentCallback **)malloc(sizeof(ArgumentCallback *) + 1);
  if(!private_free_interrupted(self->arguments, (void *[]){self}, 1)){
    return NULL;
  }

  self->arguments[0] = (ArgumentCallback *)malloc(0);
  if(!private_free_interrupted(self->arguments[0], (void *[]){self->arguments, self}, 2)){
    return NULL;
  }

  self->size_arguments = 0;

  return self;
}

void private_free_ArgumentsCallback(ArgumentsCallback *self){
  if(self){
  
    if(self->arguments){

      for(int i=0; i < self->size_arguments + 1; i++){
        if(self->arguments[i] != NULL){
          ArgumentCallback *argument = self->arguments[i];
          private_free_argument(argument);
        }
      }

      free(self->arguments);
    }

    free(self);
  }
}

CallbackProcess *new_CallbackProcess(CerradoSyn *process_father, int (*function)(MemoryShared *memory, ArgumentsCallback *arguments)){

  if(!function || !process_father){
    return NULL;
  }

  CallbackProcess *self = (CallbackProcess *)malloc(sizeof(CallbackProcess));
  if(!private_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->args = private_new_ArgumentsCallback();

  self->function_callback = function;

  self->memory = process_father->memory;

  return self;
}

void free_callback(CallbackProcess *self){
  if(self != NULL){

    private_free_ArgumentsCallback(self->args);
    
    free(self);
  }
}



