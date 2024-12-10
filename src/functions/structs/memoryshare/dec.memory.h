
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.typesH.h"
//silver_chain_scope_end



MemoryShared *private_new_MemorySahred_struct(const char *name_class, size_t size_max_traffic);

void *memory_data_attach(int memory_identification);

void private_close_memory(void *data_memory);

void private_delet_memory(MemoryShared *memory_shared);

ShmidDS *get_info_memory_location(int memory_identification);


