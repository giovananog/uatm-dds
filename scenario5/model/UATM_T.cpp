
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
    case Types::weatherInfo:
      {
        typedef ::UATM::weatherInfoTypeSupportImpl TypeSupport;

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

    case Types::trafficFlowsInfo:
      {
        typedef ::UATM::trafficFlowsInfoTypeSupportImpl TypeSupport;

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

    case Types::asirspaceRestrictions:
      {
        typedef ::UATM::asirspaceRestrictionsTypeSupportImpl TypeSupport;

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

    case Types::acceptableRoute:
      {
        typedef ::UATM::acceptableRouteTypeSupportImpl TypeSupport;

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
  this->domains_[ Participants::skyportOperatorDP] = 0;
  this->domains_[ Participants::weatherDP] = 0;
  this->domains_[ Participants::anspDP] = 0;
  this->domains_[ Participants::ussDP] = 0;
}

inline
void
Elements::Data::loadTopics()
{
  this->topicNames_[Topics::UATM__uatmDCPS__trafficFlowsInfo] = "trafficFlowsInfo";
  this->cfTopics_[Topics::UATM__uatmDCPS__trafficFlowsInfo] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__trafficFlowsInfo] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__weatherInfo] = "weatherInfo";
  this->cfTopics_[Topics::UATM__uatmDCPS__weatherInfo] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__weatherInfo] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__airspaceRestrictions] = "airspaceRestrictions";
  this->cfTopics_[Topics::UATM__uatmDCPS__airspaceRestrictions] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__airspaceRestrictions] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__acceptableRoute] = "acceptableRoute";
  this->cfTopics_[Topics::UATM__uatmDCPS__acceptableRoute] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__acceptableRoute] = MultiTopics::LAST_INDEX;
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
  this->publisherParticipants_[ Publishers::fleetOperatorPub] = Participants::uaspManagerDP;
  this->publisherParticipants_[ Publishers::skyportOperatorPub] = Participants::skyportOperatorDP;
  this->publisherParticipants_[ Publishers::weatherPub] = Participants::weatherDP;

  this->subscriberParticipants_[ Subscribers::fleetOperatorSub] = Participants::uaspManagerDP;
  this->subscriberParticipants_[ Subscribers::skyportOperatorSub] = Participants::skyportOperatorDP;
  this->subscriberParticipants_[ Subscribers::anspSub] = Participants::anspDP;
  this->subscriberParticipants_[ Subscribers::ussSub] = Participants::ussDP;

  this->types_[ Topics::UATM__uatmDCPS__trafficFlowsInfo] = Types::trafficFlowsInfo;
  this->types_[ Topics::UATM__uatmDCPS__weatherInfo] = Types::weatherInfo;
  this->types_[ Topics::UATM__uatmDCPS__airspaceRestrictions] = Types::asirspaceRestrictions;
  this->types_[ Topics::UATM__uatmDCPS__acceptableRoute] = Types::acceptableRoute;

  this->writerTopics_[ DataWriters::routeDataDW_UASP] = Topics::;
  this->writerTopics_[ DataWriters::flightAuthDW_UASP] = Topics::;
  this->writerTopics_[ DataWriters::changeRecDW_UASP] = Topics::UATM__uatmDCPS__acceptableRoute;
  this->writerTopics_[ DataWriters::tolPadReqDW_UASP] = Topics::;
  this->writerTopics_[ DataWriters::flightRoutesDW_SKO] = Topics::;
  this->writerTopics_[ DataWriters::trafficFlowsDW_SKO] = Topics::UATM__uatmDCPS__trafficFlowsInfo;
  this->writerTopics_[ DataWriters::airspaceRestDW_SKO] = Topics::UATM__uatmDCPS__airspaceRestrictions;
  this->writerTopics_[ DataWriters::writer] = Topics::UATM__uatmDCPS__weatherInfo;

  this->readerTopics_[ DataReaders::flightRequestDR_UASP] = Topics::;
  this->readerTopics_[ DataReaders::tolPadAvailabilityDR_UASP] = Topics::;
  this->readerTopics_[ DataReaders::trafficFlowsDR_UASP] = Topics::UATM__uatmDCPS__trafficFlowsInfo;
  this->readerTopics_[ DataReaders::weatherInfoDR_UASP] = Topics::UATM__uatmDCPS__weatherInfo;
  this->readerTopics_[ DataReaders::airspaceRestDR_UASP] = Topics::UATM__uatmDCPS__airspaceRestrictions;
  this->readerTopics_[ DataReaders::availabilityDR_SKO] = Topics::;
  this->readerTopics_[ DataReaders::flightCoordDR_SKO] = Topics::;
  this->readerTopics_[ DataReaders::reader] = Topics::UATM__uatmDCPS__acceptableRoute;
  this->readerTopics_[ DataReaders::reader] = Topics::UATM__uatmDCPS__acceptableRoute;

  this->publishers_[ DataWriters::routeDataDW_UASP] = Publishers::fleetOperatorPub;
  this->publishers_[ DataWriters::flightAuthDW_UASP] = Publishers::fleetOperatorPub;
  this->publishers_[ DataWriters::changeRecDW_UASP] = Publishers::fleetOperatorPub;
  this->publishers_[ DataWriters::tolPadReqDW_UASP] = Publishers::fleetOperatorPub;
  this->publishers_[ DataWriters::flightRoutesDW_SKO] = Publishers::skyportOperatorPub;
  this->publishers_[ DataWriters::trafficFlowsDW_SKO] = Publishers::skyportOperatorPub;
  this->publishers_[ DataWriters::airspaceRestDW_SKO] = Publishers::skyportOperatorPub;
  this->publishers_[ DataWriters::writer] = Publishers::weatherPub;

  this->subscribers_[ DataReaders::flightRequestDR_UASP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::tolPadAvailabilityDR_UASP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::trafficFlowsDR_UASP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::weatherInfoDR_UASP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::airspaceRestDR_UASP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::availabilityDR_SKO] = Subscribers::skyportOperatorSub;
  this->subscribers_[ DataReaders::flightCoordDR_SKO] = Subscribers::skyportOperatorSub;
  this->subscribers_[ DataReaders::reader] = Subscribers::anspSub;
  this->subscribers_[ DataReaders::reader] = Subscribers::ussSub;

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

  participant = Participants::skyportOperatorDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;

  participant = Participants::weatherDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;

  participant = Participants::anspDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;

  participant = Participants::ussDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;
}

