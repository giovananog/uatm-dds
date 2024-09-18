
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
    case Types::flightCoordination:
      {
        typedef ::UATM::flightCoordinationTypeSupportImpl TypeSupport;

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

    case Types::flightAuthorizationRequest:
      {
        typedef ::UATM::flightAuthorizationRequestTypeSupportImpl TypeSupport;

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

    case Types::flightRequestInfo:
      {
        typedef ::UATM::flightRequestInfoTypeSupportImpl TypeSupport;

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
  this->domains_[ Participants::fleetOperatorDP] = 0;
  this->domains_[ Participants::skyportOperatorDP] = 0;
  this->domains_[ Participants::flightAuthSysDP] = 0;
  this->domains_[ Participants::uaspManagerDP] = 0;
}

inline
void
Elements::Data::loadTopics()
{
  this->topicNames_[Topics::UATM__uatmDCPS__flightCoordination] = "flightCoordination";
  this->cfTopics_[Topics::UATM__uatmDCPS__flightCoordination] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__flightCoordination] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__flightAuthorizationRequest] = "flightAuthorizationRequest";
  this->cfTopics_[Topics::UATM__uatmDCPS__flightAuthorizationRequest] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__flightAuthorizationRequest] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__flightRequestInfo] = "flightRequestInfo";
  this->cfTopics_[Topics::UATM__uatmDCPS__flightRequestInfo] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__flightRequestInfo] = MultiTopics::LAST_INDEX;
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
}

inline
void
Elements::Data::loadMaps()
{
  this->publisherParticipants_[ Publishers::fleetOperatorPub] = Participants::fleetOperatorDP;
  this->publisherParticipants_[ Publishers::skyportOperatorPub] = Participants::skyportOperatorDP;
  this->publisherParticipants_[ Publishers::flightAuthSysPub] = Participants::flightAuthSysDP;
  this->publisherParticipants_[ Publishers::uaspManPub] = Participants::uaspManagerDP;

  this->subscriberParticipants_[ Subscribers::fleetOperatorSub] = Participants::fleetOperatorDP;
  this->subscriberParticipants_[ Subscribers::skyportOperatorPub] = Participants::skyportOperatorDP;
  this->subscriberParticipants_[ Subscribers::flightAuthSysSub] = Participants::flightAuthSysDP;
  this->subscriberParticipants_[ Subscribers::flightAuthSysSub] = Participants::uaspManagerDP;

  this->types_[ Topics::UATM__uatmDCPS__flightCoordination] = Types::flightCoordination;
  this->types_[ Topics::UATM__uatmDCPS__flightAuthorizationRequest] = Types::flightAuthorizationRequest;
  this->types_[ Topics::UATM__uatmDCPS__flightRequestInfo] = Types::flightRequestInfo;

  this->writerTopics_[ DataWriters::uaspFlightRequestDW_FOP] = Topics::UATM__uatmDCPS__flightAuthorizationRequest;
  this->writerTopics_[ DataWriters::flightCoordDW_FOP] = Topics::UATM__uatmDCPS__flightCoordination;
  this->writerTopics_[ DataWriters::flightRequestInfoDW_FAS] = Topics::UATM__uatmDCPS__flightRequestInfo;

  this->readerTopics_[ DataReaders::flightCoordDR_SKO] = Topics::UATM__uatmDCPS__flightCoordination;
  this->readerTopics_[ DataReaders::flightRequestDR_FAS] = Topics::UATM__uatmDCPS__flightAuthorizationRequest;
  this->readerTopics_[ DataReaders::flightRequestDR_UASP] = Topics::UATM__uatmDCPS__flightRequestInfo;

  this->publishers_[ DataWriters::uaspFlightRequestDW_FOP] = Publishers::fleetOperatorPub;
  this->publishers_[ DataWriters::assignFlightDW_FOP] = Publishers::fleetOperatorPub;
  this->publishers_[ DataWriters::flightCoordDW_FOP] = Publishers::fleetOperatorPub;
  this->publishers_[ DataWriters::flightRoutesDW_SKO] = Publishers::skyportOperatorPub;
  this->publishers_[ DataWriters::trafficFlowsDW_SKO] = Publishers::skyportOperatorPub;
  this->publishers_[ DataWriters::airspaceRestDW_SKO] = Publishers::skyportOperatorPub;
  this->publishers_[ DataWriters::flightRequestInfoDW_FAS] = Publishers::flightAuthSysPub;
  this->publishers_[ DataWriters::routeDataDW_UASP] = Publishers::uaspManPub;
  this->publishers_[ DataWriters::flightAuthDW_UASP] = Publishers::uaspManPub;
  this->publishers_[ DataWriters::changeRecDW_UASP] = Publishers::uaspManPub;
  this->publishers_[ DataWriters::tolPadReqDW_UASP] = Publishers::uaspManPub;

  this->subscribers_[ DataReaders::flightRequestDR_FOP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::availabilityDR_FOP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::FlightRoutesDR_FOP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::weatherDR_FOP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::flightAuthDR_FOP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::recommendationDR_FOP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::availabilityDR_SKO] = Subscribers::skyportOperatorPub;
  this->subscribers_[ DataReaders::flightCoordDR_SKO] = Subscribers::skyportOperatorPub;
  this->subscribers_[ DataReaders::flightRequestDR_FAS] = Subscribers::flightAuthSysSub;
  this->subscribers_[ DataReaders::flightRequestDR_UASP] = Subscribers::flightAuthSysSub;
  this->subscribers_[ DataReaders::tolPadAvailabilityDR_UASP] = Subscribers::flightAuthSysSub;
  this->subscribers_[ DataReaders::trafficFlowsDR_UASP] = Subscribers::flightAuthSysSub;
  this->subscribers_[ DataReaders::weatherInfoDR_UASP] = Subscribers::flightAuthSysSub;
  this->subscribers_[ DataReaders::airspaceRestDR_UASP] = Subscribers::flightAuthSysSub;

}

