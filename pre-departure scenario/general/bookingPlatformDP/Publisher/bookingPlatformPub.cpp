#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "../utils/functions.h"
#include <vector>
#include <thread>
#include <chrono>
#include <random>
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

    int i = 0;
    int bookingID = 0;

    OpenDDS::Model::WriterSync ws(writer);
    {
      while (true)
      {
        i++;
        bookingID++;
        if (i == 4)
        {
          // std::cout << "Todos os costumers foram processados!" << std::endl;
          break;
        }

        UATM::bookingFlightRequest bfr;

        std::string bookingIDStr = std::to_string(bookingID);
        bfr.booking_id = CORBA::string_dup(("Booking-" + bookingIDStr).c_str());
        bfr.flight_id = CORBA::string_dup(("Flight-" + bookingIDStr).c_str());
        bfr.costumer_id = CORBA::string_dup(("Costumer-" + std::to_string(rand() % 1000)).c_str());
        bfr.origin_skyport_id = CORBA::string_dup(generateOriginSkyportId().c_str());
        bfr.destination_skyport_id = CORBA::string_dup(generateDestinationCustomerId(std::string(bfr.origin_skyport_id)).c_str());

        DDS::ReturnCode_t error = writer_var->write(bfr, DDS::HANDLE_NIL);

        if (error != DDS::RETCODE_OK)
        {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                         ACE_TEXT(" write returned %d!\n"),
                     error));
        }

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