inline
void
Elements::Data::buildTopicsQos()
{
  TopicQos       topicQos;
  Topics::Values topic;
    
  topic    = Topics::UATM__uatmDCPS__trafficFlowsInfo;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = TRANSIENT_LOCAL_DURABILITY_QOS;
  topicQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
  topicQos.reliability.kind = RELIABLE_RELIABILITY_QOS;
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 50000000;
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__weatherInfo;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = TRANSIENT_LOCAL_DURABILITY_QOS;
  topicQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
  topicQos.reliability.kind = BEST_EFFORT_RELIABILITY_QOS;
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 200000000;
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__airspaceRestrictions;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = TRANSIENT_DURABILITY_QOS;
  topicQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
  topicQos.reliability.kind = RELIABLE_RELIABILITY_QOS;
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 50000000;
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__acceptableRoute;
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

  publisher    = Publishers::weatherPub;
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

  subscriber    = Subscribers::skyportOperatorSub;
  subscriberQos = TheServiceParticipant->initial_SubscriberQos();
  this->subscribersQos_[ subscriber] = subscriberQos;

  subscriber    = Subscribers::anspSub;
  subscriberQos = TheServiceParticipant->initial_SubscriberQos();
  this->subscribersQos_[ subscriber] = subscriberQos;

  subscriber    = Subscribers::ussSub;
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
  this->writerCopyTopicQos_[writer] = false;

  writer    = DataWriters::flightRoutesDW_SKO;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = false;

  writer    = DataWriters::trafficFlowsDW_SKO;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = true;

  writer    = DataWriters::airspaceRestDW_SKO;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = true;

  writer    = DataWriters::writer;
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

  reader    = DataReaders::availabilityDR_SKO;
  readerQos = TheServiceParticipant->initial_DataReaderQos();
  this->readersQos_[ reader] = readerQos;
  this->readerCopyTopicQos_[reader] = false;

  reader    = DataReaders::flightCoordDR_SKO;
  readerQos = TheServiceParticipant->initial_DataReaderQos();
  this->readersQos_[ reader] = readerQos;
  this->readerCopyTopicQos_[reader] = false;

  reader    = DataReaders::reader;
  readerQos = TheServiceParticipant->initial_DataReaderQos();
  this->readersQos_[ reader] = readerQos;
  this->readerCopyTopicQos_[reader] = true;

  reader    = DataReaders::reader;
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

    case DataWriters::flightRoutesDW_SKO:
      break;

    case DataWriters::trafficFlowsDW_SKO:
      break;

    case DataWriters::airspaceRestDW_SKO:
      break;

    case DataWriters::writer:
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

    case DataReaders::availabilityDR_SKO:
      break;

    case DataReaders::flightCoordDR_SKO:
      break;

    case DataReaders::reader:
      break;

    case DataReaders::reader:
      break;

    default:
      throw NoReaderException();
  }
}
} // End namespace uatmDCPS
} // End namespace UATM

} // End namespace Model
} // End namespace OpenDDS
