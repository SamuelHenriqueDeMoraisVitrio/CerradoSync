
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end


void add_argument(CallbackProcess *callback_self, ArgumentCallback *add_arg){

  if(!callback_self || !add_arg){
    return;
  }

  callback_self->args->size_arguments++;

  callback_self->args->arguments = (ArgumentCallback **)realloc(callback_self->args->arguments, callback_self->args->size_arguments + 1);
  if(callback_self->args->arguments == NULL){
    callback_self->args->size_arguments--;
    return;
  }

  callback_self->args->arguments[callback_self->args->size_arguments - 1] = (ArgumentCallback *)malloc(sizeof(*add_arg) + 1);
  if(callback_self->args->arguments == NULL){
    callback_self->args->size_arguments--;
    return;
  }
  
  callback_self->args->arguments[callback_self->args->size_arguments - 1] = add_arg;
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

  memcpy(self->arg, arg, arg_size);

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

CallbackProcess *new_CallbackProcess(CerradoSyn *process_father, int (*function)(ArgumentsCallback *arguments)){

  if(!function || !process_father){
    return NULL;
  }

  CallbackProcess *self = (CallbackProcess *)malloc(sizeof(CallbackProcess));
  if(!private_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->function_callback = function;

  self->args = (ArgumentsCallback *)malloc(sizeof(ArgumentsCallback) + 1);
  if(!private_free_interrupted(self->args, (void *[]){self}, 1)){
    return NULL;
  }

  self->args->arguments = (ArgumentCallback **)malloc(sizeof(ArgumentCallback *) * 2);
  if(!private_free_interrupted(self->args->arguments, (void *[]){self->args, self}, 2)){
    return NULL;
  }

  self->args->size_arguments = 1;

  self->args->arguments[0] = new_argument("memory", process_father->memory, sizeof(*process_father->memory));
  if(!private_free_interrupted(self->args->arguments[0], (void *[]){self->args->arguments, self->args, self}, 3)){
    return NULL;
  }

  return self;
}

void free_callback(CallbackProcess *self){
  if(self != NULL){

    if(self->args != NULL){

      if(self->args->arguments != NULL){

        for(int i=0; i < self->args->size_arguments + 1; i++){
          if(self->args->arguments[i] != NULL){
            ArgumentCallback *argument = self->args->arguments[i];
            private_free_argument(argument);
          }
        }
        
        free(self->args->arguments);
      }
      free(self->args);
    }
    free(self);
  }
}



