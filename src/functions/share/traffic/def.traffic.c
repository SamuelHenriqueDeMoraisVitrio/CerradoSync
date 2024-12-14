
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
#include <cstdlib>
//silver_chain_scope_end





int private_init_traffic(key_t key){

  int sem_share = semget(key, 2, IPC_CREAT | IPC_EXCL | 0666);

  if(sem_share == -1){
    return sem_share;
  }

  if(semctl(sem_share, 0, SETVAL, 1) == -1){// The first set of traffic lights will be dedicated to "memory share";
    return -1;
  }

  if(semctl(sem_share, 1, SETVAL, 0) == -1){// The second set of traffic lights will be dedicated to the "await sync";
    return -1;
  }
  
  return sem_share;
}

int private_creat_a_wait_point(const char *className, int initial, int number_traffics){
  key_t key = private_creat_key(className);

  int sem_point = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
  if(sem_point == -1){
    return -1;
  }

  for(int i = 0; i < number_traffics; i++){
    if(semctl(sem_point, i, SETVAL, initial) == -1){
      return -1;
    }
  }

  return sem_point;
}

int private_wait_to_green(key_t key, int number_traffics, int number_get){

  int sem_point = semget(key, number_traffics, 0);
  if(sem_point == -1){
    return -1;
  }

  int point_stats = semctl(sem_point, number_get, GETVAL);
  if(point_stats == -1){
    return -1;
  }

  while(point_stats != 0){
    continue;
  }

  return sem_point;
}

int private_wait_to_red(key_t key, int number_traffics, int number_get){

  int sem_point = semget(key, number_traffics, 0);
  if(sem_point == -1){
    return -1;
  }

  int point_stats = semctl(sem_point, 0, GETVAL);
  if(point_stats == -1){
    return -1;
  }

  while(point_stats == 0){
    continue;
  }

  return sem_point;
}

int private_signal_green(key_t key, int number_traffics){

  int sem_point = semget(key, number_traffics, 0);
  if(sem_point == -1){
    return -1;
  }

  struct sembuf operation = {0, 1, 0};

  if(semop(sem_point, &operation, 1) == -1){
    return -1;
  }

  return sem_point;
}

int private_signal_red(key_t key, int number_traffics){

  int sem_point = semget(key, number_traffics, 0);
  if(sem_point == -1){
    return -1;
  }

  struct sembuf operation = {0, -1, 0};

  if(semop(sem_point, &operation, 1) == -1){
    return -1;
  }

  return sem_point;
}

void private_close_traffic(key_t key){
  semctl(key, 0, IPC_RMID);
}

int create_point_traffic(CerradoSyn *self, const char *className, int initial_pointer){
  
  int result = -1;

  if((result = private_creat_a_wait_point(className, initial_pointer, 1)) == -1){
    return -1;
  }

  TrafficPointersList *objs_traffic = self->memory->traffic;
  objs_traffic->semID = (TrafficPointerObject **)realloc(objs_traffic->semID, sizeof(TrafficPointerObject *) * (objs_traffic->size_elements + 1));

  if(objs_traffic->semID == NULL){
    return -1;
  }

  TrafficPointerObject *obj_traffic = objs_traffic->semID[0];

  obj_traffic->size_elements++;


}



