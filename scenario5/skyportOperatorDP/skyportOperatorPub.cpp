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

    DDS::DataWriter_var writer_rest = model.writer(Elements::DataWriters::airspaceRestDW_SKO);
    UATM::trafficFlowsInfoDataWriter_var writer_rest_var = UATM::trafficFlowsInfoDataWriter::_narrow(writer.in());

    if (CORBA::is_nil(writer_flows_var.in())) {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" _narrow failed!\n")),
                         -1);
    }

    OpenDDS::Model::WriterSync ws(writer_flows);
    {
      UATM::trafficFlowsInfo fc;

      // Populate message and send
      fc.flows_id = 23;
      fc.area = "22";
      fc.congestion_level = "343434";
      fc.affected_routes = "343434";
      fc.timestamp = "343434";

      DDS::ReturnCode_t error = writer_flows_var->write(fc, DDS::HANDLE_NIL);

      if (error != DDS::RETCODE_OK) {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                     ACE_TEXT(" write returned %d!\n"), error));
      }
    
    DDS::DataWriter_var writer_rest = model.writer(Elements::DataWriters::airspaceRestDW_SKO);
    UATM::asirspaceRestrictionsDataWriter_var writer_rest_var = UATM::asirspaceRestrictionsDataWriter::_narrow(writer.in());

    if (CORBA::is_nil(writer_rest_var.in())) {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" _narrow failed!\n")),
                         -1);
      }
    }

    OpenDDS::Model::WriterSync ws(writer_rest);
    {
      UATM::asirspaceRestrictions fr;

      fc.restriction_id = 23;
      fc.area = "22";
      fc.restriction_type = "343434";
      fc.start_time = "343434";
      fc.end_time = "343434";
      fc.authority = "343434";

      DDS::ReturnCode_t error = writer_rest_var->write(fr, DDS::HANDLE_NIL);

      if (error != DDS::RETCODE_OK) {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                     ACE_TEXT(" write returned %d!\n"), error));
      }
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