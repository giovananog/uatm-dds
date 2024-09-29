#ifndef READER_LISTENER_AVAILABILITY_H
#define READER_LISTENER_AVAILABILITY_H

#include <tools/modeling/codegen/model/NullReaderListener.h>
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include <vector>
#include <dds/DCPS/WaitSet.h>

class ReaderListenerAvailability : public OpenDDS::Model::NullReaderListener {
  public:
    ReaderListenerAvailability(OpenDDS::Model::ReaderCondSync& rcs);
    virtual void on_data_available(DDS::DataReader_ptr reader);

    void ReaderListenerAvailability::updateAvailabilityFile(const UATM::availabilityInfo& msg);
  private:
    OpenDDS::Model::ReaderCondSync& rcs_;
    ACE_Thread_Mutex mutex_;
};

#endif // READER_LISTENER_AVAILABILITY_H
