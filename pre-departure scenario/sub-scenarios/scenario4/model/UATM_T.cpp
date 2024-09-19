
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
  for( int index = 0;
       index < Elements::ContentFilteredTopics::LAST_INDEX;
       ++index) {
    this->filterExpressions_[index] = 0;
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
    case Types::availabilityInfo:
      {
        typedef ::UATM::availabilityInfoTypeSupportImpl TypeSupport;

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

    case Types::tolPadRequest:
      {
        typedef ::UATM::tolPadRequestTypeSupportImpl TypeSupport;

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
  this->domains_[ Participants::tolPadManagerDP] = 0;
}

inline
void
Elements::Data::loadTopics()
{
  this->topicNames_[Topics::UATM__uatmDCPS__AvailabilityInfo] = "AvailabilityInfo";
  this->cfTopics_[Topics::UATM__uatmDCPS__AvailabilityInfo] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__AvailabilityInfo] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__tolPadRequest] = "tolPadRequest";
  this->cfTopics_[Topics::UATM__uatmDCPS__tolPadRequest] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__tolPadRequest] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__availabiityInfo_UASP] = "availabiityInfo_UASP";
  this->cfTopics_[Topics::UATM__uatmDCPS__availabiityInfo_UASP] = ContentFilteredTopics::UATM__uatmDCPS__availabiityInfo_UASP;
  this->multiTopics_[Topics::UATM__uatmDCPS__availabiityInfo_UASP] = MultiTopics::LAST_INDEX;
  this->filterExpressions_[ContentFilteredTopics::UATM__uatmDCPS__availabiityInfo_UASP] = "resource_type == "tolPad"";
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
  this->publisherParticipants_[ Publishers::uaspManagerPub] = Participants::uaspManagerDP;
  this->publisherParticipants_[ Publishers::tolPadManPub] = Participants::tolPadManagerDP;

  this->subscriberParticipants_[ Subscribers::uaspManagerSub] = Participants::uaspManagerDP;
  this->subscriberParticipants_[ Subscribers::tolPadManSub] = Participants::tolPadManagerDP;

  this->types_[ Topics::UATM__uatmDCPS__AvailabilityInfo] = Types::availabilityInfo;
  this->types_[ Topics::UATM__uatmDCPS__tolPadRequest] = Types::tolPadRequest;
  this->relatedTopics_[ContentFilteredTopics::UATM__uatmDCPS__availabiityInfo_UASP] = Topics::UATM__uatmDCPS__AvailabilityInfo;

  this->writerTopics_[ DataWriters::tolPadReqDW_UASP] = Topics::UATM__uatmDCPS__tolPadRequest;
  this->writerTopics_[ DataWriters::tolPadAvailabilityDW_TP] = Topics::UATM__uatmDCPS__AvailabilityInfo;

  this->readerTopics_[ DataReaders::tolPadAvailabilityDR_UASP] = Topics::UATM__uatmDCPS__availabiityInfo_UASP;
  this->readerTopics_[ DataReaders::tolPadAssignDR_TP] = Topics::UATM__uatmDCPS__tolPadRequest;

  this->publishers_[ DataWriters::routeDataDW_UASP] = Publishers::uaspManagerPub;
  this->publishers_[ DataWriters::flightAuthDW_UASP] = Publishers::uaspManagerPub;
  this->publishers_[ DataWriters::changeRecDW_UASP] = Publishers::uaspManagerPub;
  this->publishers_[ DataWriters::tolPadReqDW_UASP] = Publishers::uaspManagerPub;
  this->publishers_[ DataWriters::tolPadAvailabilityDW_TP] = Publishers::tolPadManPub;

  this->subscribers_[ DataReaders::flightRequestDR_UASP] = Subscribers::uaspManagerSub;
  this->subscribers_[ DataReaders::tolPadAvailabilityDR_UASP] = Subscribers::uaspManagerSub;
  this->subscribers_[ DataReaders::trafficFlowsDR_UASP] = Subscribers::uaspManagerSub;
  this->subscribers_[ DataReaders::weatherInfoDR_UASP] = Subscribers::uaspManagerSub;
  this->subscribers_[ DataReaders::airspaceRestDR_UASP] = Subscribers::uaspManagerSub;
  this->subscribers_[ DataReaders::tolPadAssignDR_TP] = Subscribers::tolPadManSub;

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

  participant = Participants::tolPadManagerDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;
}

inline
void
Elements::Data::buildTopicsQos()
{
  TopicQos       topicQos;
  Topics::Values topic;
    
  topic    = Topics::UATM__uatmDCPS__AvailabilityInfo;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = TRANSIENT_LOCAL_DURABILITY_QOS;
  topicQos.ownership.kind = SHARED_OWNERSHIP_QOS;
  topicQos.reliability.kind = BEST_EFFORT_RELIABILITY_QOS;
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 100000000;
  topicQos.history.depth = 10;
  topicQos.history.kind = KEEP_LAST_HISTORY_QOS;
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__tolPadRequest;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = TRANSIENT_LOCAL_DURABILITY_QOS;
  topicQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
  topicQos.reliability.kind = RELIABLE_RELIABILITY_QOS;
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 50000000;
  topicQos.history.depth = 5;
  topicQos.history.kind = KEEP_LAST_HISTORY_QOS;
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__availabiityInfo_UASP;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
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

  publisher    = Publishers::tolPadManPub;
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

  subscriber    = Subscribers::tolPadManSub;
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

  writer    = DataWriters::tolPadAvailabilityDW_TP;
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

  reader    = DataReaders::tolPadAssignDR_TP;
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

    case DataWriters::tolPadAvailabilityDW_TP:
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

    case DataReaders::tolPadAssignDR_TP:
      break;

    default:
      throw NoReaderException();
  }
}
} // End namespace uatmDCPS
} // End namespace UATM

} // End namespace Model
} // End namespace OpenDDS
