#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <chrono>
#include <dds/DCPS/transport/tcp/Tcp.h>
#include "../../model/UATMTraits.h"
#include "../utils/functions.h"
#include <model/Sync.h>


int ACE_TMAIN(int argc, ACE_TCHAR **argv)
{
  try
  {
    OpenDDS::Model::Application application(argc, argv);
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);

    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    DDS::DataWriter_var writer = model.writer(Elements::DataWriters::tolPadAvailabilityDW_TP);

    UATM::availabilityInfoDataWriter_var writer_var = UATM::availabilityInfoDataWriter::_narrow(writer.in());

    if (CORBA::is_nil(writer_var.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                            ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }

    OpenDDS::Model::WriterSync ws(writer);
    {
      while (true)
      {
        std::string filename = "tolPadManagerDP/data/tolpads.txt";
        std::vector<TolPad> tolPads = readTolPadsFromFile(filename);

        if (tolPads.empty())
        {
          // std::cout << "Todos os TolPads foram enviados!" << std::endl;
          break;
        }

        TolPad current_tolPad = tolPads.front();
        
        UATM::availabilityInfo bfr;

        bfr.resource_id = CORBA::string_dup(current_tolPad.resource_id.c_str());
        bfr.resource_type = "tolPad";
        bfr.available = current_tolPad.available;
        bfr.skyport_id = CORBA::string_dup(current_tolPad.skyport_id.c_str());
        bfr.availability_time = CORBA::string_dup(getCurrentTime().c_str());

        DDS::ReturnCode_t error = writer_var->write(bfr, DDS::HANDLE_NIL);

        if (error != DDS::RETCODE_OK)
        {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                         ACE_TEXT(" write returned %d!\n"),
                     error));
        }

        updateTolPadInFile(filename, current_tolPad.resource_id);

        std::this_thread::sleep_for(std::chrono::seconds(3));
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
