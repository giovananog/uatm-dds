#ifndef READER_LISTENER_WEATHER_H
#define READER_LISTENER_WEATHER_H

#include <tools/modeling/codegen/model/NullReaderListener.h>

#include <model/Sync.h>
#include <ace/Log_Msg.h>

#include <dds/DCPS/WaitSet.h>

class ReaderListenerWeather : public OpenDDS::Model::NullReaderListener {
  public:
    ReaderListenerWeather(OpenDDS::Model::ReaderCondSync& rcs);
    virtual void on_data_available(DDS::DataReader_ptr reader);
  private:
    OpenDDS::Model::ReaderCondSync& rcs_;
    ACE_Thread_Mutex mutex_;
};

#endif // READER_LISTENER_WEATHER_H

