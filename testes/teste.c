
#include "../CerradoSyn.h"

int print_name(ArgumentsCallback *arg){
  printf("Meu nome é %s", (const char *)arg->arguments[0]->arg);
  return 0;
}

int main(){

  CerradoSyn *main_process = new_CerradoSynStruct("main");

  ArgumentCallback *primary_arg = new_argument("name", "samuel", sizeof("samuel"));
  CallbackProcess *function_print = new_CallbackProcess(print_name, primary_arg);

  free_callback(function_print);

  free_CerradoSyn(main_process);
/*

  create_process(main_process, function_print, NULL);

  free_callback(function_print);

  for (int i = 0; i < main_process->size_process; i++) {
    pid_t temp_process = main_process->process_list[i]->process;
    waitpid(temp_process, NULL, 0);
  }

  free_CerradoSyn(main_process);
*/  
  return 0;
}

