
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end




int private_CerradoSync_memory_data_attach(CerradoSync_MemoryShared *memory_shared){
  void *data = (void *)shmat(memory_shared->memory_location, NULL, 0);
  if(data == (void *)-1){
    return -1;
  }

  memory_shared->memory_shared->memoryShared = data;
  return 1;
}

void private_CerradoSync_close_memory(CerradoSync_MemoryShared *memory_shared){

  shmdt(memory_shared->memory_shared->memoryShared);
}

int CerradoSync_get_info_memory_location(CerradoSync_MemoryShared *memory_shared, ShmidDS *shmInfo){
  if(shmctl(memory_shared->memory_location, IPC_STAT, shmInfo) == -1){
    return -1;
  }

  return 1;
}

void CerradoSync_pull_memory(CerradoSync_MemorySharedContent *self){
  
  private_CerradoSync_signal_traffic(self->traffic->trafficID, 0, -1);// pedindo ascesso à memoria;

  CerradoSync_config_memory(self, self->memoryShared, self->size_memoryShared);//Lendo

  private_CerradoSync_signal_traffic(self->traffic->trafficID, 0, 1);// Entregando ascesso à memoria;

}

void CerradoSync_push_memory(CerradoSync_MemorySharedContent *self){

  private_CerradoSync_signal_traffic(self->traffic->trafficID, 0, -1);// pedindo ascesso à memoria;

  private_CerradoSync_config_memory_share(self);// Gravando

  private_CerradoSync_signal_traffic(self->traffic->trafficID, 0, 1);// Entregando ascesso à memoria;

}

void CerradoSync_config_memory(CerradoSync_MemorySharedContent *self, void *new_value, size_t size_value){

  memset(self->memory, 0, self->size_memory);

  self->size_memory = size_value + 1;

  self->memory = (void *)realloc(self->memory, self->size_memory);

  memcpy(self->memory, new_value , size_value);
}

void private_CerradoSync_config_memory_share(CerradoSync_MemorySharedContent *self){

  memset(self->memoryShared, 0, self->size_memoryShared);

  memcpy(self->memoryShared, self->memory, self->size_memory);

}






