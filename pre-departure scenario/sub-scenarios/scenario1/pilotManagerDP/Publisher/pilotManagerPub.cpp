#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h>
#endif
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include "../../model/UATMTraits.h"
#include <dds/DCPS/WaitSet.h>


int ACE_TMAIN(int argc, ACE_TCHAR **argv)
{
  try
  {
    OpenDDS::Model::Application application(argc, argv);
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);

    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    DDS::DataWriter_var writer = model.writer(Elements::DataWriters::pilotAvailabilityDW_PLM);

    UATM::availabilityInfoDataWriter_var writer_var = UATM::availabilityInfoDataWriter::_narrow(writer.in());

    if (CORBA::is_nil(writer_var.in())) {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" _narrow failed!\n")),
                         -1);
    }

    // Block until Subscriber is available
    DDS::StatusCondition_var condition = writer->get_statuscondition();
    condition->set_enabled_statuses(DDS::PUBLICATION_MATCHED_STATUS);

    DDS::WaitSet_var ws = new DDS::WaitSet;
    ws->attach_condition(condition);

    ACE_DEBUG((LM_DEBUG,
               ACE_TEXT("Block until subscriber is available\n")));

    while (true) {
      DDS::PublicationMatchedStatus matches;
      if (writer->get_publication_matched_status(matches) != ::DDS::RETCODE_OK) {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("ERROR: %N:%l: main() -")
                          ACE_TEXT(" get_publication_matched_status failed!\n")),
                         1);
      }

      if (matches.current_count >= 1) {
        break;
      }

      DDS::ConditionSeq conditions;
      DDS::Duration_t timeout = { 60, 0 };
      if (ws->wait(conditions, timeout) != DDS::RETCODE_OK) {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("ERROR: %N:%l: main() -")
                          ACE_TEXT(" wait failed!\n")),
                         1);
      }
    }

    ACE_DEBUG((LM_DEBUG,
               ACE_TEXT("Subscriber is available\n")));

    ws->detach_condition(condition);

    // OpenDDS::Model::WriterSync ws(writer);
    // {
      UATM::availabilityInfo bfr;

      // Populate message and send
      bfr.resource_id = 3;
      bfr.resource_type = "pilot";
      bfr.status = true;
      bfr.location = "location";
      bfr.availability_time = "323123-323";

      DDS::ReturnCode_t error = writer_var->write(bfr, DDS::HANDLE_NIL);

      if (error != DDS::RETCODE_OK) {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                     ACE_TEXT(" write returned %d!\n"), error));
      }

      UATM::availabilityInfo bfr2;

      // Populate message and send
      bfr2.resource_id = 4;
      bfr2.resource_type = "not-pilot";
      bfr2.status = true;
      bfr2.location = "location";
      bfr2.availability_time = "323123-323";

      DDS::ReturnCode_t error2 = writer_var->write(bfr2, DDS::HANDLE_NIL);

      if (error2 != DDS::RETCODE_OK) {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                     ACE_TEXT(" write returned %d!\n"), error));
      }
    // }
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