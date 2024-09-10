#ifndef UATM_TRAITS_H
#define UATM_TRAITS_H
#include "UATM_T.h"
#include <model/TransportDirectives.h>

namespace UATM {
namespace uatmDCPS {

  struct UATM_Export DefaultUATMTraits {
    DefaultUATMTraits();
    virtual ~DefaultUATMTraits();
    std::string configName(const std::string& modeledName) const;
  };

  typedef OpenDDS::Model::Service< OpenDDS::Model::UATM::uatmDCPS::Elements, DefaultUATMTraits> DefaultUATMType;
}
}

#endif
