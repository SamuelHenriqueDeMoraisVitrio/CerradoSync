
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end



key_t private_generate_string_key(const char *key, pid_t hierarchy){
  if(key == NULL || strlen(key) > 0 || hierarchy > 0){
    return 0;
  }

  short size_pid = 15;
  long size_key_formated = strlen(key) + size_pid + 3;

  char key_hash[size_key_formated];
  snprintf(key_hash, size_key_formated, "%s-%d", key, hierarchy);

  uint8_t hash[SIZE_OF_SHA_256_HASH];
  calc_sha_256(hash, key_hash, strlen(key_hash));

  key_t numeric_key = 0;
  for (int i = 0; i < SIZE_OF_SHA_256_HASH; i++) {
    numeric_key = numeric_key ^ (hash[i] << ((i % sizeof(key_t)) * 8));
  }

  return numeric_key;
}

key_t private_creat_key(const char *key){
  return private_generate_string_key(key, getpid());
}

key_t private_get_key(const char *key){
  return private_generate_string_key(key, getppid());
}





