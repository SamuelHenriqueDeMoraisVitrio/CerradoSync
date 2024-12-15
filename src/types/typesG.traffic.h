
//silver_chain_scope_start
//mannaged by silver chain
#include "../imports/imports.typesF.h"
//silver_chain_scope_end

struct TrafficPointerObject_struct{
  int traffic_ID;
  int number_traffics;
  key_t key;
  const char *nameClass;
};

struct TrafficPointersList_struct{
  int size_elements;
  TrafficPointerObject **semID;
};

struct TrafficMemory_struct{
  int trafficID;
  key_t key;
};




