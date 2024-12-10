
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end


MemoryShared *private_new_MemorySahred_struct(const char *name_class, size_t size_max_traffic){
  
  MemoryShared *self = (MemoryShared *)malloc(sizeof(MemoryShared) + 1);

  self->key = private_creat_key(name_class);

  self->memory_location = shmget(self->key, size_max_traffic, _CONFIG_SHMGET_PERMISSIONS_);
  if(self->memory_location == -1){
    fprintf(stderr, "Error creating shared memory in: %d", self->key);
    return NULL;
  }

  self->memory = (void *)shmat(self->memory_location, NULL, 0);
  if(self->memory == (void *)-1){
    fprintf(stderr, "Error creating shared memory in: %d", self->key);
    private_delet_memory(self);
    return NULL;
  }

  return self;
}

void *private_memory_data_attach(MemoryShared *memory_shared){
  void *data = (void *)shmat(memory_shared->memory_location, NULL, 0);
  if(data == (void *)-1){
    perror("Error attaching memory");
    return NULL;
  }

  return data;
}

void private_close_memory(MemoryShared *memory_shared){
  shmdt(memory_shared->memory);
}

void private_delet_memory(MemoryShared *memory_shared){
  private_close_memory(memory_shared->memory);

  shmctl(memory_shared->memory_location, IPC_RMID, NULL);
  if(shmctl(memory_shared->memory_location, IPC_RMID, NULL) == -1){
    perror("Error get stats memory");
  }
}

ShmidDS *get_info_memory_location(MemoryShared *memory_shared){
  ShmidDS *shmInfo;
  if(shmctl(memory_shared->memory_location, IPC_STAT, shmInfo) == -1){
    perror("Error get stats memory");
    return NULL;
  }

  return shmInfo;
}








