
//silver_chain_scope_start
//mannaged by silver chain
#include "imports/imports.def.h"
//silver_chain_scope_end


int print_name(void *arg){
  printf("Meu nome é %s", (const char *)arg);
  return 0;
}


int main(){

  CerradoSyn *main_process = new_CerradoSynStruct("main");

  create_process(main_process, print_name, "samuel", NULL);

  for (int i = 0; i < main_process->size_process; i++) {
      waitpid(main_process->process_list[i].process, NULL, 0);
  }

  free_CerradoSyn(main_process);
  
  return 0;
}



