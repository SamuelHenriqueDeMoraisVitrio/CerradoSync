
#include "../../CerradoSyn.h"
#include <cstdint>
#include <sched.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dependencies/sha256.c"
#include "dependencies/sha256.h"


typedef struct Sha_256 Hash;

unsigned long get_key(void *key){

  pid_t pid_process_self = getpid();

  unsigned char pid_process_converted[sizeof(pid_process_self)];
  memccpy(pid_process_converted, &pid_process_self, sizeof(pid_process_self));

  Hash sha;
  uint8_t hash[SIZE_OF_SHA_256_HASH];
  sha_256_init(&sha, hash);
  pid_t pid_now = getpid();
  sha_256_write(&sha, , 0);
  
  return NULL;
}



int main(){
  

  CerradoSyn *Processo_main = new_CerradoSynStruct("main");

  //const char *ID = sprintf(ID, "%02X", "main");
  //ArgumentCallback *argument_id_shared = new_argument("memory_ID", 22, sizeof((void *)22));

  printf("%ld", (long)"main");

  return 0;
}




