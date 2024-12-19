
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../../imports/imports.dec.h"
#include <stddef.h>
//silver_chain_scope_end



MemorySharedContent *private_new_MemorySharedContent(MemoryShared *memory_struct, size_t size_memoryShared){

  MemorySharedContent *self = (MemorySharedContent*)malloc(sizeof(MemorySharedContent));
  if(!private_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->size_memoryShared = size_memoryShared;

  self->traffic = private_new_TrafficMemory(memory_struct->key);
  if(!private_free_interrupted(self->traffic, (void *[]){self}, 1)){
    return NULL;
  }

  self->memoryShared = (void *)shmat(memory_struct->memory_location, NULL, 0);;
  if(self->memoryShared == (void *)-1){
    private_free_traffic(self->traffic);
    private_free_interrupted(NULL, (void *[]){self}, 1);
    return NULL;
  }

  self->size_memory = sizeof(self->memoryShared);
  self->memory = (void *)malloc(self->size_memory + 1);
  if(!self->memory){
    private_close_memory(memory_struct);
    private_free_traffic(self->traffic);
    private_free_interrupted(NULL, (void *[]){self}, 1);
    return NULL;
  }

  memcpy(self->memory, self->memoryShared, sizeof(self->memoryShared));

  return self;
}

void private_free_MemorySharedContent(MemorySharedContent *self){
  if(self){
    
    if(self->memory){
      free(self->memory);
    }
    
    if(self->memoryShared != (void *)-1){
      shmdt(self->memoryShared);
    }

    if(self->traffic){
      private_free_traffic(self->traffic);
    }

    free(self);
  }
}



