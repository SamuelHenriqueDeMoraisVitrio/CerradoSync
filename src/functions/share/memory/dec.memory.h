
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.typesH.h"
//silver_chain_scope_end



int private_CerradoSync_memory_data_attach(CerradoSync_MemoryShared *memory_shared);

int CerradoSync_get_info_memory_location(CerradoSync_MemoryShared *memory_shared, ShmidDS *shmInfo);

void private_CerradoSync_close_memory(CerradoSync_MemoryShared *memory_shared);

void CerradoSync_pull_memory(CerradoSync_MemoryShared *self_shared);

void CerradoSync_push_memory(CerradoSync_MemoryShared *self_shared);

void CerradoSync_config_memory(CerradoSync_MemoryShared *self_shared, void *new_value, size_t size_value);

void private_CerradoSync_config_memory_share(CerradoSync_MemorySharedContent *self);

void *CerradoSync_getMemoryValue(CerradoSync_MemoryShared *memory);




