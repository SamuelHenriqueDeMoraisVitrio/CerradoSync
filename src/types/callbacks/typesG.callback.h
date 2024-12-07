
//silver_chain_scope_start
//mannaged by silver chain
#include "../../imports/imports.typesF.h"
//silver_chain_scope_end




typedef struct CallbackProcess_struct CallbackProcess;

struct CallbackProcess_struct{

  int(*function_callback)(ArgumentsCallback *args);
  int size_arguments;
  ArgumentsCallback **arguments;

};



