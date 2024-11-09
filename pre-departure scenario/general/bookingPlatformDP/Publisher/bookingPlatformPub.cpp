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

// Main entry point for the application
int ACE_TMAIN(int argc, ACE_TCHAR **argv)
{
  try
  {
    // Initialize OpenDDS Model and create a data writer for booking requests
    using OpenDDS::Model::UATM::uatmDCPS::Elements;
    OpenDDS::Model::Application application(argc, argv);
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);
    DDS::DataWriter_var writer = model.writer(Elements::DataWriters::bookingFlightRequestDW_BP);

    // Narrow the writer to the specific type
    UATM::bookingFlightRequestDataWriter_var writer_var = UATM::bookingFlightRequestDataWriter::_narrow(writer.in());

    // Check if narrowing was successful
    if (CORBA::is_nil(writer_var.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                            ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }

    int bookingID = 0; // Initialize booking ID counter
    auto startTime = std::chrono::steady_clock::now(); // Start time of the application
    double duration = 100.0;   // Total duration of the execution (seconds)
    double warmupTime = 10.0;  // Warmup period (seconds)
    double lambda = 3.0;       // Rate parameter for Poisson distribution

    bool warmupCompleted = false; // Flag to check if warmup period is over

    // Main loop for generating events and sending booking requests
    while (true)
    {
      auto currentTime = std::chrono::steady_clock::now();
      std::chrono::duration<double> elapsedTime = currentTime - startTime;

      // Check if warmup period has completed
      if (elapsedTime.count() >= warmupTime && !warmupCompleted)
      {
        warmupCompleted = true;
      }

      // End execution after specified duration
      if (elapsedTime.count() >= duration) 
      {
        break;
      }

      // Generate a Poisson-distributed number of events
      double numEvents = generatePoisson(lambda);
      double waitTime = (10.0 / numEvents); // Calculate wait time between events

      // Loop through each event to create booking requests
      for (int i = 0; i < numEvents; i++)
      {
        if (warmupCompleted)  // Only process events after warmup period
        {
          OpenDDS::Model::WriterSync ws(writer); // Synchronize writer access
          {

            bookingID++; // Increment booking ID for each request
            UATM::bookingFlightRequest bfr;

            // Assign unique identifiers to each booking, flight, and customer
            std::string bookingIDStr = std::to_string(bookingID);
            bfr.booking_id = CORBA::string_dup(("Booking-" + bookingIDStr).c_str());
            bfr.flight_id = CORBA::string_dup(("Flight-" + bookingIDStr).c_str());
            bfr.costumer_id = CORBA::string_dup(("Costumer-" + std::to_string(rand() % 1000)).c_str());
            bfr.origin_skyport_id = CORBA::string_dup(generateOriginSkyportId().c_str());
            bfr.destination_skyport_id = CORBA::string_dup(generateDestinationSkyportId(std::string(bfr.origin_skyport_id)).c_str());

            // Write the booking request if IDs are valid
            if (bfr.booking_id.in() != "0" && bfr.costumer_id.in() != "0")
            {
              DDS::ReturnCode_t error = writer_var->write(bfr, DDS::HANDLE_NIL);

              // Log an error if the write operation fails
              if (error != DDS::RETCODE_OK)
              {
                ACE_ERROR((LM_ERROR,
                           ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                               ACE_TEXT(" write returned %d!\n"),
                           error));
              }
            }
          }
        }

        // Wait before processing the next event
        std::this_thread::sleep_for(std::chrono::seconds(static_cast<int>(waitTime)));
      }
    }
  }
  // Handle CORBA and standard exceptions
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
  return 0; // Return 0 on successful execution
}
