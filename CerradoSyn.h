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

#ifndef silverchain_typesA
#define silverchain_typesA

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

#ifndef silverchain_typesB
#define silverchain_typesB

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



void private_clone_process(Process *process, int(*function)(void *arg), void *arg, int *flags);





//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end


bool create_process(CerradoSyn *main_process, int (*function_process)(void *arg), void *arg, int *flags);




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



void private_clone_process(Process *process, int(*function)(void *arg), void *arg, int *flags){

  pid_t pid_process = clone(function, process->stack + process->size_stack - 1, flags?*flags:SIGCHLD, NULL);

  process->process = pid_process;
  printf("session:\nsize_stack: %d\nprocess:%d\n", process->size_stack, process->process);


}






//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end




bool create_process(CerradoSyn *main_process, int (*function_process)(void *arg), void *arg, int *flags) {
  if (getpid() != main_process->pid_father) {
      return false;
  }

  Process *new_process = private_new_process(_SIZE_STACK_PROCESS_1MB_);

  private_clone_process(new_process, function_process, arg, flags);
  if(new_process->process == -1){
    private_free_process(new_process);
    return false;
  }

  main_process->size_process++;

  main_process->process_list = (Process **)realloc(main_process->process_list, (main_process->size_process + 1) * sizeof(Process *));

  main_process->process_list[main_process->size_process - 1] = new_process;

  printf("\n\t%d\n", main_process->process_list[main_process->size_process - 1]->process);

  return true;
}






//silver_chain_scope_start
//mannaged by silver chain

//silver_chain_scope_end

CerradoSyn *new_CerradoSynStruct(const char *class_name){
  CerradoSyn *self = malloc(sizeof(CerradoSyn));

  self->pid_father = getpid();

  self->process_list = malloc(0);
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
        if(&(self->process_list[i]) != NULL){
          //private_free_process(&self->process_list[i]);
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
