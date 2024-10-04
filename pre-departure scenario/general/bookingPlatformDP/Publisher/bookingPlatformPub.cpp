#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "../utils/functions.h"
#include <vector>
#include <thread>
#include <chrono>
#include <dds/DCPS/transport/tcp/Tcp.h>
#include "../../model/UATMTraits.h"
#include <model/Sync.h>

int ACE_TMAIN(int argc, ACE_TCHAR **argv)
{
  try
  {
    using OpenDDS::Model::UATM::uatmDCPS::Elements;
    OpenDDS::Model::Application application(argc, argv);
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);
    DDS::DataWriter_var writer = model.writer(Elements::DataWriters::bookingFlightRequestDW_BP);

    UATM::bookingFlightRequestDataWriter_var writer_var = UATM::bookingFlightRequestDataWriter::_narrow(writer.in());


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
        std::vector<FlightRequest> requests = readRequestsFromFile("bookingPlatformDP/data/costumers.txt");
        if (requests.empty())
        {
          std::cout << "Todos os costumers foram processados!" << std::endl;
          break;
        }

        FlightRequest current_request = requests.front();

        UATM::bookingFlightRequest bfr;

        bfr.booking_id = CORBA::string_dup(current_request.booking_id.c_str());
        bfr.flight_id = CORBA::string_dup(current_request.flight_id.c_str());
        bfr.costumer_id = CORBA::string_dup(current_request.costumer_id.c_str());
        bfr.skyport_id = CORBA::string_dup(current_request.skyport_id.c_str());

        DDS::ReturnCode_t error = writer_var->write(bfr, DDS::HANDLE_NIL);

        if (error != DDS::RETCODE_OK)
        {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                         ACE_TEXT(" write returned %d!\n"),
                     error));
        }

        removeRequestFromFile("bookingPlatformDP/data/costumers.txt", current_request.costumer_id);

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
