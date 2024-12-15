
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../../imports/imports.dec.h"
//silver_chain_scope_end



TrafficMemory *private_new_TrafficMemory(key_t key){
  
  TrafficMemory *self = (TrafficMemory *)malloc(sizeof(TrafficMemory));
  if(!private_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->key = key;
  self->trafficID = private_init_traffic(self->key);
  if(self->trafficID == -1){
    free(self);
    return NULL;
  }

  return self;
}

void private_free_traffic(TrafficMemory *self){
  if(self){
    private_close_traffic(self->key);
    free(self);
  }
}





