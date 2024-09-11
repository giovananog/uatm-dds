#include "./model/UATMTraits.h"

#include "dds/DCPS/transport/framework/TransportRegistry.h"
#include "dds/DCPS/transport/framework/TransportExceptions.h"
#include "dds/DCPS/transport/tcp/TcpInst.h"
#include "dds/DCPS/transport/tcp/TcpInst_rch.h"
#include "dds/DCPS/transport/multicast/MulticastInst.h"
#include "dds/DCPS/transport/multicast/MulticastInst_rch.h"
#include "dds/DCPS/transport/udp/UdpInst.h"
#include "dds/DCPS/transport/udp/UdpInst_rch.h"
#include <./model/TransportDirectives.h>

#include <stdexcept>
namespace UATM {
namespace uatmDCPS {
DefaultUATMTraits::DefaultUATMTraits() {

}

DefaultUATMTraits::~DefaultUATMTraits() { }

std::string 
DefaultUATMTraits::configName(const std::string& modeledName) const {
  std::string result;
  if (!modeledName.empty()) {
    result = std::string("UATM_default_") + modeledName;
  }
  return result;
}
}
}
