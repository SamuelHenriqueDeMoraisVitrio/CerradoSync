
//silver_chain_scope_start
//mannaged by silver chain
#include "../../../../imports/imports.dec.h"
//silver_chain_scope_end



TrafficMemory *private_new_TrafficMemory(key_t key){
  
  TrafficMemory *self = (TrafficMemory *)malloc(sizeof(TrafficMemory));
  if(!private_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->key = key;
  self->trafficID = private_init_traffic(self->key);
  if(self->trafficID == -1){
    free(self);
    return NULL;
  }

  return self;
}

TrafficPointerObject *private_new_TrafficPointerObject(const char *className, int contTraffics, int initialPointer){

  if(!private_free_interrupted((void *)className, NULL, 0)){
    return NULL;
  }

  TrafficPointerObject *self = (TrafficPointerObject *)malloc(sizeof(TrafficPointerObject) + 1);
  if(!private_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->nameClass = (const char *)malloc(strlen(className) + 1);
  if(!private_free_interrupted((void *)self->nameClass, (void *[]){self}, 1)){
    return NULL;
  }

  self->nameClass = className;
  

  self->number_traffics = contTraffics;
  if(self->number_traffics < 1){
    private_free_interrupted(NULL, (void *[]){(void *)self->nameClass, self}, 2);
    return NULL;
  }

  if((self->traffic_ID = private_creat_a_wait_point(self->nameClass, initialPointer, self->number_traffics, &self->key)) == -1){
    private_free_interrupted(NULL, (void *[]){(void *)self->nameClass, self}, 2);
    return NULL;
  }

  return self;

}

TrafficPointersList *private_new_TrafficPointersList(){

  TrafficPointersList *self = (TrafficPointersList *)malloc(sizeof(TrafficPointersList) + 1);
  if(!private_free_interrupted(self, NULL, 0)){
    return NULL;
  }

  self->size_elements = 0;
  self->semID = (TrafficPointerObject **)malloc(sizeof(TrafficPointerObject *));
  if(!private_free_interrupted(self->semID, (void *[]){self}, 1)){
    return NULL;
  }

  return self;
}

void private_free_traffic(TrafficMemory *self){
  if(self){
    private_close_traffic(self->key);
    free(self);
  }
}

void private_free_TrafficPointerObject(TrafficPointerObject *self){
  if(self){

    if(self->nameClass){
      free((void *)self->nameClass);
    }

    if(self->traffic_ID != -1){
      private_close_traffic(self->traffic_ID);
    }

    free(self);
  }
}

void private_free_TrafficPointersList(TrafficPointersList *self){

  if(self){

    if(self->semID){
      free(self->semID);
    }
    
    free(self);
  }
}





