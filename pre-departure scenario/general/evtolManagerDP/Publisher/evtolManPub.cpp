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
#include <unordered_set>
#include "../utils/functions.h"

int ACE_TMAIN(int argc, ACE_TCHAR **argv)
{
  try
  {
    OpenDDS::Model::Application application(argc, argv);
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);

    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    DDS::DataWriter_var writer = model.writer(Elements::DataWriters::evtolAvailabilityDW_EV);

    UATM::availabilityInfoDataWriter_var writer_var = UATM::availabilityInfoDataWriter::_narrow(writer.in());

    if (CORBA::is_nil(writer_var.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                            ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }

    std::string filename = "evtolManagerDP/data/evtols.txt";
    std::unordered_set<std::string> sent_evtols;

    while (true)
    {
      std::vector<EVTOL> evtols = readEVTOLsFromFile(filename);
      OpenDDS::Model::WriterSync ws(writer);
      {
        if (evtols.empty())
        {
          // std::cout << "Todos os eVTOLs foram enviados!" << std::endl;
          break;
        }

        for (const auto &evtol : evtols)
        {
          if (sent_evtols.find(std::string(evtol.evtol_id)) == sent_evtols.end())
          {

            UATM::availabilityInfo bfr;

            bfr.resource_id = CORBA::string_dup(evtol.evtol_id.c_str());
            bfr.resource_type = "evtol";
            bfr.available = evtol.available;
            bfr.skyport_id = CORBA::string_dup(evtol.skyport_id.c_str());
            bfr.availability_time = CORBA::string_dup(getCurrentTime().c_str());

            DDS::ReturnCode_t error = writer_var->write(bfr, DDS::HANDLE_NIL);

            if (error != DDS::RETCODE_OK)
            {
              ACE_ERROR((LM_ERROR,
                         ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                             ACE_TEXT(" write returned %d!\n"),
                         error));
            }

            sent_evtols.insert(std::string(evtol.evtol_id));
            break;
          }
        }
      }
      std::this_thread::sleep_for(std::chrono::seconds(3));
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
