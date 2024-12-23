
//silver_chain_scope_start
//mannaged by silver chain
#include "../../imports/imports.dec.h"
//silver_chain_scope_end



int private_CerradoSync_free_interrupted(void *arg_for_verify, void **args_for_free, size_t size_args){

  if(arg_for_verify != NULL){
    return 1;
  }

  for(int i = 0; i < size_args; i++){
    if((args_for_free[i]) == NULL){
      free(args_for_free[i]);
    }
  }

  return 0;
}




