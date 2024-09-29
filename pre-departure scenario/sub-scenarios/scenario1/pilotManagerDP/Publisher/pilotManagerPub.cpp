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

struct Pilot
{
  std::string pilot_id;
  std::string skyport_id;
  int available;
  int sent;
};

std::vector<Pilot> readPilotsFromFile(const std::string &filename)
{
  std::ifstream file(filename);
  std::string line;
  std::vector<Pilot> pilots;

  while (std::getline(file, line))
  {
    if (!line.empty())
    {
      Pilot pilot;
      std::istringstream ss(line);

      std::string temp;

      std::getline(ss, temp, '='); 
      std::getline(ss, pilot.pilot_id, ','); 

      std::getline(ss, temp, '='); 
      std::getline(ss, pilot.skyport_id, ','); 

      std::getline(ss, temp, '='); 
      ss >> pilot.available; 
      ss.ignore(1); 

      std::getline(ss, temp, '='); 
      ss >> pilot.sent; 

      if (pilot.sent == 0)
      {
        pilots.push_back(pilot);
      }
    }
  }
  return pilots;
}

void updatePilotInFile(const std::string &filename, const std::string &pilot_id)
{
  std::ifstream file(filename);
  std::string line;
  std::vector<std::string> lines;

  while (std::getline(file, line))
  {
    if (line.find(pilot_id) != std::string::npos)
    {
      std::string updated_line = line;
      size_t pos = line.find("sent=0");
      if (pos != std::string::npos)
      {
        updated_line.replace(pos, 6, "sent=1");
      }
      lines.push_back(updated_line);
    }
    else
    {
      lines.push_back(line);
    }
  }
  file.close();

  std::ofstream outFile(filename);
  for (const auto &l : lines)
  {
    outFile << l << "\n";
  }
}

  int ACE_TMAIN(int argc, ACE_TCHAR **argv)
  {
    try
    {
      OpenDDS::Model::Application application(argc, argv);
      UATM::uatmDCPS::DefaultUATMType model(application, argc, argv);

      using OpenDDS::Model::UATM::uatmDCPS::Elements;

      DDS::DataWriter_var writer = model.writer(Elements::DataWriters::pilotAvailabilityDW_PLM);

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
          std::string filename = "pilotManagerDP/pilots.txt";
          std::vector<Pilot> pilots = readPilotsFromFile(filename);

          if (pilots.empty())
          {
            std::cout << "Todos os pilotos foram enviados!" << std::endl;
            break;
          }

          Pilot current_pilot = pilots.front();

          UATM::availabilityInfo bfr;

          bfr.resource_id = CORBA::string_dup(current_pilot.pilot_id.c_str());
          bfr.resource_type = "pilot";
          bfr.available = current_pilot.available;
          bfr.skyport_id = CORBA::string_dup(current_pilot.skyport_id.c_str());
          bfr.availability_time = "323123";

          DDS::ReturnCode_t error = writer_var->write(bfr, DDS::HANDLE_NIL);


          if (error != DDS::RETCODE_OK)
          {
            ACE_ERROR((LM_ERROR,
                       ACE_TEXT("(%P|%t) ERROR: %N:%l: main() -")
                           ACE_TEXT(" write returned %d!\n"),
                       error));
          }

          updatePilotInFile(filename, current_pilot.pilot_id);

          std::this_thread::sleep_for(std::chrono::seconds(6));
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
