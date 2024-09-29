#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h>
#endif
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include "../../model/UATMTraits.h"
#include "../utils/functions.h"

#include <unordered_set> 
#include <sstream>       
#include <fstream>
#include <thread>

int ACE_TMAIN(int argc, ACE_TCHAR **argv)
{
  try
  {
    OpenDDS::Model::Application application(argc, argv);
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);

    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    {
      while (true)
      {
        DDS::DataWriter_var writer_assign = model.writer(Elements::DataWriters::tolPadReqDW_UASP);
        UATM::tolPadRequestDataWriter_var writer_assign_var = UATM::tolPadRequestDataWriter::_narrow(writer_assign.in());

        if (CORBA::is_nil(writer_assign_var.in()))
        {
          ACE_ERROR_RETURN((LM_ERROR,
                            ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                                ACE_TEXT(" _narrow failed!\n")),
                           -1);
        }

        std::string tolPadID;
        std::string resourceFile = "uaspManagerDP/data/tolpads.txt";

        if (checkAvailability(resourceFile, tolPadID))
        { 
          OpenDDS::Model::WriterSync ws2(writer_assign);
          {
            UATM::tolPadRequest tr;
            tr.assign_id = 1;
            tr.flight_id = 2;
            tr.tol_pad_id = tolPadID.c_str();
            tr.assign_time = "13212";

            DDS::ReturnCode_t error = writer_assign_var->write(tr, DDS::HANDLE_NIL);
            if (error != DDS::RETCODE_OK)
            {
              ACE_ERROR((LM_ERROR,
                         ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                             ACE_TEXT(" write returned %d!\n"),
                         error));
            }
          }
        }
        std::this_thread::sleep_for(std::chrono::seconds(4));
      }
    }
  }
  catch (const CORBA::Exception &e)
  {
    e._tao_print_exception("Exception caught in main():");
    return -1;
  }
  catch (const std::exception &ex)
  {
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" Exception caught: %C\n"),
                      ex.what()),
                     -1);
  }
  return 0;
}