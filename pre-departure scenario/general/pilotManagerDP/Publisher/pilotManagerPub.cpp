#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <thread>
#include <chrono>
#include <dds/DCPS/transport/tcp/Tcp.h>  // Include OpenDDS library for TCP transport
#include "../../model/UATMTraits.h"    // Include UATM traits, possibly defining message types and other entities
#include "../utils/functions.h"       // Include utility functions
#include <model/Sync.h>               // Include synchronization utilities
#include <unordered_set>              // For using unordered sets to track sent pilots

int ACE_TMAIN(int argc, ACE_TCHAR **argv)
{
  try
  {
    // Initialize the OpenDDS application and model
    OpenDDS::Model::Application application(argc, argv);
    UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);

    using OpenDDS::Model::UATM::uatmDCPS::Elements;

    // Create a DataWriter for the pilot availability data
    DDS::DataWriter_var writer = model.writer(Elements::DataWriters::pilotAvailabilityDW_PLM);

    UATM::availabilityInfoDataWriter_var writer_var = UATM::availabilityInfoDataWriter::_narrow(writer.in());

    // Check if the writer was properly narrowed
    if (CORBA::is_nil(writer_var.in()))
    {
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                            ACE_TEXT(" _narrow failed!\n")),
                       -1);
    }

    // Path to the file containing the pilot information
    std::string filename = "pilotManagerDP/data/pilots.txt";
    // Set to track which pilots have already been sent
    std::unordered_set<std::string> sent_pilots;
    int i = 0;
    // Start time for the simulation
    auto startTime = std::chrono::steady_clock::now();
    // Duration after which to stop the loop (in seconds)
    double duration = 100.0;
    
    // Infinite loop to send pilot availability information
    while (true)
    {
      // Read the list of pilots from the file
      std::vector<Pilot> pilots = readPilotsFromFile(filename);
      // Get the current time and calculate elapsed time
      auto currentTime = std::chrono::steady_clock::now();
      std::chrono::duration<double> elapsedTime = currentTime - startTime;

      // If the elapsed time has exceeded the duration, stop the loop
      if (elapsedTime.count() >= duration)
      {
        // Clear the contents of the file and add new data
        std::ofstream file(filename, std::ios::trunc);
        std::string data =
           "pilot_id=Pilot-1,skyport_id=Skyport-1,available=1\n"
           "pilot_id=Pilot-2,skyport_id=Skyport-1,available=1\n"
           "pilot_id=Pilot-3,skyport_id=Skyport-2,available=1\n";
        file << data; // Write the data to the file
        file.close(); // Close the file
        break; // Break the loop after updating the file
      }

      // Synchronize the writer before sending any data
      OpenDDS::Model::WriterSync ws(writer);
      {
        // Loop through each pilot and send their availability data if not already sent or if available
        for (const auto &pilot : pilots)
        {
          if (sent_pilots.find(std::string(pilot.pilot_id)) == sent_pilots.end() || pilot.available == 1)
          {
            // Create a message to send the pilot's availability
            UATM::availabilityInfo bfr;

            // Populate the message with the pilot's data
            bfr.resource_id = CORBA::string_dup(pilot.pilot_id.c_str());
            bfr.resource_type = "pilot";
            bfr.available = pilot.available;
            bfr.skyport_id = CORBA::string_dup(pilot.skyport_id.c_str());
            bfr.availability_time = CORBA::string_dup(getCurrentTime().c_str());

            // Write the availability information to the DDS DataWriter
            DDS::ReturnCode_t error = writer_var->write(bfr, DDS::HANDLE_NIL);

            // Check if there was an error sending the data
            if (error != DDS::RETCODE_OK)
            {
              ACE_ERROR((LM_ERROR,
                         ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                             ACE_TEXT(" write returned %d!\n"),
                         error));
            }

            // Add the pilot to the set of sent pilots
            sent_pilots.insert(std::string(pilot.pilot_id));

            break; // Exit the loop after sending the first pilot's availability
          }
        }
      }

      // Wait for 2 seconds before sending the next piece of data
      std::this_thread::sleep_for(std::chrono::seconds(2));
    }
  }
  catch (const CORBA::Exception &e) // Catch CORBA exceptions
  {
    e._tao_print_exception("Exception caught in main():");
    return -1;
  }
  catch (const std::exception &ex) // Catch standard exceptions
  {
    ACE_ERROR_RETURN((LM_ERROR,
                      ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                          ACE_TEXT(" Exception caught: %C\n"),
                      ex.what()),
                     -1);
  }
  return 0;
}
