

#include "../../CerradoSyn.h"

#include <stdio.h>
#include <unistd.h>

int print_name_filho(ArgumentsCallback *args){

  printf("\n\tMeu nome é %s\n", (const char *)args->arguments[0]->arg);
  sleep(2);

  return 0;

}

int print_name(ArgumentsCallback *arg){

  CerradoSyn *process_filho = new_CerradoSynStruct("filho1");
  CerradoSyn *process_filho2 = new_CerradoSynStruct("filho2");

  ArgumentCallback *primary_argument = new_argument("name", "Danilo", sizeof("Danilo"));
  CallbackProcess *funcrion_callback_filho = new_CallbackProcess(print_name_filho, primary_argument);

  ArgumentCallback *primary_argument_2 = new_argument("name", "Samuel H", sizeof("Samuel H"));
  CallbackProcess *function_callback_filho_2 = new_CallbackProcess(print_name_filho, primary_argument_2);

  create_process(process_filho, funcrion_callback_filho, NULL);
  create_process(process_filho2, function_callback_filho_2, NULL);

  free_callback(funcrion_callback_filho);
  free_callback(function_callback_filho_2);

  for (int i = 0; i < process_filho->size_process; i++) {
    pid_t temp_process = process_filho->process_list[i]->process;
    waitpid(temp_process, NULL, 0);
  }
  printf("\n\tFim processos filho filho 1 %d\n", process_filho->process_list[0]->process);

  for (int i = 0; i < process_filho2->size_process; i++) {
    pid_t temp_process = process_filho2->process_list[i]->process;
    waitpid(temp_process, NULL, 0);
  }
  printf("\n\tFim processos filho filho 2 %d\n", process_filho2->process_list[0]->process);

  printf("\n\tFinal processos filho %d\n", getpid());
  return 0;
}

int main(){

  CerradoSyn *main_process = new_CerradoSynStruct("main");

  ArgumentCallback *primary_arg = new_argument("name", "samuel", sizeof("samuel"));
  CallbackProcess *function_print = new_CallbackProcess(print_name, primary_arg);

  create_process(main_process, function_print, NULL);

  free_callback(function_print);

  printf("\n\tHello Word!\n");
  for (int i = 0; i < main_process->size_process; i++) {
    pid_t temp_process = main_process->process_list[i]->process;
    waitpid(temp_process, NULL, 0);
  }
  printf("\n\tBye Word %d\n", getpid());

  free_CerradoSyn(main_process);

  return 0;
}



