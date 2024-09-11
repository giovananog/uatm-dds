
#include "UATM_T.h"

#include "UATMTypeSupportImpl.h"


// For transport configuration
#include "dds/DCPS/transport/tcp/TcpInst.h"
#include "dds/DCPS/transport/udp/UdpInst.h"
#include "dds/DCPS/transport/multicast/MulticastInst.h"

#include "dds/DCPS/Service_Participant.h"
#include "model/Utilities.h"

namespace OpenDDS { namespace Model { 
namespace UATM {
namespace uatmDCPS {

inline
Elements::Data::Data()
{ 
  for( int index = 0;
       index < Elements::Types::LAST_INDEX;
       ++index) {
    this->typeNames_[index] = 0;
  }
  this->loadDomains();
  this->loadTopics();
  this->loadMaps(); /// MUST precede the QoS loading.
  this->loadTransportConfigNames();

  this->buildParticipantsQos();
  this->buildTopicsQos();
  this->buildPublishersQos();
  this->buildSubscribersQos();
  this->buildPublicationsQos();
  this->buildSubscriptionsQos();
}

inline
Elements::Data::~Data()
{
  for(int index = 0;
      index < Elements::Types::LAST_INDEX;
      ++index) {
    if(this->typeNames_[index]) {
      CORBA::string_free(this->typeNames_[index]); // Created by CORBA::string_dup()
      this->typeNames_[index] = 0;
    }
  }
}

inline
void
Elements::Data::registerType(
  Types::Values      type,
  DomainParticipant* participant)
{
  switch(type) {
    case Types::flightAuthorization:
      {
        typedef ::UATM::flightAuthorizationTypeSupportImpl TypeSupport;

        TypeSupport* typeSupport = new TypeSupport();
        if( RETCODE_OK != typeSupport->register_type( participant, 0)) {
          throw BadRegisterException();
        }

        if( this->typeNames_[ type]) {
          CORBA::string_free( this->typeNames_[ type]); // Was created by CORBA::string_dup()
        }
        this->typeNames_[ type] = typeSupport->get_type_name();
      }
      break;

    case Types::flightChangeRec:
      {
        typedef ::UATM::flightChangeRecTypeSupportImpl TypeSupport;

        TypeSupport* typeSupport = new TypeSupport();
        if( RETCODE_OK != typeSupport->register_type( participant, 0)) {
          throw BadRegisterException();
        }

        if( this->typeNames_[ type]) {
          CORBA::string_free( this->typeNames_[ type]); // Was created by CORBA::string_dup()
        }
        this->typeNames_[ type] = typeSupport->get_type_name();
      }
      break;

  default:
    throw NoTypeException();
    break;
  }
}

inline
void
Elements::Data::loadDomains()
{
  this->domains_[ Participants::uaspManagerDP] = 0;
  this->domains_[ Participants::fleetOperatorDP] = 0;
  this->domains_[ Participants::pilotManagerDP] = 0;
}

inline
void
Elements::Data::loadTopics()
{
  this->topicNames_[Topics::UATM__uatmDCPS__flightAuthorization] = "flightAuthorization";
  this->cfTopics_[Topics::UATM__uatmDCPS__flightAuthorization] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__flightAuthorization] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__flightChanceRec] = "flightChanceRec";
  this->cfTopics_[Topics::UATM__uatmDCPS__flightChanceRec] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__flightChanceRec] = MultiTopics::LAST_INDEX;
}

inline
std::string
Elements::Data::transportConfigName(Participants::Values which)
{
  return participantTxCfgNames_[which];
}

inline
std::string
Elements::Data::transportConfigName(Publishers::Values which)
{
  return publisherTxCfgNames_[which];
}

inline
std::string
Elements::Data::transportConfigName(Subscribers::Values which)
{
  return subscriberTxCfgNames_[which];
}

inline
std::string
Elements::Data::transportConfigName(DataWriters::Values which)
{
  return writerTxCfgNames_[which];
}

inline
std::string
Elements::Data::transportConfigName(DataReaders::Values which)
{
  return readerTxCfgNames_[which];
}

inline
void
Elements::Data::loadTransportConfigNames()
{
  participantTxCfgNames_[Participants::uaspManagerDP] = "";
  writerTxCfgNames_[DataWriters::routeDataDW_UASP] = "";
}

inline
void
Elements::Data::loadMaps()
{
  this->publisherParticipants_[ Publishers::uaspManagerPub] = Participants::uaspManagerDP;
  this->publisherParticipants_[ Publishers::fleetOperatorPub] = Participants::fleetOperatorDP;
  this->publisherParticipants_[ Publishers::pilotManPub] = Participants::pilotManagerDP;

  this->subscriberParticipants_[ Subscribers::uaspManagerSub] = Participants::uaspManagerDP;
  this->subscriberParticipants_[ Subscribers::fleetOperatorSub] = Participants::fleetOperatorDP;
  this->subscriberParticipants_[ Subscribers::pilotManSub] = Participants::pilotManagerDP;

  this->types_[ Topics::UATM__uatmDCPS__flightAuthorization] = Types::flightAuthorization;
  this->types_[ Topics::UATM__uatmDCPS__flightChanceRec] = Types::flightChangeRec;

  this->writerTopics_[ DataWriters::routeDataDW_UASP] = Topics::;
  this->writerTopics_[ DataWriters::flightAuthDW_UASP] = Topics::UATM__uatmDCPS__flightAuthorization;
  this->writerTopics_[ DataWriters::changeRecDW_UASP] = Topics::UATM__uatmDCPS__flightChanceRec;
  this->writerTopics_[ DataWriters::tolPadReqDW_UASP] = Topics::;
  this->writerTopics_[ DataWriters::uaspFlightRequestDW_FOP] = Topics::;
  this->writerTopics_[ DataWriters::assignFlightDW_FOP] = Topics::;
  this->writerTopics_[ DataWriters::flightCoordDW_FOP] = Topics::;
  this->writerTopics_[ DataWriters::pilotAvailabilityDW_PLM] = Topics::;

  this->readerTopics_[ DataReaders::flightRequestDR_UASP] = Topics::;
  this->readerTopics_[ DataReaders::tolPadAvailabilityDR_UASP] = Topics::;
  this->readerTopics_[ DataReaders::trafficFlowsDR_UASP] = Topics::;
  this->readerTopics_[ DataReaders::weatherInfoDR_UASP] = Topics::;
  this->readerTopics_[ DataReaders::airspaceRestDR_UASP] = Topics::;
  this->readerTopics_[ DataReaders::flightRequestDR_FOP] = Topics::;
  this->readerTopics_[ DataReaders::availabilityDR_FOP] = Topics::;
  this->readerTopics_[ DataReaders::FlightRoutesDR_FOP] = Topics::;
  this->readerTopics_[ DataReaders::weatherDR_FOP] = Topics::;
  this->readerTopics_[ DataReaders::flightAuthDR_FOP] = Topics::UATM__uatmDCPS__flightAuthorization;
  this->readerTopics_[ DataReaders::recommendationDR_FOP] = Topics::UATM__uatmDCPS__flightChanceRec;
  this->readerTopics_[ DataReaders::flightAuthDR_PLM] = Topics::UATM__uatmDCPS__flightAuthorization;
  this->readerTopics_[ DataReaders::changeRecDR_PLM] = Topics::UATM__uatmDCPS__flightChanceRec;
  this->readerTopics_[ DataReaders::flighAssignDR_PLM] = Topics::;

  this->publishers_[ DataWriters::routeDataDW_UASP] = Publishers::uaspManagerPub;
  this->publishers_[ DataWriters::flightAuthDW_UASP] = Publishers::uaspManagerPub;
  this->publishers_[ DataWriters::changeRecDW_UASP] = Publishers::uaspManagerPub;
  this->publishers_[ DataWriters::tolPadReqDW_UASP] = Publishers::uaspManagerPub;
  this->publishers_[ DataWriters::uaspFlightRequestDW_FOP] = Publishers::fleetOperatorPub;
  this->publishers_[ DataWriters::assignFlightDW_FOP] = Publishers::fleetOperatorPub;
  this->publishers_[ DataWriters::flightCoordDW_FOP] = Publishers::fleetOperatorPub;
  this->publishers_[ DataWriters::pilotAvailabilityDW_PLM] = Publishers::pilotManPub;

  this->subscribers_[ DataReaders::flightRequestDR_UASP] = Subscribers::uaspManagerSub;
  this->subscribers_[ DataReaders::tolPadAvailabilityDR_UASP] = Subscribers::uaspManagerSub;
  this->subscribers_[ DataReaders::trafficFlowsDR_UASP] = Subscribers::uaspManagerSub;
  this->subscribers_[ DataReaders::weatherInfoDR_UASP] = Subscribers::uaspManagerSub;
  this->subscribers_[ DataReaders::airspaceRestDR_UASP] = Subscribers::uaspManagerSub;
  this->subscribers_[ DataReaders::flightRequestDR_FOP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::availabilityDR_FOP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::FlightRoutesDR_FOP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::weatherDR_FOP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::flightAuthDR_FOP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::recommendationDR_FOP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::flightAuthDR_PLM] = Subscribers::pilotManSub;
  this->subscribers_[ DataReaders::changeRecDR_PLM] = Subscribers::pilotManSub;
  this->subscribers_[ DataReaders::flighAssignDR_PLM] = Subscribers::pilotManSub;

}

inline
void
Elements::Data::buildParticipantsQos()
{
  DomainParticipantQos participantQos;
  Participants::Values participant;
    
  participant = Participants::uaspManagerDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;

  participant = Participants::fleetOperatorDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;

  participant = Participants::pilotManagerDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;
}

inline
void
Elements::Data::buildTopicsQos()
{
  TopicQos       topicQos;
  Topics::Values topic;
    
  topic    = Topics::UATM__uatmDCPS__flightAuthorization;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = PERSISTENT_DURABILITY_QOS;
  topicQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
  topicQos.reliability.kind = RELIABLE_RELIABILITY_QOS;
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 50000000;
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__flightChanceRec;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = TRANSIENT_DURABILITY_QOS;
  topicQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
  topicQos.reliability.kind = RELIABLE_RELIABILITY_QOS;
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 50000000;
  this->topicsQos_[ topic] = topicQos;
}

inline
void
Elements::Data::buildPublishersQos()
{
  PublisherQos       publisherQos;
  Publishers::Values publisher;

  publisher    = Publishers::uaspManagerPub;
  publisherQos = TheServiceParticipant->initial_PublisherQos();
  this->publishersQos_[ publisher] = publisherQos;

  publisher    = Publishers::fleetOperatorPub;
  publisherQos = TheServiceParticipant->initial_PublisherQos();
  this->publishersQos_[ publisher] = publisherQos;

  publisher    = Publishers::pilotManPub;
  publisherQos = TheServiceParticipant->initial_PublisherQos();
  this->publishersQos_[ publisher] = publisherQos;
}

inline
void
Elements::Data::buildSubscribersQos()
{
  SubscriberQos       subscriberQos;
  Subscribers::Values subscriber;

  subscriber    = Subscribers::uaspManagerSub;
  subscriberQos = TheServiceParticipant->initial_SubscriberQos();
  this->subscribersQos_[ subscriber] = subscriberQos;

  subscriber    = Subscribers::fleetOperatorSub;
  subscriberQos = TheServiceParticipant->initial_SubscriberQos();
  this->subscribersQos_[ subscriber] = subscriberQos;

  subscriber    = Subscribers::pilotManSub;
  subscriberQos = TheServiceParticipant->initial_SubscriberQos();
  this->subscribersQos_[ subscriber] = subscriberQos;
}

inline
void
Elements::Data::buildPublicationsQos()
{
  DataWriters::Values  writer;
  DataWriterQos        writerQos;

  writer    = DataWriters::routeDataDW_UASP;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = true;

  writer    = DataWriters::flightAuthDW_UASP;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = true;

  writer    = DataWriters::changeRecDW_UASP;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = true;

  writer    = DataWriters::tolPadReqDW_UASP;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = true;

  writer    = DataWriters::uaspFlightRequestDW_FOP;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = true;

  writer    = DataWriters::assignFlightDW_FOP;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = true;

  writer    = DataWriters::flightCoordDW_FOP;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = true;

  writer    = DataWriters::pilotAvailabilityDW_PLM;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = true;
}

inline
void
Elements::Data::buildSubscriptionsQos()
{
  DataReaders::Values  reader;
  DataReaderQos        readerQos;

  reader    = DataReaders::flightRequestDR_UASP;
  readerQos = TheServiceParticipant->initial_DataReaderQos();
  this->readersQos_[ reader] = readerQos;
  this->readerCopyTopicQos_[reader] = true;

  reader    = DataReaders::tolPadAvailabilityDR_UASP;
  readerQos = TheServiceParticipant->initial_DataReaderQos();
  this->readersQos_[ reader] = readerQos;
  this->readerCopyTopicQos_[reader] = true;

  reader    = DataReaders::trafficFlowsDR_UASP;
  readerQos = TheServiceParticipant->initial_DataReaderQos();
  this->readersQos_[ reader] = readerQos;
  this->readerCopyTopicQos_[reader] = true;

  reader    = DataReaders::weatherInfoDR_UASP;
  readerQos = TheServiceParticipant->initial_DataReaderQos();
  this->readersQos_[ reader] = readerQos;
  this->readerCopyTopicQos_[reader] = true;

  reader    = DataReaders::airspaceRestDR_UASP;
  readerQos = TheServiceParticipant->initial_DataReaderQos();
  this->readersQos_[ reader] = readerQos;
  this->readerCopyTopicQos_[reader] = true;

  reader    = DataReaders::flightRequestDR_FOP;
  readerQos = TheServiceParticipant->initial_DataReaderQos();
  this->readersQos_[ reader] = readerQos;
  this->readerCopyTopicQos_[reader] = true;

  reader    = DataReaders::availabilityDR_FOP;
  readerQos = TheServiceParticipant->initial_DataReaderQos();
  this->readersQos_[ reader] = readerQos;
  this->readerCopyTopicQos_[reader] = true;

  reader    = DataReaders::FlightRoutesDR_FOP;
  readerQos = TheServiceParticipant->initial_DataReaderQos();
  this->readersQos_[ reader] = readerQos;
  this->readerCopyTopicQos_[reader] = true;

  reader    = DataReaders::weatherDR_FOP;
  readerQos = TheServiceParticipant->initial_DataReaderQos();
  this->readersQos_[ reader] = readerQos;
  this->readerCopyTopicQos_[reader] = true;

  reader    = DataReaders::flightAuthDR_FOP;
  readerQos = TheServiceParticipant->initial_DataReaderQos();
  this->readersQos_[ reader] = readerQos;
  this->readerCopyTopicQos_[reader] = true;

  reader    = DataReaders::recommendationDR_FOP;
  readerQos = TheServiceParticipant->initial_DataReaderQos();
  this->readersQos_[ reader] = readerQos;
  this->readerCopyTopicQos_[reader] = true;

  reader    = DataReaders::flightAuthDR_PLM;
  readerQos = TheServiceParticipant->initial_DataReaderQos();
  this->readersQos_[ reader] = readerQos;
  this->readerCopyTopicQos_[reader] = true;

  reader    = DataReaders::changeRecDR_PLM;
  readerQos = TheServiceParticipant->initial_DataReaderQos();
  this->readersQos_[ reader] = readerQos;
  this->readerCopyTopicQos_[reader] = true;

  reader    = DataReaders::flighAssignDR_PLM;
  readerQos = TheServiceParticipant->initial_DataReaderQos();
  this->readersQos_[ reader] = readerQos;
  this->readerCopyTopicQos_[reader] = true;
}


inline
void
Elements::Data::copyPublicationQos(
  DataWriters::Values which,
  DataWriterQos&  
)
{
  switch(which) {
    case DataWriters::routeDataDW_UASP:
      break;

    case DataWriters::flightAuthDW_UASP:
      break;

    case DataWriters::changeRecDW_UASP:
      break;

    case DataWriters::tolPadReqDW_UASP:
      break;

    case DataWriters::uaspFlightRequestDW_FOP:
      break;

    case DataWriters::assignFlightDW_FOP:
      break;

    case DataWriters::flightCoordDW_FOP:
      break;

    case DataWriters::pilotAvailabilityDW_PLM:
      break;

    default:
      throw NoWriterException();
  }
}

inline
void
Elements::Data::copySubscriptionQos(
  DataReaders::Values which,
  DataReaderQos&  
)
{
  switch(which) {
    case DataReaders::flightRequestDR_UASP:
      break;

    case DataReaders::tolPadAvailabilityDR_UASP:
      break;

    case DataReaders::trafficFlowsDR_UASP:
      break;

    case DataReaders::weatherInfoDR_UASP:
      break;

    case DataReaders::airspaceRestDR_UASP:
      break;

    case DataReaders::flightRequestDR_FOP:
      break;

    case DataReaders::availabilityDR_FOP:
      break;

    case DataReaders::FlightRoutesDR_FOP:
      break;

    case DataReaders::weatherDR_FOP:
      break;

    case DataReaders::flightAuthDR_FOP:
      break;

    case DataReaders::recommendationDR_FOP:
      break;

    case DataReaders::flightAuthDR_PLM:
      break;

    case DataReaders::changeRecDR_PLM:
      break;

    case DataReaders::flighAssignDR_PLM:
      break;

    default:
      throw NoReaderException();
  }
}
} // End namespace uatmDCPS
} // End namespace UATM

} // End namespace Model
} // End namespace OpenDDS
