
//silver_chain_scope_start
//mannaged by silver chain
#include "../../imports/imports.typesF.h"
//silver_chain_scope_end





struct CallbackProcess_struct{
  int(*function_callback)(MemoryShared *memory, ArgumentsCallback *arguments);
  ArgumentsCallback *args;
  MemoryShared *memory;
};



