#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h>
#endif
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include "../../model/UATMTraits.h"


int ACE_TMAIN(int argc, ACE_TCHAR **argv)
{
  try
  {
    OpenDDS::Model::Application application(argc, argv);
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);
    UATM::uatmDCPS::DefaultUATMType model2(application, argc, argv);
    UATM::uatmDCPS::DefaultUATMType model3(application, argc, argv);

    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    DDS::DataWriter_var writer = model.writer(Elements::DataWriters::flightRoutesDW_SKO);

    UATM::flightRoutesInfoDataWriter_var writer_var = UATM::flightRoutesInfoDataWriter::_narrow(writer.in());

    if (CORBA::is_nil(writer_var.in())) {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" _narrow failed!\n")),
                         -1);
    }

    OpenDDS::Model::WriterSync ws(writer);
    {
      UATM::flightRoutesInfo fr;

      fr.flight_route_id = 1;
      UATM::ArrayString waypoints;
      fr.flight_id = 2;
      fr.estimated_time = "estimated_time";
      fr.status = true;

      DDS::ReturnCode_t error = writer_var->write(fr, DDS::HANDLE_NIL);

      if (error != DDS::RETCODE_OK) {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                     ACE_TEXT(" wribte returned %d!\n"), error));
      }
    }

    DDS::DataWriter_var writer_flows = model2.writer(Elements::DataWriters::trafficFlowsDW_SKO);
    UATM::trafficFlowsInfoDataWriter_var writer_flows_var = UATM::trafficFlowsInfoDataWriter::_narrow(writer_flows.in());

    if (CORBA::is_nil(writer_flows_var.in())) {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" _narrow failed!\n")),
                         -1);
    }

    OpenDDS::Model::WriterSync ws2(writer_flows);
    {
      UATM::trafficFlowsInfo fc;

      fc.flows_id = 1;
      fc.area = "area";
      fc.congestion_level = "congestion_level";
      UATM::ArrayString affected_routes;
      fc.timestamp = "timestamp";

      DDS::ReturnCode_t error = writer_flows_var->write(fc, DDS::HANDLE_NIL);

      if (error != DDS::RETCODE_OK) {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                     ACE_TEXT(" write returned %d!\n"), error));
      }
    }
    
    DDS::DataWriter_var writer_rest = model3.writer(Elements::DataWriters::airspaceRestDW_SKO);
    UATM::airspaceRestrictionsDataWriter_var writer_rest_var = UATM::airspaceRestrictionsDataWriter::_narrow(writer_rest.in());

    if (CORBA::is_nil(writer_rest_var.in())) {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" _narrow failed!\n")),
                         -1);
      }

    OpenDDS::Model::WriterSync ws3(writer_rest);
    {
      UATM::airspaceRestrictions fr;

      fr.restriction_id = 1;
      fr.area = "area";
      fr.restriction_type = "restriction_type";
      fr.start_time = "start_time";
      fr.end_time = "end_time";
      fr.authority = "authority";

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