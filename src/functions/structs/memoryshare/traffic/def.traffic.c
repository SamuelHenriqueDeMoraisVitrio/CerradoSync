
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../../imports/imports.dec.h"
//silver_chain_scope_end



CerradoSync_TrafficMemory *private_CerradoSync_new_TrafficMemory(key_t key){
  
  CerradoSync_TrafficMemory *self = (CerradoSync_TrafficMemory *)malloc(sizeof(CerradoSync_TrafficMemory));
  if(!private_CerradoSync_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->key = key;
  self->trafficID = private_CerradoSync_init_traffic(self->key);
  if(self->trafficID == -1){
    free(self);
    return NULL;
  }

  return self;
}

CerradoSync_TrafficPointerObject *private_CerradoSync_new_TrafficPointerObject(const char *className, int contTraffics, int initialPointer){

  if(!private_CerradoSync_free_interrupted((void *)className, NULL, 0)){
    return NULL;
  }

  CerradoSync_TrafficPointerObject *self = (CerradoSync_TrafficPointerObject *)malloc(sizeof(CerradoSync_TrafficPointerObject));
  if(!private_CerradoSync_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->nameClass = (const char *)malloc(strlen(className) + 1);
  if(!private_CerradoSync_free_interrupted((void *)self->nameClass, (void *[]){self}, 1)){
    return NULL;
  }

  self->nameClass = className;
  

  self->number_traffics = contTraffics;
  if(self->number_traffics < 1){
    private_CerradoSync_free_interrupted(NULL, (void *[]){(void *)self->nameClass, self}, 2);
    return NULL;
  }

  if((self->traffic_ID = private_CerradoSync_creat_a_wait_point(self->nameClass, initialPointer, self->number_traffics, &self->key)) == -1){
    private_CerradoSync_free_interrupted(NULL, (void *[]){(void *)self->nameClass, self}, 2);
    return NULL;
  }

  return self;

}

CerradoSync_TrafficPointersList *private_CerradoSync_new_TrafficPointersList(){

  CerradoSync_TrafficPointersList *self = (CerradoSync_TrafficPointersList *)calloc(1, sizeof(CerradoSync_TrafficPointersList));
  if(!private_CerradoSync_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->size_elements = 0;
  self->semID = (CerradoSync_TrafficPointerObject **)malloc(sizeof(CerradoSync_TrafficPointerObject *));
  if(!private_CerradoSync_free_interrupted(self->semID, (void *[]){self}, 1)){
    return NULL;
  }

  return self;
}

void private_CerradoSync_free_traffic(CerradoSync_TrafficMemory *self){
  if(self){
    private_CerradoSync_close_traffic(self->trafficID);
    free(self);
  }
}

void private_CerradoSync_free_TrafficPointerObject(CerradoSync_TrafficPointerObject *self){
  if(self){

    if(self->traffic_ID != -1){
      private_CerradoSync_close_traffic(self->traffic_ID);
    }

    free(self);
  }
}

void private_CerradoSync_free_TrafficPointersList(CerradoSync_TrafficPointersList *self){

  if(self){

    if(self->semID){
      for(int i = 0; i < self->size_elements; i++){
        private_CerradoSync_free_TrafficPointerObject(self->semID[i]);
      }
      free(self->semID);
    }
    
    free(self);
  }
}





