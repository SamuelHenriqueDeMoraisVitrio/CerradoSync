
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../../imports/imports.dec.h"
//silver_chain_scope_end



CerradoSync_MemorySharedContent *private_CerradoSync_new_MemorySharedContent(CerradoSync_MemoryShared *memory_struct, size_t size_memoryShared){

  CerradoSync_MemorySharedContent *self = (CerradoSync_MemorySharedContent*)malloc(sizeof(CerradoSync_MemorySharedContent));
  if(!private_CerradoSync_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->size_memoryShared = size_memoryShared;

  self->traffic = private_CerradoSync_new_TrafficMemory(memory_struct->key);
  if(!private_CerradoSync_free_interrupted(self->traffic, (void *[]){self}, 1)){
    return NULL;
  }

  self->memoryShared = (void *)shmat(memory_struct->memory_location, NULL, 0);;
  if(self->memoryShared == (void *)-1){
    private_CerradoSync_free_traffic(self->traffic);
    private_CerradoSync_free_interrupted(NULL, (void *[]){self}, 1);
    return NULL;
  }

  self->size_memory = sizeof(self->memoryShared);
  self->memory = (void *)malloc(self->size_memory + 1);
  if(!self->memory){
    private_CerradoSync_close_memory(memory_struct);
    private_CerradoSync_free_traffic(self->traffic);
    private_CerradoSync_free_interrupted(NULL, (void *[]){self}, 1);
    return NULL;
  }

  memcpy(self->memory, self->memoryShared, self->size_memory);

  return self;
}

void private_CerradoSync_free_MemorySharedContent(CerradoSync_MemorySharedContent *self){
  if(self){
    
    if(self->memory){
      free(self->memory);
    }
    
    if(self->memoryShared != (void *)-1){
      shmdt(self->memoryShared);
    }

    if(self->traffic){
      private_CerradoSync_free_traffic(self->traffic);
    }

    free(self);
  }
}



