
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

  char name[strlen(name_argument) + 1];
  strcpy(name, name_argument);
  self->name_argument = name_argument;

  self->arg = (void *)malloc(arg_size + 1);
  if(!self->arg){
    free(self);
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
    free(self);
  }
}

CallbackProcess *new_CallbackProcess(int (*function)(ArgumentsCallback *arguments), ArgumentCallback *primary_arg){

  if(!function || !primary_arg){
    return NULL;
  }

  CallbackProcess *self = (CallbackProcess *)malloc(sizeof(CallbackProcess));

  self->function_callback = function;

  self->args = (ArgumentsCallback *)malloc(sizeof(ArgumentsCallback) + 1);

  self->args->arguments = (ArgumentCallback **)malloc(sizeof(ArgumentCallback *) * 2);
  self->args->size_arguments = 1;
  self->args->arguments[0] = (ArgumentCallback *)malloc(sizeof(*primary_arg));
  self->args->arguments[0] = primary_arg;

  return self;

}

void free_callback(CallbackProcess *self){
  if(self != NULL){

    if(self->args != NULL){

      if(self->args->arguments != NULL){

        for(int i=0; i < self->args->size_arguments; i++){
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



