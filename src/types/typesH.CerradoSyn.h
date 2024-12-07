
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.typesG.h"
//silver_chain_scope_end

typedef struct CerradoSynStruct CerradoSyn;

struct CerradoSynStruct{
  Process **process_list;
  int pid_father;
  size_t size_process;
  const char *name_class;
  void *memory;
  CerradoSyn *class_list;
};






