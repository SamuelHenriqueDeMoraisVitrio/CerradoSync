
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end




int private_get_stats_traffic(key_t key, int number_traffics, int index_get){
  int sem_point = semget(key, number_traffics, 0);
  if(sem_point == -1){
    return -1;
  }

  return semctl(sem_point, index_get, GETVAL);
}

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

int private_creat_a_wait_point(const char *className, int initial, int number_traffics, key_t *key){
  *key = private_creat_key(className);

  int sem_point = semget(*key, 1, IPC_CREAT | IPC_EXCL | 0666);
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

int private_wait(key_t key, int number_traffics, int index_get, int color){

  int point_stats = private_get_stats_traffic(key, number_traffics, index_get);
  if(point_stats == -1){
    return -1;
  }

  if(color == GREEN_TRAFFIC){
    while(point_stats != 0){
      continue;
    }
  }
  if(color == RED_TRAFFIC){
    while(point_stats == 0){
      continue;
    }
  }

  return 1;
}

int private_signal_traffic(key_t key, int number_traffics, int color){

  int sem_point = semget(key, number_traffics, 0);
  if(sem_point == -1){
    return -1;
  }

  struct sembuf operation = {0, (color>0?(GREEN_TRAFFIC):(RED_TRAFFIC - 1)), 0};

  if(semop(sem_point, &operation, 1) == -1){
    return -1;
  }

  return sem_point;
}

void private_close_traffic(key_t key){
  semctl(key, 0, IPC_RMID);
}

int create_pointer_traffic(CerradoSyn *self, const char *className, int initial_pointer){
  
  int result = -1;
  int number_of_traffics = 1;
  key_t key;

  if((result = private_creat_a_wait_point(className, initial_pointer, number_of_traffics, &key)) == -1){
    return -1;
  }

  TrafficPointersList *objs_traffic = self->memory->traffic;
  objs_traffic->semID = (TrafficPointerObject **)realloc(objs_traffic->semID, sizeof(TrafficPointerObject *) * (objs_traffic->size_elements + 1));
  if(objs_traffic->semID == NULL){
    return -1;
  }

  TrafficPointerObject *obj_traffic = (TrafficPointerObject *)malloc(sizeof(TrafficPointerObject) + 1);
  if(obj_traffic == NULL){
    return -1;
  }

  obj_traffic->traffic_ID = result;
  obj_traffic->number_traffics = number_of_traffics;
  obj_traffic->key = key;
  obj_traffic->nameClass = className;

  objs_traffic->semID[objs_traffic->size_elements - 1] = obj_traffic;

  objs_traffic->size_elements++;

  return 1;
}

int wait_traffic(MemoryShared memory, const char *className, int color){

  key_t key = private_generate_string_key(className, memory.pid);
  if(!key){
    return -1;
  }

  if(private_wait(key, 1, 0, color) == -1){
    return -1;
  }

  return 1;
}

int signal_traffic(MemoryShared memory, const char *className, int color){

  key_t key = private_generate_string_key(className, memory.pid);
  if(!key){
    return -1;
  }

  int result = -1;
  if((result = private_get_stats_traffic(key, 1, 0)) == -1){
    return -1;
  }

  int expected = -1;
  if(color == GREEN_TRAFFIC){
    if(result == 0){
      private_signal_traffic(key, 1, GREEN_TRAFFIC);
      return 1;
    }
  }
  if(color == RED_TRAFFIC){
    if(result > 0){
      private_signal_traffic(key, 1, RED_TRAFFIC);
      return 1;
    }
  }

  return 0;
}






