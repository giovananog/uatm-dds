#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h>
#endif
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include "../../model/UATMTraits.h"

std::string read_skyport_id() {
    std::ifstream file("skyport_id.txt");
    std::string tol_pad_id;
    if (file.is_open()) {
        std::getline(file, tol_pad_id);
        file.close();
    } else {
        ACE_ERROR((LM_ERROR, ACE_TEXT("ERROR - file not open\n")));
    }
    return tol_pad_id;
}

int ACE_TMAIN(int argc, ACE_TCHAR **argv)
{
  try
  {
    OpenDDS::Model::Application application(argc, argv);
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);

    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    DDS::DataWriter_var writer = model.writer(Elements::DataWriters::assignFlightDW_FOP);

    UATM::flightAssignDataWriter_var writer_var = UATM::flightAssignDataWriter::_narrow(writer.in());

    if (CORBA::is_nil(writer_var.in())) {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" _narrow failed!\n")),
                         -1);
    } 

    OpenDDS::Model::WriterSync ws(writer);
    {
      std::string skyport_id = read_skyport_id();
      if (!skyport_id.empty()) {
      UATM::flightAssign fa;

      // Populate message and send
      fa.flight_assign_id = 3232;
      fa.assign_time = "1245-32";
      fa.operator_id =  std::stoi(skyport_id);
      fa.assign_status = true;
      UATM::ArrayString resources_id;

      fa.resources_id[0] = "2323";
      fa.resources_id[1] = "3232";

      DDS::ReturnCode_t error = writer_var->write(fa, DDS::HANDLE_NIL);

      if (error != DDS::RETCODE_OK) {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                     ACE_TEXT(" write returned %d!\n"), error));
      }
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