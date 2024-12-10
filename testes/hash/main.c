

#include "../../CerradoSyn.h"



int second_process(ArgumentsCallback *args){

  const char *key_father = args->arguments[1]->arg;
  const char *key_son = private_get_key(args->arguments[0]->arg);
  printf("\n\tkey_son   : %s\n", key_son);
  printf("\n\tkey_father: %s\n", key_father);

  if(strcmp(key_son, key_father) == 0){
    printf("\n\tequal\n");
    return 0;
  }

  printf("\n\tDiferent\n");

  return 0;
}


int main(){
  
  CerradoSyn *main = new_CerradoSynStruct("samuel");

  ArgumentCallback *new_primary_argument = new_argument("class", (char *)main->name_class, strlen(main->name_class));
  CallbackProcess *callback_main = new_CallbackProcess(second_process, new_primary_argument);

  const char *key_father = private_creat_key(main->name_class);

  ArgumentCallback *new_second_argument = new_argument("key_father", (char *)key_father, strlen(key_father));
  add_argument(callback_main, new_second_argument);

  create_process(main, callback_main, NULL);
  free_callback(callback_main);

  printf("\n\tHello Word!\n");
  for (int i = 0; i < main->size_process; i++) {
    pid_t temp_process = main->process_list[i]->process;
    waitpid(temp_process, NULL, 0);
  }
  printf("\n\tBye Word %d\n", getpid());

  free_CerradoSyn(main);

  return 0;
}


