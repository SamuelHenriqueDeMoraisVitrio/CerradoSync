
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.typesG.h"
//silver_chain_scope_end


struct private_CerradoSyncStruct{
  int pid_father;
  const char *name_class;
  key_t key;

  CerradoSync_Process **process_list;
  size_t size_process;
  
  CerradoSync_MemoryShared *memory;
  CerradoSyn *class_list;
};






