#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>
#ifdef ACE_AS_STATIC_LIBS
#include <dds/DCPS/transport/tcp/Tcp.h>
#endif
#include <model/Sync.h>
#include <ace/Log_Msg.h>
#include <thread>
#include "../../model/UATMTraits.h"
#include "../utils/functions.h"

int ACE_TMAIN(int argc, ACE_TCHAR **argv)
{
  try
  {
    OpenDDS::Model::Application application(argc, argv);
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);

    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    DDS::DataWriter_var writer = model.writer(Elements::DataWriters::assignFlightDW_FOP);
    UATM::flightAssignDataWriter_var writer_var = UATM::flightAssignDataWriter::_narrow(writer.in());

    if (CORBA::is_nil(writer_var.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                            ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }
    

    while (true)
    {
    std::string evtolID;
    std::string pilotID;
    std::string flightID;
    std::string resourceFile = "fleetOperatorDP/data/availabilities.txt";

    if (checkAvailability(resourceFile, evtolID, pilotID))
    {
      std::string flightFile = "fleetOperatorDP/data/requests.txt"; 

      if (findAndAssignFlight(flightFile, evtolID, pilotID, flightID))
      {
        UATM::flightAssign fa;

        fa.flight_assign_id = 3232; 
        fa.assign_time = "1245-32";   
        fa.flight_id = CORBA::string_dup(flightID.c_str());  
        fa.evtol_id = CORBA::string_dup(evtolID.c_str());
        fa.pilot_id = CORBA::string_dup(pilotID.c_str());

        removeAssignedResources(resourceFile, evtolID, pilotID);

        DDS::ReturnCode_t error = writer_var->write(fa, DDS::HANDLE_NIL);

        if (error != DDS::RETCODE_OK)
        {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                         ACE_TEXT(" write returned %d!\n"),
                     error));
        }
      }
      else
      {
        std::cout << "Nenhum voo disponível para atribuir evtol e piloto." << std::endl;
      }
    }
    else
    {
      std::cout << "Nenhum evtol ou piloto disponível." << std::endl;
    }
        std::this_thread::sleep_for(std::chrono::seconds(13));
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
