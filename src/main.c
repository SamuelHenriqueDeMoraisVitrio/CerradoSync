
//silver_chain_scope_start
//mannaged by silver chain
#include "imports/imports.def.h"
//silver_chain_scope_end



int process_son(ArgumentsCallback *args){

  MemoryShared *memory = (MemoryShared *)args->arguments[0]->arg;
  printf("\n\t4: {%s}\n", (const char *)memory->memory);

  strcpy(memory->memory, "meu nome é Henrique");

  printf("\n\t5: {%s}\n", (const char *)memory->memory);

  return 0;
}


int main(){
  
  CerradoSyn *main = new_CerradoSynStruct("main", _DEFAULT_MAX_SIZE_TRAFFIC_);
  create_point_traffic(main, RED);

  strcpy(main->memory->memory, "Meu nome é samuel");
  printf("\n\t1: {%s}\n", (const char *)main->memory->memory);

  CallbackProcess *callback = new_CallbackProcess(main, process_son);

  create_process(main, callback, NULL);

  printf("\n\t2: {%s}\n", (const char *)main->memory->memory);

  free_callback(callback);

  printf("\n\tHello Word!\n");
  for (int i = 0; i < main->size_process; i++) {
    pid_t temp_process = main->process_list[i]->process;
    waitpid(temp_process, NULL, 0);
  }
  printf("\n\tBye Word %d\n", getpid());

  printf("\n\t3: {%s}\n", (const char *)main->memory->memory);

  free_CerradoSyn(main);

  return 0;
}



