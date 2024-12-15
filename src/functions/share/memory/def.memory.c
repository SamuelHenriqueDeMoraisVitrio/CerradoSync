
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end




void *private_memory_data_attach(MemoryShared *memory_shared){
  void *data = (void *)shmat(memory_shared->memory_location, NULL, 0);
  if(data == (void *)-1){
    return NULL;
  }

  return data;
}

void private_close_memory(MemoryShared *memory_shared){

  shmdt(memory_shared->memory_shared->memory);
}

ShmidDS *get_info_memory_location(MemoryShared *memory_shared){
  ShmidDS *shmInfo;
  if(shmctl(memory_shared->memory_location, IPC_STAT, shmInfo) == -1){
    return NULL;
  }

  return shmInfo;
}

void pull_memory(MemorySharedContent *self){
  
  private_signal_traffic(self->traffic->trafficID, 0, -1);// pedindo ascesso à memoria;

  memcpy(self->memory, self->memoryShared, sizeof(self->memoryShared));//Lendo

  private_signal_traffic(self->traffic->trafficID, 0, 1);// Entregando ascesso à memoria;

}

void push_memory(MemorySharedContent *self){

  private_signal_traffic(self->traffic->trafficID, 0, -1);// pedindo ascesso à memoria;

  memcpy(self->memoryShared, self->memory, sizeof(self->memory));// Gravando

  private_signal_traffic(self->traffic->trafficID, 0, 1);// Entregando ascesso à memoria;

}






