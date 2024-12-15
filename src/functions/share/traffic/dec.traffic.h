
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.typesH.h"
//silver_chain_scope_end



int private_get_stats_traffic(int id, int index_get);

int private_init_traffic(key_t key);

int private_creat_a_wait_point(const char *className, int initial, int number_traffics, key_t *key);

int private_wait(key_t key, int number_traffics, int index_get, int color);

int private_signal_traffic(int id, int index_traffic, int color);

void private_close_traffic(int id);

int create_pointer_traffic(CerradoSyn *self, const char *className, int initial_pointer);

int wait_traffic(MemoryShared memory, const char *className, int color);

int signal_traffic(MemoryShared memory, const char *className, int color);









