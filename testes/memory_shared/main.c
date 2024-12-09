
#include "../../CerradoSyn.h"

#include "dependencies/sha256.h"
#include "dependencies/sha256.c"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


typedef struct Sha_256 Hash;

const char *get_key(const char *key){
  if(key == NULL){
    perror("\n\tKey passada incorreta;\n");
    return 0;
  }

  short size_pid = 15;
  long size_key_formated = strlen(key) + size_pid + 3;

  char key_hash[size_key_formated]; //= (char *)malloc(size_key_formated);
  snprintf(key_hash, size_key_formated, "%s-%d", key, getpid());

  uint8_t hash[SIZE_OF_SHA_256_HASH];
  calc_sha_256(hash, key_hash, strlen(key_hash));

  //free(key_hash);
  
  char *hash_string = (char*)malloc(SIZE_OF_SHA_256_HASH * 2 + 1);
  for (unsigned int i = 0; i < SIZE_OF_SHA_256_HASH; i++) {
      sprintf(hash_string + i * 2, "%02x", hash[i]);
  }

  return hash_string;
}



int main(){

  const char *class = "Meu nome é samuel henrique de morais vitrio";
  const char *class2 = "Meu nome é samuel henrique de morais vitrio";
  const char *key = get_key(class);
  const char *key2 = get_key(class2);

  printf("\n\tkeys:\n%s\n%s\n", key, key2);

  if(strcmp(key, key2) == 0){
    printf("\n\tIguais\n\n");
    return 0;
  }

  printf("\n\tDiferente\n\n");

  return 0;
}




