
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.typesH.h"
//silver_chain_scope_end



CerradoSync_CallbackProcess *CerradoSync_new_CallbackProcess(CerradoSync *process_father, int (*function)(CerradoSync_MemoryShared *memory, CerradoSync_ArgumentsCallback *arguments));

CerradoSync_ArgumentCallback *CerradoSync_new_argument(const char *name_argument, void *arg, size_t arg_size);

void private_CerradoSync_free_argument(CerradoSync_ArgumentCallback *self);

void private_CerradoSync_free_callback(CerradoSync_CallbackProcess *self);

int CerradoSync_add_argument(CerradoSync_CallbackProcess *callback_self, CerradoSync_ArgumentCallback *add_arg);

CerradoSync_ArgumentsCallback *private_CerradoSync_new_ArgumentsCallback();

void private_CerradoSync_free_ArgumentsCallback(CerradoSync_ArgumentsCallback *self);





