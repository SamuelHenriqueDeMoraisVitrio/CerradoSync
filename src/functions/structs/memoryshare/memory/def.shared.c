
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../../imports/imports.dec.h"
//silver_chain_scope_end



MemorySharedContent *private_new_MemorySharedContent(void *memory, key_t key){
  MemorySharedContent *self = (MemorySharedContent*)malloc(sizeof(MemorySharedContent));
  if(!private_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->traffic = private_new_TrafficMemory(key);
  if(!private_free_interrupted(self->traffic, (void *[]){self}, 1)){
    return NULL;
  }
  self->memoryShared = memory;
  self->memory = (void *)malloc(sizeof(memory));
  if(!private_free_interrupted(self->memory, (void *[]){self}, 1)){
    return NULL;
  }
  memcpy(self->memory, memory, sizeof(memory));

  return self;
}

void private_free_MemorySharedContent(MemorySharedContent *self){
  if(self){
    if(self->traffic){
      private_free_traffic(self->traffic);
    }
    if(self->memory){
      free(self->memory);
    }
    
    if(self->memoryShared != (void *)-1){
      shmdt(self->memoryShared);
    }
    free(self);
  }
}



