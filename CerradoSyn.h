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

int callback_config(void *arg){

  setbuf(stdout, NULL);

  CallbackProcess *stuct_arg = (CallbackProcess *)arg;

  int(*function_callback)(ArgumentsCallback *arguments);
  function_callback = stuct_arg->function_callback;

  int var_return = function_callback(stuct_arg->args);

  //fflush(stdout);

  return var_return;
}

void private_clone_process(Process *process, CallbackProcess *callback, int *flags){

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





//silver_chain_scope_start
//mannaged by silver chain

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






//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end



Process *private_new_process(int size_stack){

  Process *self = malloc(sizeof(Process));

  self->size_stack = size_stack;
  self->stack = malloc(self->size_stack);

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
