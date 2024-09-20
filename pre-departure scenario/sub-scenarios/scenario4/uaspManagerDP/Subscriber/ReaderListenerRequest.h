#ifndef READER_LISTENER_REQUEST_H
#define READER_LISTENER_REQUEST_H
  
#include <tools/modeling/codegen/model/NullReaderListener.h>

#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include <vector>
#include "../../model/UATMTraits.h"
#include <dds/DCPS/WaitSet.h>


class ReaderListenerRequest : public OpenDDS::Model::NullReaderListener {
  public:
    ReaderListenerRequest(OpenDDS::Model::ReaderCondSync& rcs);
    virtual void on_data_available(DDS::DataReader_ptr reader);

    // static std::vector<UATM::availabilityInfo> storedAvailabilities;
  private:
    OpenDDS::Model::ReaderCondSync& rcs_;
    ACE_Thread_Mutex mutex_;
};

#endif // READER_LISTENER_REQUEST_H

