
//silver_chain_scope_start
//mannaged by silver chain
#include "imports/imports.def.h"
//silver_chain_scope_end



int process_print_name(CerradoSync_MemoryShared *memory, CerradoSync_ArgumentsCallback *args){

  const char *name = (const char *)args->arguments[0]->arg;

  printf("\n\tname: %s\n", name);

  CerradoSync_signal_traffic(memory, "className", RED_TRAFFIC);

  CerradoSync_pull_memory(memory->memory_shared);

  int *valorMemoryAnterior = (int *)memory->memory_shared->memory;

  printf("\n\tProcess son with memory: %d\n", *valorMemoryAnterior);

  char *meu_nomeCompleto = "Samuel Henrique De Morias Vitrio";
  CerradoSync_config_memory(memory->memory_shared, meu_nomeCompleto, strlen(meu_nomeCompleto) + 1);

  CerradoSync_push_memory(memory->memory_shared);

  CerradoSync_signal_traffic(memory, "className", GREEN_TRAFFIC);

  printf("aq");

  printf("\n\tValor novo : %s\n", (const char *)memory->memory_shared->memory);

  return 0;
}

int main(){

  CerradoSync *main = new_CerradoSyncStruct("main", _DEFAULT_MAX_SIZE_MEMORY_TRAFFIC_);
  return 1;

  CerradoSync_MemoryShared *memory = main->memory;
  CerradoSync_pull_memory(memory->memory_shared);
  int valorAtual = 450;
  CerradoSync_config_memory(memory->memory_shared, &valorAtual, sizeof(int));
  CerradoSync_push_memory(memory->memory_shared);

  printf("\n\tpai: %d\n", (int)*((int *)memory->memory_shared->memory));

  CerradoSync_create_pointer_traffic(main, "className", RED_TRAFFIC);

  CerradoSync_CallbackProcess *callback = CerradoSync_new_CallbackProcess(main, process_print_name);

  const char *name = "Samuel Henrique";
  CerradoSync_ArgumentCallback *name_arg = CerradoSync_new_argument("teste", (void *)name, strlen(name));
  CerradoSync_add_argument(callback, name_arg);

  CerradoSync_create_process(main, callback, NULL);

  CerradoSync_wait_traffic(memory, "className", GREEN_TRAFFIC);
  CerradoSync_pull_memory(memory->memory_shared);

  printf("\n\tpai depois: %s\n", (const char *)memory->memory_shared->memory);

  printf("\n\tHello Word!\n");
  for (int i = 0; i < main->size_process; i++) {
    pid_t temp_process = main->process_list[i]->process;
    int status;
    waitpid(temp_process, &status, 0);
    printf("\n\tstatus: %d of process: %d\n", WIFEXITED(status), i);
  }
  printf("\n\tBye Word\n");

  free_CerradoSync(main);

  return 0;
}



