
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

  self->key = private_creat_key(name_class);
  if(!self->key){
    private_free_interrupted(NULL, (void *[]){self}, size_arguments);
    return NULL;
  }

  self->pid = getpid();

  self->memory_location = shmget(self->key, size_max_traffic, _CONFIG_SHMGET_PERMISSIONS_);
  if(self->memory_location == -1){
    private_free_interrupted(NULL, (void *[]){self}, ++size_arguments);
    printf("erroB");
    return NULL;
  }


  self->memory_shared = private_new_MemorySharedContent(self, size_max_traffic);
  if(!self->memory_shared){
    shmctl(self->memory_location, IPC_RMID, NULL);
    private_free_interrupted(NULL, (void *[]){self}, size_arguments);
    printf("erro");
    return NULL;
  }

  self->traffic = private_new_TrafficPointersList();
  if(!self->traffic){
    private_free_MemorySharedContent(self->memory_shared);
    shmctl(self->memory_location, IPC_RMID, NULL);
    private_free_interrupted(NULL, (void *[]){self}, size_arguments);
    printf("erro");
    return NULL;
  }

  return self;
}

void private_delet_memory(MemoryShared *memory_shared){
  if(memory_shared){
    private_free_TrafficPointersList(memory_shared->traffic);
    private_free_MemorySharedContent(memory_shared->memory_shared);

    shmctl(memory_shared->memory_location, IPC_RMID, NULL);

    free(memory_shared);
  }
}










