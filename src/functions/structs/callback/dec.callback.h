
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.typesH.h"
//silver_chain_scope_end



CallbackProcess *new_CallbackProcess(CerradoSyn *process_father, int (*function)(ArgumentsCallback *arguments));

ArgumentCallback *new_argument(const char *name_argument, void *arg, size_t arg_size);

void private_free_argument(ArgumentCallback *self);

void free_callback(CallbackProcess *self);

void add_argument(CallbackProcess *callback_self, ArgumentCallback *add_arg);





