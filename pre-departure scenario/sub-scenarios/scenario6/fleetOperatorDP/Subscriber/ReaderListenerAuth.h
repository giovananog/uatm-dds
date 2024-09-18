#ifndef READER_LISTENER_AUTH_H
#define READER_LISTENER_AUTH_H

#include < tools/modeling/codegen/model/NullReaderListener.h>

#include <model/Sync.h>
#include <ace/Log_Msg.h>

#include <dds/DCPS/WaitSet.h>


class ReaderListenerAuth : public OpenDDS::Model::NullReaderListener {
  public:
    ReaderListenerAuth(OpenDDS::Model::ReaderCondSync& rcs);
    virtual void on_data_available(DDS::DataReader_ptr reader);
  private:
    OpenDDS::Model::ReaderCondSync& rcs_;
    ACE_Thread_Mutex mutex_;
};

#endif // READER_LISTENER_AUTH_H