inline
void
Elements::Data::buildParticipantsQos()
{
  DomainParticipantQos participantQos;
  Participants::Values participant;
    
  participant = Participants::fleetOperatorDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;

  participant = Participants::skyportOperatorDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;

  participant = Participants::flightAuthSysDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;

  participant = Participants::uaspManagerDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;
}

inline
void
Elements::Data::buildTopicsQos()
{
  TopicQos       topicQos;
  Topics::Values topic;
    
  topic    = Topics::UATM__uatmDCPS__flightCoordination;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = TRANSIENT_DURABILITY_QOS;
  topicQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
  topicQos.reliability.kind = RELIABLE_RELIABILITY_QOS;
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 50000000;
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__flightAuthorizationRequest;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = TRANSIENT_DURABILITY_QOS;
  topicQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
  topicQos.reliability.kind = RELIABLE_RELIABILITY_QOS;
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 50000000;
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__flightRequestInfo;
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

  publisher    = Publishers::fleetOperatorPub;
  publisherQos = TheServiceParticipant->initial_PublisherQos();
  this->publishersQos_[ publisher] = publisherQos;

  publisher    = Publishers::skyportOperatorPub;
  publisherQos = TheServiceParticipant->initial_PublisherQos();
  this->publishersQos_[ publisher] = publisherQos;

  publisher    = Publishers::flightAuthSysPub;
  publisherQos = TheServiceParticipant->initial_PublisherQos();
  this->publishersQos_[ publisher] = publisherQos;

  publisher    = Publishers::uaspManPub;
  publisherQos = TheServiceParticipant->initial_PublisherQos();
  this->publishersQos_[ publisher] = publisherQos;
}

inline
void
Elements::Data::buildSubscribersQos()
{
  SubscriberQos       subscriberQos;
  Subscribers::Values subscriber;

  subscriber    = Subscribers::fleetOperatorSub;
  subscriberQos = TheServiceParticipant->initial_SubscriberQos();
  this->subscribersQos_[ subscriber] = subscriberQos;

  subscriber    = Subscribers::skyportOperatorPub;
  subscriberQos = TheServiceParticipant->initial_SubscriberQos();
  this->subscribersQos_[ subscriber] = subscriberQos;

  subscriber    = Subscribers::flightAuthSysSub;
  subscriberQos = TheServiceParticipant->initial_SubscriberQos();
  this->subscribersQos_[ subscriber] = subscriberQos;

  subscriber    = Subscribers::flightAuthSysSub;
  subscriberQos = TheServiceParticipant->initial_SubscriberQos();
  this->subscribersQos_[ subscriber] = subscriberQos;
}

inline
void
Elements::Data::buildPublicationsQos()
{
  DataWriters::Values  writer;
  DataWriterQos        writerQos;

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

  writer    = DataWriters::flightRoutesDW_SKO;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = true;

  writer    = DataWriters::trafficFlowsDW_SKO;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = true;

  writer    = DataWriters::airspaceRestDW_SKO;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = true;

  writer    = DataWriters::flightRequestInfoDW_FAS;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = true;

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
}

inline
void
Elements::Data::buildSubscriptionsQos()
{
  DataReaders::Values  reader;
  DataReaderQos        readerQos;

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

  reader    = DataReaders::availabilityDR_SKO;
  readerQos = TheServiceParticipant->initial_DataReaderQos();
  this->readersQos_[ reader] = readerQos;
  this->readerCopyTopicQos_[reader] = true;

  reader    = DataReaders::flightCoordDR_SKO;
  readerQos = TheServiceParticipant->initial_DataReaderQos();
  this->readersQos_[ reader] = readerQos;
  this->readerCopyTopicQos_[reader] = true;

  reader    = DataReaders::flightRequestDR_FAS;
  readerQos = TheServiceParticipant->initial_DataReaderQos();
  this->readersQos_[ reader] = readerQos;
  this->readerCopyTopicQos_[reader] = true;

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
}


inline
void
Elements::Data::copyPublicationQos(
  DataWriters::Values which,
  DataWriterQos&  
)
{
  switch(which) {
    case DataWriters::uaspFlightRequestDW_FOP:
      break;

    case DataWriters::assignFlightDW_FOP:
      break;

    case DataWriters::flightCoordDW_FOP:
      break;

    case DataWriters::flightRoutesDW_SKO:
      break;

    case DataWriters::trafficFlowsDW_SKO:
      break;

    case DataWriters::airspaceRestDW_SKO:
      break;

    case DataWriters::flightRequestInfoDW_FAS:
      break;

    case DataWriters::routeDataDW_UASP:
      break;

    case DataWriters::flightAuthDW_UASP:
      break;

    case DataWriters::changeRecDW_UASP:
      break;

    case DataWriters::tolPadReqDW_UASP:
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

    case DataReaders::availabilityDR_SKO:
      break;

    case DataReaders::flightCoordDR_SKO:
      break;

    case DataReaders::flightRequestDR_FAS:
      break;

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

    default:
      throw NoReaderException();
  }
}
} // End namespace uatmDCPS
} // End namespace UATM

} // End namespace Model
} // End namespace OpenDDS
