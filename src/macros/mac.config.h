
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.depH.h"
//silver_chain_scope_end



#define _SIZE_STACK_PROCESS_1MB_ 1024 * 1024 // 1mb

#define _DEFAULT_MAX_SIZE_TRAFFIC_ 1024 // 1kb

#define _WRITE_AND_READ_ 0666 // Read and write permission

#define _CONFIG_SHMGET_PERMISSIONS_ _WRITE_AND_READ_ | IPC_CREAT | IPC_EXCL // shmget configuration function

