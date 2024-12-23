
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.typesF.h"
//silver_chain_scope_end


struct private_CerradoSync_MemorySharedContent_struct{
  void *memoryShared;
  void *memory;
  size_t size_memory;
  size_t size_memoryShared;
  CerradoSync_TrafficMemory *traffic;
};

struct private_CerradoSync_MemoryShared_struct{
  CerradoSync_MemorySharedContent *memory_shared;
  key_t key;
  pid_t pid;
  int memory_location;
  CerradoSync_TrafficPointersList *traffic;
};



