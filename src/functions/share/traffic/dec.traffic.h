
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.typesH.h"
//silver_chain_scope_end



int private_CerradoSync_get_stats_traffic(int id, int index_get);

int private_CerradoSync_init_traffic(key_t key);

int private_CerradoSync_creat_a_wait_point(const char *className, int initial, int number_traffics, key_t *key);

int private_CerradoSync_wait(key_t key, int number_traffics, int index_get, int color);

int private_CerradoSync_signal_traffic(int id, int index_traffic, int color);

void private_CerradoSync_close_traffic(int id);

int CerradoSync_create_pointer_traffic(CerradoSync *self, const char *className, int initial_pointer);

int CerradoSync_wait_traffic(CerradoSync_MemoryShared *memory, const char *className, int color);

int CerradoSync_signal_traffic(CerradoSync_MemoryShared *memory, const char *className, int color);









