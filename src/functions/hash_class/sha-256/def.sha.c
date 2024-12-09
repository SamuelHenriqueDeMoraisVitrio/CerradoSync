
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
#include <unistd.h>
//silver_chain_scope_end



const char *private_generate_string_key(const char *key, pid_t hierarchy){
  if(key == NULL){
    perror("\n\tKey passada incorreta;\n");
    return 0;
  }

  short size_pid = 15;
  long size_key_formated = strlen(key) + size_pid + 3;

  char key_hash[size_key_formated];
  snprintf(key_hash, size_key_formated, "%s-%d", key, hierarchy);

  uint8_t hash[SIZE_OF_SHA_256_HASH];
  calc_sha_256(hash, key_hash, strlen(key_hash));

  char *hash_string = (char*)malloc(SIZE_OF_SHA_256_HASH * 2 + 1);
  for (unsigned int i = 0; i < SIZE_OF_SHA_256_HASH; i++) {
      sprintf(hash_string + i * 2, "%02x", hash[i]);
  }

  return hash_string;
}

const char *private_creat_key(const char *key){
  return private_generate_string_key(key, getpid());
}

const char *private_get_key(const char *key){
  return private_generate_string_key(key, getppid());
}





