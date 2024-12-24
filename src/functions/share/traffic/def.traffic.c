
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../imports/imports.dec.h"
//silver_chain_scope_end




int private_CerradoSync_get_stats_traffic(int id, int index_get){
  return semctl(id, index_get, GETVAL);
}

int private_CerradoSync_init_traffic(key_t key){

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

int private_CerradoSync_creat_a_wait_point(const char *className, int initial, int number_traffics, key_t *key){
  *key = private_CerradoSync_creat_key(className);

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

int private_CerradoSync_wait(key_t key, int number_traffics, int index_get, int color){

  int sem_point = semget(key, number_traffics, 0);
  if(sem_point == -1){
    return -1;
  }

  int point_stats = private_CerradoSync_get_stats_traffic(sem_point, index_get);
  if(point_stats == -1){
    return -1;
  }

  if(color == GREEN_TRAFFIC || color == RED_TRAFFIC){
    
    do{

      point_stats = private_CerradoSync_get_stats_traffic(sem_point, index_get);
      if(point_stats == -1){
        return -1;
      }
      usleep(1000);
      continue;
    
    }while((color == GREEN_TRAFFIC && point_stats == 0) || (color == RED_TRAFFIC && point_stats > 0));

    return 1;

  }

  return 0;
}

int private_CerradoSync_signal_traffic(int id, int index_traffic, int color){

  struct sembuf operation = {index_traffic, color, 0};

  if(semop(id, &operation, 1) == -1){
    return -1;
  }

  return id;
}

void private_CerradoSync_close_traffic(int id){
  semctl(id, 0, IPC_RMID);
}

int CerradoSync_create_pointer_traffic(CerradoSync *self, const char *className, int initial_pointer){
  
  int result = -1;
  int number_of_traffics = 1;
  key_t key;

  if(initial_pointer <= 0){
    initial_pointer = RED_TRAFFIC;
  }
  if(initial_pointer > 0){
    initial_pointer = GREEN_TRAFFIC;
  }

  if((result = private_CerradoSync_creat_a_wait_point(className, initial_pointer, number_of_traffics, &key)) == -1){
    return -1;
  }

  CerradoSync_TrafficPointersList *objs_traffic = self->memory->traffic;
  objs_traffic->semID = (CerradoSync_TrafficPointerObject **)realloc(objs_traffic->semID, sizeof(CerradoSync_TrafficPointerObject *) * (objs_traffic->size_elements + 1));
  if(objs_traffic->semID == NULL){
    return -1;
  }

  CerradoSync_TrafficPointerObject *obj_traffic = (CerradoSync_TrafficPointerObject *)malloc(sizeof(CerradoSync_TrafficPointerObject));
  if(obj_traffic == NULL){
    return -1;
  }

  obj_traffic->traffic_ID = result;
  obj_traffic->number_traffics = number_of_traffics;
  obj_traffic->key = key;
  obj_traffic->nameClass = className;

  objs_traffic->semID[objs_traffic->size_elements] = obj_traffic;

  objs_traffic->size_elements++;

  return 1;
}

int CerradoSync_wait_traffic(CerradoSync_MemoryShared *memory, const char *className, int color){

  key_t key = private_CerradoSync_generate_string_key(className, memory->pid);
  if(!key){
    return -1;
  }

  if(private_CerradoSync_wait(key, 1, 0, color) == -1){
    return -1;
  }

  return 1;
}

int CerradoSync_signal_traffic(CerradoSync_MemoryShared *memory, const char *className, int color){

  key_t key = private_CerradoSync_generate_string_key(className, memory->pid);
  if(!key){
    return -1;
  }

  int sem_point = semget(key, 1, 0);
  if(sem_point == -1){
    return -1;
  }

  int result = -1;
  if((result = private_CerradoSync_get_stats_traffic(sem_point, 0)) == -1){
    return -1;
  }

  if(color == GREEN_TRAFFIC){
    if(result <= 0){
      private_CerradoSync_signal_traffic(sem_point, 0, 1);
      return 1;
    }
  }
  if(color == RED_TRAFFIC){
    if(result > 0){
      private_CerradoSync_signal_traffic(sem_point, 0, -1);
      return 1;
    }
  }

  return 0;
}





