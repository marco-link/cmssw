#ifndef EcalTPGOddWeightIdMap_h
#define EcalTPGOddWeightIdMap_h

#include "CondFormats/Serialization/interface/Serializable.h"

#include <map>
#include "CondFormats/EcalObjects/interface/EcalTPGWeights.h"
#include <cstdint>

class EcalTPGOddWeightIdMap {
public:
  typedef std::map<uint32_t, EcalTPGWeights> EcalTPGWeightMap;
  typedef std::map<uint32_t, EcalTPGWeights>::const_iterator EcalTPGWeightMapItr;

  EcalTPGOddWeightIdMap();
  ~EcalTPGOddWeightIdMap();

  const EcalTPGWeightMap& getMap() const { return map_; }
  void setValue(const uint32_t& id, const EcalTPGWeights& value);

private:
  EcalTPGWeightMap map_;

  COND_SERIALIZABLE;
};

#endif
