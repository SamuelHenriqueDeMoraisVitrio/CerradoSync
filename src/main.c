
//silver_chain_scope_start
//mannaged by silver chain
#include "imports/imports.def.h"
//silver_chain_scope_end



int process_print_name(MemoryShared *memory, ArgumentsCallback *args){

  const char *name = (const char *)args->arguments[0];

  printf("\n\tname: %s", name);

  return 0;
}

int main(){
  
  CerradoSyn *main = new_CerradoSynStruct("main", _DEFAULT_MAX_SIZE_TRAFFIC_);
  if(!main->memory->traffic){
    printf("Main");
    return 1;
  }

  if(create_pointer_traffic(main, "className", RED_TRAFFIC) == -1){
    printf("\n\tParou aqui\n");
    return 1;
  }


  CallbackProcess *callback = new_CallbackProcess(main, process_print_name);

  const char *name = "Samuel Henrique";
  ArgumentCallback *name_arg = new_argument("teste", (void *)name, strlen(name));
  add_argument(callback, name_arg);

  create_process(main, callback, NULL);
  free_callback(callback);

  printf("\n\tHello Word!\n");
  for (int i = 0; i < main->size_process; i++) {
    pid_t temp_process = main->process_list[i]->process;
    waitpid(temp_process, NULL, 0);
  }
  printf("\n\tBye Word\n");

  free_CerradoSyn(main);

  return 0;
}



