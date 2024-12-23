
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end


CerradoSync_MemoryShared *private_CerradoSync_new_MemorySahred_struct(const char *name_class, size_t size_max_traffic){
  
  short size_arguments = 0;

  CerradoSync_MemoryShared *self = (CerradoSync_MemoryShared *)malloc(sizeof(CerradoSync_MemoryShared) + 1);
  if(!private_CerradoSync_free_interrupted(self, NULL, size_arguments)){
    return NULL;
  }

  self->key = private_CerradoSync_creat_key(name_class);
  if(!self->key){
    private_CerradoSync_free_interrupted(NULL, (void *[]){self}, size_arguments);
    return NULL;
  }

  self->pid = getpid();

  self->memory_location = shmget(self->key, size_max_traffic, _CONFIG_SHMGET_PERMISSIONS_);
  if(self->memory_location == -1){
    private_CerradoSync_free_interrupted(NULL, (void *[]){self}, ++size_arguments);
    return NULL;
  }

  self->memory_shared = private_CerradoSync_new_MemorySharedContent(self, size_max_traffic);
  if(!self->memory_shared){
    shmctl(self->memory_location, IPC_RMID, NULL);
    private_CerradoSync_free_interrupted(NULL, (void *[]){self}, size_arguments);
    return NULL;
  }

  self->traffic = private_CerradoSync_new_TrafficPointersList();
  if(!self->traffic){
    private_CerradoSync_free_MemorySharedContent(self->memory_shared);
    shmctl(self->memory_location, IPC_RMID, NULL);
    private_CerradoSync_free_interrupted(NULL, (void *[]){self}, size_arguments);
    return NULL;
  }

  return self;
}

void private_CerradoSync_delet_memory(CerradoSync_MemoryShared *memory_shared){
  if(memory_shared){
    private_CerradoSync_free_TrafficPointersList(memory_shared->traffic);
    private_CerradoSync_free_MemorySharedContent(memory_shared->memory_shared);

    shmctl(memory_shared->memory_location, IPC_RMID, NULL);

    free(memory_shared);
  }
}










