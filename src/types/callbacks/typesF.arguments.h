
//silver_chain_scope_start
//mannaged by silver chain
#include "../../imports/imports.mac.h"
//silver_chain_scope_end



typedef struct Arguments_struct ArgumentsCallback;
typedef struct Argument_struct ArgumentCallback;

struct Arguments_struct{
  int size_arguments;
  ArgumentCallback **arguments;
};

struct Argument_struct{
  void *arg;
  const char *name_argument;
};




