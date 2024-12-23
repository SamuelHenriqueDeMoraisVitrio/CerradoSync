
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.typesF.h"
//silver_chain_scope_end

struct private_CerradoSync_TrafficPointerObject_struct{
  int traffic_ID;
  int number_traffics;
  key_t key;
  const char *nameClass;
};

struct private_CerradoSync_TrafficPointersList_struct{
  int size_elements;
  CerradoSync_TrafficPointerObject **semID;
};

struct private_CerradoSync_TrafficMemory_struct{
  int trafficID;
  key_t key;
};




