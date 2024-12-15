
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end


MemoryShared *private_new_MemorySahred_struct(const char *name_class, size_t size_max_traffic){
  
  short size_arguments = 0;

  MemoryShared *self = (MemoryShared *)malloc(sizeof(MemoryShared) + 1);
  if(!private_free_interrupted(self, NULL, size_arguments)){
    return NULL;
  }

  self->memory_location = shmget(self->key, size_max_traffic, _CONFIG_SHMGET_PERMISSIONS_);
  if(self->memory_location == -1){
    private_free_interrupted(NULL, (void *[]){self}, ++size_arguments);
    return NULL;
  }

  void *memory = (void *)shmat(self->memory_location, NULL, 0);
  if(memory == (void *)-1){
    shmctl(self->memory_location, IPC_RMID, NULL);
    private_free_interrupted(NULL, (void *[]){self}, size_arguments);
    return NULL;
  }

  self->memory_shared = private_new_MemorySharedContent(memory);
  if(self->memory_shared == NULL){
    shmdt(self->memory_shared->memory);
    shmctl(self->memory_location, IPC_RMID, NULL);
    private_free_interrupted(self->memory_shared, (void *[]){self}, size_arguments);
    return NULL;
  }

  self->key = private_creat_key(name_class);
  self->pid = getpid();

  self->memory_shared->traffic = private_new_TrafficMemory(self->key);
  if(self->memory_shared->traffic == NULL){
    shmdt(self->memory_shared->memory);
    shmctl(self->memory_location, IPC_RMID, NULL);
    private_free_MemorySharedContent(self->memory_shared);
    private_free_interrupted(self->memory_shared->traffic, (void *[]){self}, ++size_arguments);
    return NULL;
  }

  return self;
}

void private_delet_memory(MemoryShared *memory_shared){
  if(memory_shared != NULL){
    private_close_memory(memory_shared);

    private_free_MemorySharedContent(memory_shared->memory_shared);

    private_free_traffic(memory_shared->memory_shared->traffic);

    shmctl(memory_shared->memory_location, IPC_RMID, NULL);

    free(memory_shared);
  }
}










