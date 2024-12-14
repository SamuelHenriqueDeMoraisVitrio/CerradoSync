
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end


MemoryShared *private_new_MemorySahred_struct(const char *name_class, size_t size_max_traffic){
  
  MemoryShared *self = (MemoryShared *)malloc(sizeof(MemoryShared) + 1);

  self->key = private_creat_key(name_class);
  self->pid = getpid();

  self->memory_location = shmget(self->key, size_max_traffic, _CONFIG_SHMGET_PERMISSIONS_);
  if(self->memory_location == -1){
    return NULL;
  }

  void *memory = (void *)shmat(self->memory_location, NULL, 0);
  if(memory == (void *)-1){
    private_delet_memory(self);
    return NULL;
  }

  self->memory_shared = private_new_MemorySharedContent(memory);
  if(self->memory_shared == NULL){
    private_delet_memory(self);
    return NULL;
  }

  return self;
}

void private_delet_memory(MemoryShared *memory_shared){
  if(memory_shared != NULL){
    private_close_memory(memory_shared->memory_shared->memory);

    shmctl(memory_shared->memory_location, IPC_RMID, NULL);

    if(memory_shared->memory_shared != NULL){
      private_free_MemorySharedContent(memory_shared->memory_shared);
    }

    free(memory_shared);
  }
}










