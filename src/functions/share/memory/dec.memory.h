
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.typesH.h"
//silver_chain_scope_end



int private_memory_data_attach(CerradoSync_MemoryShared *memory_shared);

ShmidDS *get_info_memory_location(CerradoSync_MemoryShared *memory_shared);

void private_close_memory(CerradoSync_MemoryShared *memory_shared);

void pull_memory(CerradoSync_MemorySharedContent *self);

void push_memory(CerradoSync_MemorySharedContent *self);

void config_memory(CerradoSync_MemorySharedContent *self, void *new_value, size_t size_value);

void private_config_memory_share(CerradoSync_MemorySharedContent *self);




