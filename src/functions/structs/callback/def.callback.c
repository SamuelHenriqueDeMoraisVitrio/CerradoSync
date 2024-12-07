
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
#include <cstdlib>
#include <stdlib.h>
//silver_chain_scope_end

ArgumentsCallback *add_argument(const char *name_argument, void *arg){

  if(!name_argument || !arg){
    return NULL;
  }

  ArgumentsCallback *self = (ArgumentsCallback *)malloc(sizeof(ArgumentsCallback));

  char name[strlen(name_argument) + 1];
  strcpy(name, name_argument);
  self->name_argument = name_argument;
  self->arg = (void *)malloc(0);
  self->arg = (void *)realloc(arg, sizeof(arg));

  return self;

}

void private_free_argument(ArgumentsCallback *self){
  if(self != NULL){
    if(self->arg != NULL){
      free(self->arg);
    }
    free(self);
  }
}

CallbackProcess *new_CallbackProcess(int (*function)(ArgumentsCallback *args), ArgumentsCallback *prymari_arg){

  if(!function || !prymari_arg){
    return NULL;
  }

  CallbackProcess *self = (CallbackProcess *)malloc(sizeof(CallbackProcess));

  self->function_callback = function;
  self->arguments = (ArgumentsCallback **)malloc(sizeof(ArgumentsCallback *) * 2);
  self->size_arguments = 1;
  self->arguments[0] = (ArgumentsCallback *)malloc(sizeof(*prymari_arg));
  self->arguments[0] = prymari_arg;

  return self;

}

void private_free_arguments(CallbackProcess *self){
  if(self != NULL){

  }
}



