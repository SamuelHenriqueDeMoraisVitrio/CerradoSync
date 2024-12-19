
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.typesH.h"
//silver_chain_scope_end



int private_memory_data_attach(MemoryShared *memory_shared);

ShmidDS *get_info_memory_location(MemoryShared *memory_shared);

void private_close_memory(MemoryShared *memory_shared);

void pull_memory(MemorySharedContent *self);

void push_memory(MemorySharedContent *self);

void config_memory(MemorySharedContent *self, void *new_value, size_t size_value);

void private_config_memory_share(MemorySharedContent *self);




