
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.typesH.h"
//silver_chain_scope_end





int private_init_traffic(key_t key);

int private_creat_a_wait_point(const char *className, int initial, int number_traffics);

int private_wait_to_green(key_t key, int number_traffics, int number_get);

int private_wait_to_red(key_t key, int number_traffics, int number_get);

int private_signal_green(key_t key, int number_traffics);

int private_signal_red(key_t key, int number_traffics);




