
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../../imports/imports.typesH.h"
//silver_chain_scope_end



CerradoSync_TrafficMemory *private_CerradoSync_new_TrafficMemory(key_t key);

CerradoSync_TrafficPointerObject *private_CerradoSync_new_TrafficPointerObject(const char *className, int contTraffics, int initialPointer);

CerradoSync_TrafficPointersList *private_CerradoSync_new_TrafficPointersList();

void private_CerradoSync_free_TrafficPointerObject(CerradoSync_TrafficPointerObject *self);

void private_CerradoSync_free_TrafficPointersList(CerradoSync_TrafficPointersList *self);

void private_CerradoSync_free_traffic(CerradoSync_TrafficMemory *self);



