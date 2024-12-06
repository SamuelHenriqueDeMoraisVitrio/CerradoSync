
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.mac.h"
//silver_chain_scope_end

typedef struct ProcessStruct Process;

struct ProcessStruct{
  pid_t process;
  void *stack;
  int size_stack;
};






