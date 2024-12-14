
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.typesF.h"
#include <sched.h>
//silver_chain_scope_end


struct MemorySharedContent_struct{
  void *memoryShared;
  void *memory;
};

struct MemoryShared_struct{
  MemorySharedContent *memory_shared;
  key_t key;
  pid_t pid;
  int memory_location;
  TrafficPointersList *traffic;
};



