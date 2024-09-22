
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

    case Types::airspaceRestrictions:
      {
        typedef ::UATM::airspaceRestrictionsTypeSupportImpl TypeSupport;

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

    case Types::bookingFlightRequest:
      {
        typedef ::UATM::bookingFlightRequestTypeSupportImpl TypeSupport;

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

    case Types::flightAssign:
      {
        typedef ::UATM::flightAssignTypeSupportImpl TypeSupport;

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

    case Types::flightRoutesInfo:
      {
        typedef ::UATM::flightRoutesInfoTypeSupportImpl TypeSupport;

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
  this->domains_[ Participants::anspDP] = 0;
  this->domains_[ Participants::ussDP] = 0;
  this->domains_[ Participants::weatherDP] = 0;
  this->domains_[ Participants::bookingPlatformDP] = 0;
  this->domains_[ Participants::evtolManagerDP] = 0;
  this->domains_[ Participants::pilotManagerDP] = 0;
  this->domains_[ Participants::skyportManDP] = 0;
  this->domains_[ Participants::tolPadManagerDP] = 0;
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
  this->topicNames_[Topics::UATM__uatmDCPS__tolPadRequest] = "tolPadRequest";
  this->cfTopics_[Topics::UATM__uatmDCPS__tolPadRequest] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__tolPadRequest] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__flightRouteInfo] = "flightRouteInfo";
  this->cfTopics_[Topics::UATM__uatmDCPS__flightRouteInfo] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__flightRouteInfo] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__weatherInfo] = "weatherInfo";
  this->cfTopics_[Topics::UATM__uatmDCPS__weatherInfo] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__weatherInfo] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__trafficFlowsInfo] = "trafficFlowsInfo";
  this->cfTopics_[Topics::UATM__uatmDCPS__trafficFlowsInfo] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__trafficFlowsInfo] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__flightChangeRec] = "flightChangeRec";
  this->cfTopics_[Topics::UATM__uatmDCPS__flightChangeRec] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__flightChangeRec] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__flightAuthorization] = "flightAuthorization";
  this->cfTopics_[Topics::UATM__uatmDCPS__flightAuthorization] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__flightAuthorization] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__flightAssign] = "flightAssign";
  this->cfTopics_[Topics::UATM__uatmDCPS__flightAssign] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__flightAssign] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__bookingFlightRequest] = "bookingFlightRequest";
  this->cfTopics_[Topics::UATM__uatmDCPS__bookingFlightRequest] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__bookingFlightRequest] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__availabilityInfo] = "availabilityInfo";
  this->cfTopics_[Topics::UATM__uatmDCPS__availabilityInfo] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__availabilityInfo] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__airspaceRestrictions] = "airspaceRestrictions";
  this->cfTopics_[Topics::UATM__uatmDCPS__airspaceRestrictions] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__airspaceRestrictions] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__accceptableRoute] = "accceptableRoute";
  this->cfTopics_[Topics::UATM__uatmDCPS__accceptableRoute] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__accceptableRoute] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__availabilityInfo_FOP] = "availabilityInfo_FOP";
  this->cfTopics_[Topics::UATM__uatmDCPS__availabilityInfo_FOP] = ContentFilteredTopics::UATM__uatmDCPS__availabilityInfo_FOP;
  this->multiTopics_[Topics::UATM__uatmDCPS__availabilityInfo_FOP] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__availabilityInfo_UASP] = "availabilityInfo_UASP";
  this->cfTopics_[Topics::UATM__uatmDCPS__availabilityInfo_UASP] = ContentFilteredTopics::UATM__uatmDCPS__availabilityInfo_UASP;
  this->multiTopics_[Topics::UATM__uatmDCPS__availabilityInfo_UASP] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__availabilityInfo_SKO] = "availabilityInfo_SKO";
  this->cfTopics_[Topics::UATM__uatmDCPS__availabilityInfo_SKO] = ContentFilteredTopics::UATM__uatmDCPS__availabilityInfo_SKO;
  this->multiTopics_[Topics::UATM__uatmDCPS__availabilityInfo_SKO] = MultiTopics::LAST_INDEX;
  this->filterExpressions_[ContentFilteredTopics::UATM__uatmDCPS__availabilityInfo_FOP] = "resource_type LIKE 'pilot' or resource_type LIKE 'evtol' or resource_type LIKE 'skyport'  ";
  this->filterExpressions_[ContentFilteredTopics::UATM__uatmDCPS__availabilityInfo_UASP] = "resource_type LIKE 'tolPad'";
  this->filterExpressions_[ContentFilteredTopics::UATM__uatmDCPS__availabilityInfo_SKO] = "resource_type LIKE 'skyport'";
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
  this->publisherParticipants_[ Publishers::weatherPub] = Participants::weatherDP;
  this->publisherParticipants_[ Publishers::bookingPlatformPub] = Participants::bookingPlatformDP;
  this->publisherParticipants_[ Publishers::evtolManPub] = Participants::evtolManagerDP;
  this->publisherParticipants_[ Publishers::pilotManPub] = Participants::pilotManagerDP;
  this->publisherParticipants_[ Publishers::SkyportManPub] = Participants::skyportManDP;
  this->publisherParticipants_[ Publishers::tolPadManPub] = Participants::tolPadManagerDP;

  this->subscriberParticipants_[ Subscribers::fleetOperatorSub] = Participants::fleetOperatorDP;
  this->subscriberParticipants_[ Subscribers::skyportOperatorSub] = Participants::skyportOperatorDP;
  this->subscriberParticipants_[ Subscribers::flightAuthSysSub] = Participants::flightAuthSysDP;
  this->subscriberParticipants_[ Subscribers::uaspManSub] = Participants::uaspManagerDP;
  this->subscriberParticipants_[ Subscribers::anspSub] = Participants::anspDP;
  this->subscriberParticipants_[ Subscribers::ussSub] = Participants::ussDP;
  this->subscriberParticipants_[ Subscribers::evtolManSub] = Participants::evtolManagerDP;
  this->subscriberParticipants_[ Subscribers::pilotManSub] = Participants::pilotManagerDP;
  this->subscriberParticipants_[ Subscribers::tolPadManSub] = Participants::tolPadManagerDP;

  this->types_[ Topics::UATM__uatmDCPS__flightCoordination] = Types::flightCoordination;
  this->types_[ Topics::UATM__uatmDCPS__flightAuthorizationRequest] = Types::flightAuthorizationRequest;
  this->types_[ Topics::UATM__uatmDCPS__flightRequestInfo] = Types::flightRequestInfo;
  this->types_[ Topics::UATM__uatmDCPS__tolPadRequest] = Types::tolPadRequest;
  this->types_[ Topics::UATM__uatmDCPS__flightRouteInfo] = Types::flightRoutesInfo;
  this->types_[ Topics::UATM__uatmDCPS__weatherInfo] = Types::weatherInfo;
  this->types_[ Topics::UATM__uatmDCPS__trafficFlowsInfo] = Types::trafficFlowsInfo;
  this->types_[ Topics::UATM__uatmDCPS__flightChangeRec] = Types::flightChangeRec;
  this->types_[ Topics::UATM__uatmDCPS__flightAuthorization] = Types::flightAuthorization;
  this->types_[ Topics::UATM__uatmDCPS__flightAssign] = Types::flightAssign;
  this->types_[ Topics::UATM__uatmDCPS__bookingFlightRequest] = Types::bookingFlightRequest;
  this->types_[ Topics::UATM__uatmDCPS__availabilityInfo] = Types::availabilityInfo;
  this->types_[ Topics::UATM__uatmDCPS__airspaceRestrictions] = Types::airspaceRestrictions;
  this->types_[ Topics::UATM__uatmDCPS__accceptableRoute] = Types::acceptableRoute;
  this->relatedTopics_[ContentFilteredTopics::UATM__uatmDCPS__availabilityInfo_FOP] = Topics::UATM__uatmDCPS__availabilityInfo;
  this->relatedTopics_[ContentFilteredTopics::UATM__uatmDCPS__availabilityInfo_UASP] = Topics::UATM__uatmDCPS__availabilityInfo;
  this->relatedTopics_[ContentFilteredTopics::UATM__uatmDCPS__availabilityInfo_SKO] = Topics::UATM__uatmDCPS__availabilityInfo;

  this->writerTopics_[ DataWriters::uaspFlightRequestDW_FOP] = Topics::UATM__uatmDCPS__flightAuthorizationRequest;
  this->writerTopics_[ DataWriters::assignFlightDW_FOP] = Topics::UATM__uatmDCPS__flightAssign;
  this->writerTopics_[ DataWriters::flightCoordDW_FOP] = Topics::UATM__uatmDCPS__flightCoordination;
  this->writerTopics_[ DataWriters::flightRoutesDW_SKO] = Topics::UATM__uatmDCPS__flightRouteInfo;
  this->writerTopics_[ DataWriters::trafficFlowsDW_SKO] = Topics::UATM__uatmDCPS__trafficFlowsInfo;
  this->writerTopics_[ DataWriters::airspaceRestDW_SKO] = Topics::UATM__uatmDCPS__airspaceRestrictions;
  this->writerTopics_[ DataWriters::flightRequestInfoDW_FAS] = Topics::UATM__uatmDCPS__flightRequestInfo;
  this->writerTopics_[ DataWriters::routeDataDW_UASP] = Topics::UATM__uatmDCPS__accceptableRoute;
  this->writerTopics_[ DataWriters::flightAuthDW_UASP] = Topics::UATM__uatmDCPS__flightAuthorization;
  this->writerTopics_[ DataWriters::changeRecDW_UASP] = Topics::UATM__uatmDCPS__flightChangeRec;
  this->writerTopics_[ DataWriters::tolPadReqDW_UASP] = Topics::UATM__uatmDCPS__tolPadRequest;
  this->writerTopics_[ DataWriters::weatherInfoDW_WTR] = Topics::UATM__uatmDCPS__weatherInfo;
  this->writerTopics_[ DataWriters::bookingFlightRequestDW_BP] = Topics::UATM__uatmDCPS__bookingFlightRequest;
  this->writerTopics_[ DataWriters::evtolAvailabilityDW_EV] = Topics::UATM__uatmDCPS__availabilityInfo;
  this->writerTopics_[ DataWriters::pilotAvailabilityDW_PLM] = Topics::UATM__uatmDCPS__availabilityInfo;
  this->writerTopics_[ DataWriters::skyportAvailabilityDW_SKM] = Topics::UATM__uatmDCPS__availabilityInfo;
  this->writerTopics_[ DataWriters::tolPadAvailabilityDW_TP] = Topics::UATM__uatmDCPS__availabilityInfo;

  this->readerTopics_[ DataReaders::flightRequestDR_FOP] = Topics::UATM__uatmDCPS__bookingFlightRequest;
  this->readerTopics_[ DataReaders::availabilityDR_FOP] = Topics::UATM__uatmDCPS__availabilityInfo_FOP;
  this->readerTopics_[ DataReaders::FlightRoutesDR_FOP] = Topics::UATM__uatmDCPS__flightRouteInfo;
  this->readerTopics_[ DataReaders::weatherDR_FOP] = Topics::UATM__uatmDCPS__weatherInfo;
  this->readerTopics_[ DataReaders::flightAuthDR_FOP] = Topics::UATM__uatmDCPS__flightAuthorization;
  this->readerTopics_[ DataReaders::recommendationDR_FOP] = Topics::UATM__uatmDCPS__flightChangeRec;
  this->readerTopics_[ DataReaders::availabilityDR_SKO] = Topics::UATM__uatmDCPS__availabilityInfo_SKO;
  this->readerTopics_[ DataReaders::flightCoordDR_SKO] = Topics::UATM__uatmDCPS__flightCoordination;
  this->readerTopics_[ DataReaders::flightRequestDR_FAS] = Topics::UATM__uatmDCPS__flightAuthorizationRequest;
  this->readerTopics_[ DataReaders::flightRequestDR_UASP] = Topics::UATM__uatmDCPS__flightRequestInfo;
  this->readerTopics_[ DataReaders::tolPadAvailabilityDR_UASP] = Topics::UATM__uatmDCPS__availabilityInfo_UASP;
  this->readerTopics_[ DataReaders::trafficFlowsDR_UASP] = Topics::UATM__uatmDCPS__trafficFlowsInfo;
  this->readerTopics_[ DataReaders::weatherInfoDR_UASP] = Topics::UATM__uatmDCPS__weatherInfo;
  this->readerTopics_[ DataReaders::airspaceRestDR_UASP] = Topics::UATM__uatmDCPS__airspaceRestrictions;
  this->readerTopics_[ DataReaders::routeDataDR_ANSP] = Topics::UATM__uatmDCPS__accceptableRoute;
  this->readerTopics_[ DataReaders::routeDataDR_USS] = Topics::UATM__uatmDCPS__accceptableRoute;
  this->readerTopics_[ DataReaders::flightAssignDR_EV] = Topics::UATM__uatmDCPS__flightAssign;
  this->readerTopics_[ DataReaders::flightAuthDR_PLM] = Topics::UATM__uatmDCPS__flightAuthorization;
  this->readerTopics_[ DataReaders::changeRecDR_PLM] = Topics::UATM__uatmDCPS__flightChangeRec;
  this->readerTopics_[ DataReaders::flightAssignDR_PLM] = Topics::UATM__uatmDCPS__flightAssign;
  this->readerTopics_[ DataReaders::tolPadAssignDR_TP] = Topics::UATM__uatmDCPS__tolPadRequest;

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
  this->publishers_[ DataWriters::weatherInfoDW_WTR] = Publishers::weatherPub;
  this->publishers_[ DataWriters::bookingFlightRequestDW_BP] = Publishers::bookingPlatformPub;
  this->publishers_[ DataWriters::evtolAvailabilityDW_EV] = Publishers::evtolManPub;
  this->publishers_[ DataWriters::pilotAvailabilityDW_PLM] = Publishers::pilotManPub;
  this->publishers_[ DataWriters::skyportAvailabilityDW_SKM] = Publishers::SkyportManPub;
  this->publishers_[ DataWriters::tolPadAvailabilityDW_TP] = Publishers::tolPadManPub;

  this->subscribers_[ DataReaders::flightRequestDR_FOP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::availabilityDR_FOP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::FlightRoutesDR_FOP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::weatherDR_FOP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::flightAuthDR_FOP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::recommendationDR_FOP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::availabilityDR_SKO] = Subscribers::skyportOperatorSub;
  this->subscribers_[ DataReaders::flightCoordDR_SKO] = Subscribers::skyportOperatorSub;
  this->subscribers_[ DataReaders::flightRequestDR_FAS] = Subscribers::flightAuthSysSub;
  this->subscribers_[ DataReaders::flightRequestDR_UASP] = Subscribers::uaspManSub;
  this->subscribers_[ DataReaders::tolPadAvailabilityDR_UASP] = Subscribers::uaspManSub;
  this->subscribers_[ DataReaders::trafficFlowsDR_UASP] = Subscribers::uaspManSub;
  this->subscribers_[ DataReaders::weatherInfoDR_UASP] = Subscribers::uaspManSub;
  this->subscribers_[ DataReaders::airspaceRestDR_UASP] = Subscribers::uaspManSub;
  this->subscribers_[ DataReaders::routeDataDR_ANSP] = Subscribers::anspSub;
  this->subscribers_[ DataReaders::routeDataDR_USS] = Subscribers::ussSub;
  this->subscribers_[ DataReaders::flightAssignDR_EV] = Subscribers::evtolManSub;
  this->subscribers_[ DataReaders::flightAuthDR_PLM] = Subscribers::pilotManSub;
  this->subscribers_[ DataReaders::changeRecDR_PLM] = Subscribers::pilotManSub;
  this->subscribers_[ DataReaders::flightAssignDR_PLM] = Subscribers::pilotManSub;
  this->subscribers_[ DataReaders::tolPadAssignDR_TP] = Subscribers::tolPadManSub;

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

  participant = Participants::anspDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;

  participant = Participants::ussDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;

  participant = Participants::weatherDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;

  participant = Participants::bookingPlatformDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;

  participant = Participants::evtolManagerDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;

  participant = Participants::pilotManagerDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;

  participant = Participants::skyportManDP;
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
    
  topic    = Topics::UATM__uatmDCPS__flightCoordination;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = TRANSIENT_DURABILITY_QOS;
  topicQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
  topicQos.reliability.kind = RELIABLE_RELIABILITY_QOS;
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 50000000;
  topicQos.history.depth = 5;
  topicQos.history.kind = KEEP_LAST_HISTORY_QOS;
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
  topicQos.history.depth = 5;
  topicQos.history.kind = KEEP_LAST_HISTORY_QOS;
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

  topic    = Topics::UATM__uatmDCPS__flightRouteInfo;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = TRANSIENT_DURABILITY_QOS;
  topicQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
  topicQos.reliability.kind = RELIABLE_RELIABILITY_QOS;
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 50000000;
  topicQos.history.depth = 5;
  topicQos.history.kind = KEEP_LAST_HISTORY_QOS;
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__weatherInfo;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = TRANSIENT_LOCAL_DURABILITY_QOS;
  topicQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
  topicQos.reliability.kind = RELIABLE_RELIABILITY_QOS; //mudar aq BEST_EFFORT_RELIABILITY_QOS
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 200000000;
  topicQos.history.depth = 5;
  topicQos.history.kind = KEEP_LAST_HISTORY_QOS;
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__trafficFlowsInfo;
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

  topic    = Topics::UATM__uatmDCPS__flightChangeRec;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = TRANSIENT_DURABILITY_QOS;
  topicQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
  topicQos.reliability.kind = RELIABLE_RELIABILITY_QOS;
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 50000000;
  topicQos.history.depth = 10;
  topicQos.history.kind = KEEP_LAST_HISTORY_QOS;
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__flightAuthorization;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = PERSISTENT_DURABILITY_QOS;
  topicQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
  topicQos.reliability.kind = RELIABLE_RELIABILITY_QOS;
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 50000000;
  topicQos.history.depth = 1;
  topicQos.history.kind = KEEP_ALL_HISTORY_QOS;
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__flightAssign;
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

  topic    = Topics::UATM__uatmDCPS__bookingFlightRequest;
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

  topic    = Topics::UATM__uatmDCPS__availabilityInfo;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = TRANSIENT_LOCAL_DURABILITY_QOS;
  topicQos.ownership.kind = SHARED_OWNERSHIP_QOS;
  topicQos.reliability.kind = RELIABLE_RELIABILITY_QOS; //mudar aq tbm
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 100000000;
  topicQos.history.depth = 10;
  topicQos.history.kind = KEEP_LAST_HISTORY_QOS;
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
  topicQos.history.depth = 5;
  topicQos.history.kind = KEEP_LAST_HISTORY_QOS;
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__accceptableRoute;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = TRANSIENT_DURABILITY_QOS;
  topicQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
  topicQos.reliability.kind = RELIABLE_RELIABILITY_QOS;
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 50000000;
  topicQos.history.depth = 5;
  topicQos.history.kind = KEEP_LAST_HISTORY_QOS;
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__availabilityInfo_FOP;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__availabilityInfo_UASP;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__availabilityInfo_SKO;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
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

  publisher    = Publishers::weatherPub;
  publisherQos = TheServiceParticipant->initial_PublisherQos();
  this->publishersQos_[ publisher] = publisherQos;

  publisher    = Publishers::bookingPlatformPub;
  publisherQos = TheServiceParticipant->initial_PublisherQos();
  this->publishersQos_[ publisher] = publisherQos;

  publisher    = Publishers::evtolManPub;
  publisherQos = TheServiceParticipant->initial_PublisherQos();
  this->publishersQos_[ publisher] = publisherQos;

  publisher    = Publishers::pilotManPub;
  publisherQos = TheServiceParticipant->initial_PublisherQos();
  this->publishersQos_[ publisher] = publisherQos;

  publisher    = Publishers::SkyportManPub;
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

  subscriber    = Subscribers::fleetOperatorSub;
  subscriberQos = TheServiceParticipant->initial_SubscriberQos();
  this->subscribersQos_[ subscriber] = subscriberQos;

  subscriber    = Subscribers::skyportOperatorSub;
  subscriberQos = TheServiceParticipant->initial_SubscriberQos();
  this->subscribersQos_[ subscriber] = subscriberQos;

  subscriber    = Subscribers::flightAuthSysSub;
  subscriberQos = TheServiceParticipant->initial_SubscriberQos();
  this->subscribersQos_[ subscriber] = subscriberQos;

  subscriber    = Subscribers::uaspManSub;
  subscriberQos = TheServiceParticipant->initial_SubscriberQos();
  this->subscribersQos_[ subscriber] = subscriberQos;

  subscriber    = Subscribers::anspSub;
  subscriberQos = TheServiceParticipant->initial_SubscriberQos();
  this->subscribersQos_[ subscriber] = subscriberQos;

  subscriber    = Subscribers::ussSub;
  subscriberQos = TheServiceParticipant->initial_SubscriberQos();
  this->subscribersQos_[ subscriber] = subscriberQos;

  subscriber    = Subscribers::evtolManSub;
  subscriberQos = TheServiceParticipant->initial_SubscriberQos();
  this->subscribersQos_[ subscriber] = subscriberQos;

  subscriber    = Subscribers::pilotManSub;
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

  writer    = DataWriters::weatherInfoDW_WTR;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = true;

  writer    = DataWriters::bookingFlightRequestDW_BP;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = true;

  writer    = DataWriters::evtolAvailabilityDW_EV;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = true;

  writer    = DataWriters::pilotAvailabilityDW_PLM;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = true;

  writer    = DataWriters::skyportAvailabilityDW_SKM;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = true;

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

  reader    = DataReaders::routeDataDR_ANSP;
  readerQos = TheServiceParticipant->initial_DataReaderQos();
  this->readersQos_[ reader] = readerQos;
  this->readerCopyTopicQos_[reader] = true;

  reader    = DataReaders::routeDataDR_USS;
  readerQos = TheServiceParticipant->initial_DataReaderQos();
  this->readersQos_[ reader] = readerQos;
  this->readerCopyTopicQos_[reader] = true;

  reader    = DataReaders::flightAssignDR_EV;
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

  reader    = DataReaders::flightAssignDR_PLM;
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

    case DataWriters::weatherInfoDW_WTR:
      break;

    case DataWriters::bookingFlightRequestDW_BP:
      break;

    case DataWriters::evtolAvailabilityDW_EV:
      break;

    case DataWriters::pilotAvailabilityDW_PLM:
      break;

    case DataWriters::skyportAvailabilityDW_SKM:
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

    case DataReaders::routeDataDR_ANSP:
      break;

    case DataReaders::routeDataDR_USS:
      break;

    case DataReaders::flightAssignDR_EV:
      break;

    case DataReaders::flightAuthDR_PLM:
      break;

    case DataReaders::changeRecDR_PLM:
      break;

    case DataReaders::flightAssignDR_PLM:
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
