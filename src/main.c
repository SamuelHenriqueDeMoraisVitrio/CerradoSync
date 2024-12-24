
//silver_chain_scope_start
//mannaged by silver chain
#include "imports/imports.def.h"
//silver_chain_scope_end



int process_print_name(CerradoSync_MemoryShared *memory, CerradoSync_ArgumentsCallback *args){

  const char *name = (const char *)args->arguments[0]->arg;

  printf("\n\tname: %s\n", name);

  CerradoSync_signal_traffic(memory, "className", RED_TRAFFIC);//Sends a signal for traffic with name 'className' to be closed

  CerradoSync_pull_memory(memory);

  int *valorMemoryAnterior = (int *)CerradoSync_getMemoryValue(memory);

  printf("\n\tProcess son with memory: %d\n", *valorMemoryAnterior);

  char *meu_nomeCompleto = "Samuel Henrique De Morias Vitrio";
  CerradoSync_config_memory(memory, meu_nomeCompleto, strlen(meu_nomeCompleto) + 1); //Updates static memory with the specified value

  CerradoSync_push_memory(memory);

  sleep(8);

  CerradoSync_signal_traffic(memory, "className", GREEN_TRAFFIC); //Sends a signal for traffic with name 'className' to be opened

  printf("\n\tValor novo : %s\n", (const char *)memory->memory_shared->memory);

  sleep(8);

  return 0;
}

int main(){

  int result;

  CerradoSync *main = new_CerradoSyncStruct("main", _DEFAULT_MAX_SIZE_MEMORY_TRAFFIC_); //create a class of process
  if(!main){
    printf("null");
    return 1;
  }

  CerradoSync_MemoryShared *memory = main->memory; //The memory structure that contains global memory and static memory to work with

  CerradoSync_pull_memory(memory); //Update memory static
  int valorAtual = 450;
  CerradoSync_config_memory(memory, &valorAtual, sizeof(int)); //Assigns a value to static memory
  CerradoSync_push_memory(memory); //Update global memory with static memory

  printf("\n\tpai: %d\n", (int)*((int *)CerradoSync_getMemoryValue(memory))); //Get pointer of static memory value

  result = CerradoSync_create_pointer_traffic(main, "className", RED_TRAFFIC); //Creates an initially blocked traffic point
  if(result == -1){
    printf("null");
    return 1;
  }

  CerradoSync_CallbackProcess *callback = CerradoSync_new_CallbackProcess(main, process_print_name);//Struct callback process

  const char *name = "Samuel Henrique";
  CerradoSync_ArgumentCallback *name_arg = CerradoSync_new_argument("teste", (void *)name, strlen(name));//A new argument for callback
  CerradoSync_add_argument(callback, name_arg);//Assign argument for callback

  result = CerradoSync_create_process(main, callback, CERRADOSYNC_THREADPROCESS); //Create a process independent
  if(result == -1){
    printf("null");
    return 1;
  }

  CerradoSync_wait_traffic(memory, "className", GREEN_TRAFFIC); //Wait pointer traffic 'className' stay open
  CerradoSync_pull_memory(memory); //Get the latest information from global memory and copy it to static memory

  printf("\n\tpai depois: %s\n", (const char *)CerradoSync_getMemoryValue(memory));

  printf("\n\tHello Word!\n");
  /*Process wait does not work with threads
  if(CerradoSync_wait_class_process_ended(main) == CERRADOSYNC_ERROR_A_GET_STATUS){
    printf("error");
  }
  */
  printf("\n\tBye Word\n");

  free_CerradoSync(main);//Releases all allocated memory of the created process class

  return 0;
}



