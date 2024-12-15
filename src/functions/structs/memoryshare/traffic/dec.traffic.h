
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../../imports/imports.typesH.h"
//silver_chain_scope_end



TrafficMemory *private_new_TrafficMemory(key_t key);

TrafficPointerObject *private_new_TrafficPointerObject(const char *className, int contTraffics, int initialPointer);

TrafficPointersList *private_new_TrafficPointersList();

void private_free_TrafficPointerObject(TrafficPointerObject *self);

void private_free_TrafficPointersList(TrafficPointersList *self);

void private_free_traffic(TrafficMemory *self);



