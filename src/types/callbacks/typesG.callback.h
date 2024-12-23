
//silver_chain_scope_start
//mannaged by silver chain
#include "../../imports/imports.typesF.h"
//silver_chain_scope_end





struct private_CerradoSync_CallbackProcess_struct{
  int(*function_callback)(CerradoSync_MemoryShared *memory, CerradoSync_ArgumentsCallback *arguments);
  CerradoSync_ArgumentsCallback *args;
  CerradoSync_MemoryShared *memory;
};



