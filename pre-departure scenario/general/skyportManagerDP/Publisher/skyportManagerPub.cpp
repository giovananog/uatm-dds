#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <chrono>
#include <dds/DCPS/transport/tcp/Tcp.h>
#include "../../model/UATMTraits.h"
#include <model/Sync.h>
#include "../utils/functions.h"


int ACE_TMAIN(int argc, ACE_TCHAR **argv)
{
  try
  {
    OpenDDS::Model::Application application(argc, argv);
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);

    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    DDS::DataWriter_var writer = model.writer(Elements::DataWriters::skyportAvailabilityDW_SKM);

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
        std::string filename = "skyportManagerDP/data/skyports.txt";
        std::vector<Skyport> skyports = readSkyportsFromFile(filename);

        if (skyports.empty())
        {
          std::cout << "Todos os Skyports foram enviados!" << std::endl;
          break;
        }

        Skyport current_skyport = skyports.front();

        UATM::availabilityInfo fa;

        fa.resource_id = CORBA::string_dup(current_skyport.resource_id.c_str());
        fa.resource_type = "skyport";
        fa.available = current_skyport.available;
        fa.skyport_id = CORBA::string_dup(current_skyport.resource_id.c_str());
        fa.availability_time = "time";

        DDS::ReturnCode_t error = writer_var->write(fa, DDS::HANDLE_NIL);

        if (error != DDS::RETCODE_OK)
        {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                         ACE_TEXT(" write returned %d!\n"),
                     error));
        }

        updateSkyportInFile(filename, current_skyport.resource_id);

        std::this_thread::sleep_for(std::chrono::seconds(15));
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
