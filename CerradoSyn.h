#ifndef silverchain_imp
#define silverchain_imp

#define _GNU_SOURCE
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>






#endif

#ifndef silverchain_mac
#define silverchain_mac

//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



#define _SIZE_STACK_PROCESS_1MB_ 1024 * 1024




#endif

#ifndef silverchain_typesF
#define silverchain_typesF

//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



typedef struct Arguments_struct ArgumentsCallback;
typedef struct Argument_struct ArgumentCallback;

struct Arguments_struct{
  int size_arguments;
  ArgumentCallback **arguments;
};

struct Argument_struct{
  void *arg;
  const char *name_argument;
};





#endif

#ifndef silverchain_typesG
#define silverchain_typesG

//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end




typedef struct CallbackProcess_struct CallbackProcess;

struct CallbackProcess_struct{
  int(*function_callback)(ArgumentsCallback *arguments);
  ArgumentsCallback *args;
};





//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end

typedef struct ProcessStruct Process;

struct ProcessStruct{
  pid_t process;
  void *stack;
  int size_stack;
};







#endif

#ifndef silverchain_typesH
#define silverchain_typesH

//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end

typedef struct CerradoSynStruct CerradoSyn;

struct CerradoSynStruct{
  Process **process_list;
  int pid_father;
  size_t size_process;
  const char *name_class;
  void *memory;
  CerradoSyn *class_list;
};







#endif

#ifndef silverchain_dec
#define silverchain_dec

//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end


int private_free_interrupted(void *arg_for_verify, void **args_for_free, size_t size_args);






//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



void private_clone_process(Process *process, CallbackProcess *callback, int *flags);





//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end


bool create_process(CerradoSyn *main_process, CallbackProcess *callback, int *flags);




//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



CallbackProcess *new_CallbackProcess(int (*function)(ArgumentsCallback *arguments), ArgumentCallback *primary_arg);

ArgumentCallback *new_argument(const char *name_argument, void *arg, size_t arg_size);

void private_free_argument(ArgumentCallback *self);

void free_callback(CallbackProcess *self);

void add_argument(CallbackProcess *callback_self, ArgumentCallback *add_arg);







//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end




CerradoSyn *new_CerradoSynStruct(const char *class_name);

void free_CerradoSyn(CerradoSyn *self);






//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



Process *private_new_process(int size_stack);

void private_free_process(Process *self);





#endif

#ifndef silverchain_def
#define silverchain_def

//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



int private_free_interrupted(void *arg_for_verify, void **args_for_free, size_t size_args){

  if(arg_for_verify != NULL){
    return 1;
  }

  for(int i = 0; i < size_args; i++){
    if((args_for_free[i]) == NULL){
      free(args_for_free[i]);
    }
  }

  return -1;
}






//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end

int callback_config(void *arg){


  CallbackProcess *stuct_arg = (CallbackProcess *)arg;

  int(*function_callback)(ArgumentsCallback *arguments);
  function_callback = stuct_arg->function_callback;

  int var_return = function_callback(stuct_arg->args);

  return var_return;
}

void private_clone_process(Process *process, CallbackProcess *callback, int *flags){

  if(!process){
    perror("Error cloning process");
    exit(EXIT_FAILURE);
  }

  pid_t pid_process = clone(callback_config, process->stack + process->size_stack - 1, flags?*flags:SIGCHLD, callback);

  process->process = pid_process;


}






//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end




bool create_process(CerradoSyn *main_process, CallbackProcess *callback, int *flags) {
  if (getpid() != main_process->pid_father) {
      return false;
  }

  if(!main_process || !callback){
    return false;
  }

  Process *new_process = private_new_process(_SIZE_STACK_PROCESS_1MB_);

  private_clone_process(new_process, callback, flags);
  if(new_process->process == -1){
    private_free_process(new_process);
    return false;
  }

  main_process->size_process++;

  main_process->process_list = (Process **)realloc(main_process->process_list, (main_process->size_process + 1) * sizeof(Process *));

  main_process->process_list[main_process->size_process - 1] = new_process;

  return true;
}






//silver_chain_scope_start
//mannaged by silver chain

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

CallbackProcess *new_CallbackProcess(int (*function)(ArgumentsCallback *arguments), ArgumentCallback *primary_arg){

  if(!function || !primary_arg){
    return NULL;
  }

  CallbackProcess *self = (CallbackProcess *)malloc(sizeof(CallbackProcess));
  if(!private_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->function_callback = function;

  self->args = (ArgumentsCallback *)malloc(sizeof(ArgumentsCallback) * 2);
  if(!private_free_interrupted(self->args, (void *[]){self}, 1)){
    return NULL;
  }

  self->args->arguments = (ArgumentCallback **)malloc(sizeof(ArgumentCallback *) * 2);
  if(!private_free_interrupted(self->args->arguments, (void *[]){self->args, self}, 2)){
    return NULL;
  }

  self->args->size_arguments = 1;

  self->args->arguments[0] = (ArgumentCallback *)malloc(sizeof(*primary_arg));
  if(!private_free_interrupted(self->args->arguments[0], (void *[]){self->args->arguments, self->args, self}, 3)){
    return NULL;
  }

  self->args->arguments[0] = primary_arg;

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





//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end

CerradoSyn *new_CerradoSynStruct(const char *class_name){

  static bool primary_run = true;
  if(primary_run){
    setbuf(stdout, NULL);
  }
  primary_run = false;

  CerradoSyn *self = malloc(sizeof(CerradoSyn));
  if(!private_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->pid_father = getpid();

  self->process_list = (Process **)malloc(sizeof(Process *) * 2);
  if(!private_free_interrupted(self->process_list, (void *[]){self}, 1)){
    return NULL;
  }

  self->size_process = 0;

  self->name_class = (const char *)malloc(strlen(class_name) + 1);
  if(!private_free_interrupted((char *)self->name_class, (void *[]){self->process_list, self}, 2)){
    return NULL;
  }

  strcpy((char *)self->name_class, class_name);

  self->class_list = NULL;
  
  self->memory = NULL;

  return self;
}

void free_CerradoSyn(CerradoSyn *self){

  if(self != NULL){

    if(self->process_list != NULL){
      for(int i = 0; i < self->size_process + 1; i++){//O tamanho do process_list sempre vai ser uma unidade maior que o size_process por motivos de seguraça;
        if(self->process_list[i] != NULL){
          Process *process_temp = self->process_list[i];
          private_free_process(process_temp);
        }
      }
      free(self->process_list);
    }

    if(self->name_class != NULL){
      free((char *)self->name_class);
    }

    free(self);
  }

  return;
}






//silver_chain_scope_start
//mannaged by silver chain

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




#endif
