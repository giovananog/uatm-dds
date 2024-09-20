#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h>
#endif
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include "../../model/UATMTraits.h"

std::string read_tolpad_id() {
    std::ifstream file("tolpad_id.txt");
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

    DDS::DataWriter_var writer_assign = model.writer(Elements::DataWriters::tolPadReqDW_UASP);
    UATM::tolPadRequestDataWriter_var writer_assign_var = UATM::tolPadRequestDataWriter::_narrow(writer_assign.in());
  

    if (CORBA::is_nil(writer_assign_var.in())) {
        ACE_ERROR_RETURN((LM_ERROR,
                          ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" _narrow failed!\n")),
                         -1);
    }

    OpenDDS::Model::WriterSync ws(writer_assign);
    {
      std::string tol_pad_id = read_tolpad_id();
            if (!tol_pad_id.empty()) {
                UATM::tolPadRequest tr;
                tr.assign_id = 23;
                tr.flight_id = 99;
                tr.tol_pad_id = std::stoi(tol_pad_id); // Converte o tol_pad_id para inteiro
                tr.assign_time = "1245-54"; // Altere conforme necessário

                DDS::ReturnCode_t error = writer_assign_var->write(tr, DDS::HANDLE_NIL);
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