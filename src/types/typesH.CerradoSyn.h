
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.typesG.h"
//silver_chain_scope_end


struct CerradoSynStruct{
  int pid_father;
  const char *name_class;
  key_t key;

  Process **process_list;
  size_t size_process;
  
  MemoryShared *memory;
  CerradoSyn *class_list;
};






