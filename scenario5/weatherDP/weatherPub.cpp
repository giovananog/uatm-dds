#ifdef ACE_AS_STATIC_LIBS
#include <C:/Users/ongio_1lak36v/Downloads/OpenDDS-3.29.1/dds/DCPS/transport/tcp/Tcp.h>
#endif
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include "UATMTraits.h"

#include <model/Sync.h>


int ACE_TMAIN(int argc, ACE_TCHAR **argv)
{
  try
  {
    OpenDDS::Model::Application application(argc, argv);
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);

    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    DDS::DataWriter_var writer_weather = model.writer(Elements::DataWriters::tolPadAvailabilityDW_TP);
    UATM::weatherInfoDataWriter_var writer_weather_var = UATM::weatherInfoDataWriter::_narrow(writer_weather.in());

    if (CORBA::is_nil(writer_weather_var.in())) {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" _narrow failed!\n")),
                         -1);
    }

    OpenDDS::Model::WriterSync ws(writer_weather);
    {
      UATM::weatherInfo wi;

      // Populate message and send
      wi.weather_id = 23;
      wi.location = "22";
      wi.temperature = 2.5;
      wi.wind_speed = 1.1;
      wi.weather_condition = "true";
      wi.observation_time = "true";

      DDS::ReturnCode_t error = writer_weather_var->write(wi, DDS::HANDLE_NIL);

      if (error != DDS::RETCODE_OK) {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                     ACE_TEXT(" write returned %d!\n"), error));
      }
  } catch (const CORBA::Exception& e) {
    e._tao_print_exception("Exception caught in main():");
    return -1;

  } catch( const std::exception& ex) {
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                      ACE_TEXT(" Exception caught: %C\n"),
                      ex.what()),
                     -1);
  }
  return 0;
}