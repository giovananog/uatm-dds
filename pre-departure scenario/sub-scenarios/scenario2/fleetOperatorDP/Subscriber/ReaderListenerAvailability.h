#ifndef READER_LISTENER_AVAILABILITY_H
#define READER_LISTENER_AVAILABILITY_H

#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include <dds/DCPS/WaitSet.h>
#include <tools/modeling/codegen/model/NullReaderListener.h>


class ReaderListenerAvailability : public OpenDDS::Model::NullReaderListener {
  public:
    ReaderListenerAvailability(OpenDDS::Model::ReaderCondSync& rcs);
    virtual void on_data_available(DDS::DataReader_ptr reader);
  private:
    OpenDDS::Model::ReaderCondSync& rcs_;
    ACE_Thread_Mutex mutex_;
};

#endif // READER_LISTENER_AVAILABILITY_H
