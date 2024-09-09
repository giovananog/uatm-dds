
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

    case Types::flightChanceRec:
      {
        typedef ::UATM::flightChanceRecTypeSupportImpl TypeSupport;

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

  default:
    throw NoTypeException();
    break;
  }
}

inline
void
Elements::Data::loadDomains()
{
  this->domains_[ Participants::bookingPlatformDP] = 0;
  this->domains_[ Participants::fleetOperatorDP] = 0;
  this->domains_[ Participants::evtolManagerDP] = 0;
  this->domains_[ Participants::pilotManagerDP] = 0;
  this->domains_[ Participants::weatherSystemDP] = 0;
  this->domains_[ Participants::skyportOperatorDP] = 0;
  this->domains_[ Participants::skyportsManDP] = 0;
  this->domains_[ Participants::uaspManagerDP] = 0;
  this->domains_[ Participants::FlighAuthSysDP] = 0;
  this->domains_[ Participants::anspDP] = 0;
  this->domains_[ Participants::ussDP] = 0;
  this->domains_[ Participants::tolPadManagerDP] = 0;
}

inline
void
Elements::Data::loadTopics()
{
  this->topicNames_[Topics::UATM__uatmDCPS__BookingFlightRequest] = "BookingFlightRequest";
  this->cfTopics_[Topics::UATM__uatmDCPS__BookingFlightRequest] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__BookingFlightRequest] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__AvailabilityInfo] = "AvailabilityInfo";
  this->cfTopics_[Topics::UATM__uatmDCPS__AvailabilityInfo] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__AvailabilityInfo] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__weatherInfo] = "weatherInfo";
  this->cfTopics_[Topics::UATM__uatmDCPS__weatherInfo] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__weatherInfo] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__FlightRoutesInfo] = "FlightRoutesInfo";
  this->cfTopics_[Topics::UATM__uatmDCPS__FlightRoutesInfo] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__FlightRoutesInfo] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__FlightAssign] = "FlightAssign";
  this->cfTopics_[Topics::UATM__uatmDCPS__FlightAssign] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__FlightAssign] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__FlightCoordination] = "FlightCoordination";
  this->cfTopics_[Topics::UATM__uatmDCPS__FlightCoordination] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__FlightCoordination] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__FlightAuthorizationRequest] = "FlightAuthorizationRequest";
  this->cfTopics_[Topics::UATM__uatmDCPS__FlightAuthorizationRequest] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__FlightAuthorizationRequest] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__TolPadRequest] = "TolPadRequest";
  this->cfTopics_[Topics::UATM__uatmDCPS__TolPadRequest] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__TolPadRequest] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__TrafficFlowsInfo] = "TrafficFlowsInfo";
  this->cfTopics_[Topics::UATM__uatmDCPS__TrafficFlowsInfo] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__TrafficFlowsInfo] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__AirspaceRestrictions] = "AirspaceRestrictions";
  this->cfTopics_[Topics::UATM__uatmDCPS__AirspaceRestrictions] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__AirspaceRestrictions] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__acceptableRoute] = "acceptableRoute";
  this->cfTopics_[Topics::UATM__uatmDCPS__acceptableRoute] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__acceptableRoute] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__FlightAuthorization] = "FlightAuthorization";
  this->cfTopics_[Topics::UATM__uatmDCPS__FlightAuthorization] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__FlightAuthorization] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__FlightChangeRec] = "FlightChangeRec";
  this->cfTopics_[Topics::UATM__uatmDCPS__FlightChangeRec] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__FlightChangeRec] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__FlightRequestInfo] = "FlightRequestInfo";
  this->cfTopics_[Topics::UATM__uatmDCPS__FlightRequestInfo] = ContentFilteredTopics::LAST_INDEX;
  this->multiTopics_[Topics::UATM__uatmDCPS__FlightRequestInfo] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__AvailabilityFOP] = "AvailabilityFOP";
  this->cfTopics_[Topics::UATM__uatmDCPS__AvailabilityFOP] = ContentFilteredTopics::UATM__uatmDCPS__AvailabilityFOP;
  this->multiTopics_[Topics::UATM__uatmDCPS__AvailabilityFOP] = MultiTopics::LAST_INDEX;
  this->topicNames_[Topics::UATM__uatmDCPS__AvailabilitySKO] = "AvailabilitySKO";
  this->cfTopics_[Topics::UATM__uatmDCPS__AvailabilitySKO] = ContentFilteredTopics::UATM__uatmDCPS__AvailabilitySKO;
  this->multiTopics_[Topics::UATM__uatmDCPS__AvailabilitySKO] = MultiTopics::LAST_INDEX;
  this->filterExpressions_[ContentFilteredTopics::UATM__uatmDCPS__AvailabilityFOP] = "";
  this->filterExpressions_[ContentFilteredTopics::UATM__uatmDCPS__AvailabilitySKO] = "";
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
  participantTxCfgNames_[Participants::bookingPlatformDP] = "";
  writerTxCfgNames_[DataWriters::bookingFlightRequestDW_BP] = "";
}

inline
void
Elements::Data::loadMaps()
{
  this->publisherParticipants_[ Publishers::bookingPlatformPub] = Participants::bookingPlatformDP;
  this->publisherParticipants_[ Publishers::fleetOperatorPub] = Participants::fleetOperatorDP;
  this->publisherParticipants_[ Publishers::evtolManPub] = Participants::evtolManagerDP;
  this->publisherParticipants_[ Publishers::pilotManPub] = Participants::pilotManagerDP;
  this->publisherParticipants_[ Publishers::weatherSysPub] = Participants::weatherSystemDP;
  this->publisherParticipants_[ Publishers::skyportOperatorPub] = Participants::skyportOperatorDP;
  this->publisherParticipants_[ Publishers::skyportsManPub] = Participants::skyportsManDP;
  this->publisherParticipants_[ Publishers::uaspManPub] = Participants::uaspManagerDP;
  this->publisherParticipants_[ Publishers::FlightAuthSysPub] = Participants::FlighAuthSysDP;
  this->publisherParticipants_[ Publishers::tolPadManPub] = Participants::tolPadManagerDP;

  this->subscriberParticipants_[ Subscribers::fleetOperatorSub] = Participants::fleetOperatorDP;
  this->subscriberParticipants_[ Subscribers::evtolManSub] = Participants::evtolManagerDP;
  this->subscriberParticipants_[ Subscribers::pilotManSub] = Participants::pilotManagerDP;
  this->subscriberParticipants_[ Subscribers::skyportOperatorSub] = Participants::skyportOperatorDP;
  this->subscriberParticipants_[ Subscribers::skyportsManSub] = Participants::skyportsManDP;
  this->subscriberParticipants_[ Subscribers::uaspManSub] = Participants::uaspManagerDP;
  this->subscriberParticipants_[ Subscribers::FlightAuthSysSub] = Participants::FlighAuthSysDP;
  this->subscriberParticipants_[ Subscribers::anspSub] = Participants::anspDP;
  this->subscriberParticipants_[ Subscribers::ussSub] = Participants::ussDP;
  this->subscriberParticipants_[ Subscribers::tolPadManSub] = Participants::tolPadManagerDP;

  this->types_[ Topics::UATM__uatmDCPS__BookingFlightRequest] = Types::bookingFlightRequest;
  this->types_[ Topics::UATM__uatmDCPS__AvailabilityInfo] = Types::availabilityInfo;
  this->types_[ Topics::UATM__uatmDCPS__weatherInfo] = Types::weatherInfo;
  this->types_[ Topics::UATM__uatmDCPS__FlightRoutesInfo] = Types::flightRoutesInfo;
  this->types_[ Topics::UATM__uatmDCPS__FlightAssign] = Types::flightAssign;
  this->types_[ Topics::UATM__uatmDCPS__FlightCoordination] = Types::flightCoordination;
  this->types_[ Topics::UATM__uatmDCPS__FlightAuthorizationRequest] = Types::flightAuthorizationRequest;
  this->types_[ Topics::UATM__uatmDCPS__TolPadRequest] = Types::tolPadRequest;
  this->types_[ Topics::UATM__uatmDCPS__TrafficFlowsInfo] = Types::trafficFlowsInfo;
  this->types_[ Topics::UATM__uatmDCPS__AirspaceRestrictions] = Types::airspaceRestrictions;
  this->types_[ Topics::UATM__uatmDCPS__acceptableRoute] = Types::acceptableRoute;
  this->types_[ Topics::UATM__uatmDCPS__FlightAuthorization] = Types::flightAuthorization;
  this->types_[ Topics::UATM__uatmDCPS__FlightChangeRec] = Types::flightChanceRec;
  this->types_[ Topics::UATM__uatmDCPS__FlightRequestInfo] = Types::flightRequestInfo;
  this->relatedTopics_[ContentFilteredTopics::UATM__uatmDCPS__AvailabilityFOP] = Topics::UATM__uatmDCPS__AvailabilityInfo;
  this->relatedTopics_[ContentFilteredTopics::UATM__uatmDCPS__AvailabilitySKO] = Topics::UATM__uatmDCPS__AvailabilityInfo;

  this->writerTopics_[ DataWriters::bookingFlightRequestDW_BP] = Topics::UATM__uatmDCPS__BookingFlightRequest;
  this->writerTopics_[ DataWriters::uaspFlightRequestDW_FOP] = Topics::UATM__uatmDCPS__FlightAuthorizationRequest;
  this->writerTopics_[ DataWriters::assignFlightDW_FOP] = Topics::UATM__uatmDCPS__FlightAssign;
  this->writerTopics_[ DataWriters::flightCoordDW_FOP] = Topics::UATM__uatmDCPS__FlightCoordination;
  this->writerTopics_[ DataWriters::evtolAvailabilityDW_ev] = Topics::UATM__uatmDCPS__AvailabilityInfo;
  this->writerTopics_[ DataWriters::pilotAvailabilityDW_PLM] = Topics::UATM__uatmDCPS__AvailabilityInfo;
  this->writerTopics_[ DataWriters::weatherInfoDW_WTR] = Topics::UATM__uatmDCPS__weatherInfo;
  this->writerTopics_[ DataWriters::FlightRoutesDW_SKO] = Topics::UATM__uatmDCPS__FlightRoutesInfo;
  this->writerTopics_[ DataWriters::TrafficFlowDW_SKO] = Topics::UATM__uatmDCPS__TrafficFlowsInfo;
  this->writerTopics_[ DataWriters::airspaceRestDW_SKO] = Topics::UATM__uatmDCPS__AirspaceRestrictions;
  this->writerTopics_[ DataWriters::SkyportAvailabilityDW_SKM] = Topics::UATM__uatmDCPS__AvailabilityInfo;
  this->writerTopics_[ DataWriters::routeDataDW_UASP] = Topics::UATM__uatmDCPS__acceptableRoute;
  this->writerTopics_[ DataWriters::flightAuthDW_UASP] = Topics::UATM__uatmDCPS__FlightAuthorization;
  this->writerTopics_[ DataWriters::changeRecDW_UASP] = Topics::UATM__uatmDCPS__FlightChangeRec;
  this->writerTopics_[ DataWriters::tolPadReqDW_UASP] = Topics::UATM__uatmDCPS__TolPadRequest;
  this->writerTopics_[ DataWriters::FlightRequestInfoDW_FAS] = Topics::UATM__uatmDCPS__FlightRequestInfo;
  this->writerTopics_[ DataWriters::tolPadAvailabilityDW_TP] = Topics::UATM__uatmDCPS__AvailabilityInfo;

  this->readerTopics_[ DataReaders::flightRequestDR_FOP] = Topics::UATM__uatmDCPS__BookingFlightRequest;
  this->readerTopics_[ DataReaders::availabilityDR_FOP] = Topics::UATM__uatmDCPS__AvailabilityFOP;
  this->readerTopics_[ DataReaders::FlightRoutesDR_FOP] = Topics::UATM__uatmDCPS__FlightRoutesInfo;
  this->readerTopics_[ DataReaders::weatherDR_FOP] = Topics::UATM__uatmDCPS__weatherInfo;
  this->readerTopics_[ DataReaders::flightAuthDR_FOP] = Topics::UATM__uatmDCPS__FlightAuthorization;
  this->readerTopics_[ DataReaders::recommendationDR_FOP] = Topics::UATM__uatmDCPS__FlightChangeRec;
  this->readerTopics_[ DataReaders::flightAssignDR_EV] = Topics::UATM__uatmDCPS__FlightAssign;
  this->readerTopics_[ DataReaders::flightAuthDR_PLM] = Topics::UATM__uatmDCPS__FlightAuthorization;
  this->readerTopics_[ DataReaders::changeRecDR_PLM] = Topics::UATM__uatmDCPS__FlightChangeRec;
  this->readerTopics_[ DataReaders::flighAssignDR_PLM] = Topics::UATM__uatmDCPS__FlightAssign;
  this->readerTopics_[ DataReaders::availabilityDR_SKO] = Topics::UATM__uatmDCPS__AvailabilitySKO;
  this->readerTopics_[ DataReaders::flightCoordDR_SKO] = Topics::UATM__uatmDCPS__FlightCoordination;
  this->readerTopics_[ DataReaders::assignFlightDR_SKM] = Topics::UATM__uatmDCPS__FlightAssign;
  this->readerTopics_[ DataReaders::FlightRequestDR_UASP] = Topics::UATM__uatmDCPS__FlightRequestInfo;
  this->readerTopics_[ DataReaders::trafficFlowsDR_UASP] = Topics::UATM__uatmDCPS__TrafficFlowsInfo;
  this->readerTopics_[ DataReaders::weatherInfoDR_UASP] = Topics::UATM__uatmDCPS__weatherInfo;
  this->readerTopics_[ DataReaders::airspaceRestDR_UASP] = Topics::UATM__uatmDCPS__AirspaceRestrictions;
  this->readerTopics_[ DataReaders::tolPadAvailabilityDR_UASP] = Topics::UATM__uatmDCPS__AvailabilityInfo;
  this->readerTopics_[ DataReaders::flightRequestDR_FAS] = Topics::UATM__uatmDCPS__FlightAuthorizationRequest;
  this->readerTopics_[ DataReaders::routeDataDR_ANSP] = Topics::UATM__uatmDCPS__acceptableRoute;
  this->readerTopics_[ DataReaders::routeDataDR_USS] = Topics::UATM__uatmDCPS__acceptableRoute;
  this->readerTopics_[ DataReaders::tolPadAssignDR_TP] = Topics::UATM__uatmDCPS__TolPadRequest;

  this->publishers_[ DataWriters::bookingFlightRequestDW_BP] = Publishers::bookingPlatformPub;
  this->publishers_[ DataWriters::uaspFlightRequestDW_FOP] = Publishers::fleetOperatorPub;
  this->publishers_[ DataWriters::assignFlightDW_FOP] = Publishers::fleetOperatorPub;
  this->publishers_[ DataWriters::flightCoordDW_FOP] = Publishers::fleetOperatorPub;
  this->publishers_[ DataWriters::evtolAvailabilityDW_ev] = Publishers::evtolManPub;
  this->publishers_[ DataWriters::pilotAvailabilityDW_PLM] = Publishers::pilotManPub;
  this->publishers_[ DataWriters::weatherInfoDW_WTR] = Publishers::weatherSysPub;
  this->publishers_[ DataWriters::FlightRoutesDW_SKO] = Publishers::skyportOperatorPub;
  this->publishers_[ DataWriters::TrafficFlowDW_SKO] = Publishers::skyportOperatorPub;
  this->publishers_[ DataWriters::airspaceRestDW_SKO] = Publishers::skyportOperatorPub;
  this->publishers_[ DataWriters::SkyportAvailabilityDW_SKM] = Publishers::skyportsManPub;
  this->publishers_[ DataWriters::routeDataDW_UASP] = Publishers::uaspManPub;
  this->publishers_[ DataWriters::flightAuthDW_UASP] = Publishers::uaspManPub;
  this->publishers_[ DataWriters::changeRecDW_UASP] = Publishers::uaspManPub;
  this->publishers_[ DataWriters::tolPadReqDW_UASP] = Publishers::uaspManPub;
  this->publishers_[ DataWriters::FlightRequestInfoDW_FAS] = Publishers::FlightAuthSysPub;
  this->publishers_[ DataWriters::tolPadAvailabilityDW_TP] = Publishers::tolPadManPub;

  this->subscribers_[ DataReaders::flightRequestDR_FOP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::availabilityDR_FOP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::FlightRoutesDR_FOP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::weatherDR_FOP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::flightAuthDR_FOP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::recommendationDR_FOP] = Subscribers::fleetOperatorSub;
  this->subscribers_[ DataReaders::flightAssignDR_EV] = Subscribers::evtolManSub;
  this->subscribers_[ DataReaders::flightAuthDR_PLM] = Subscribers::pilotManSub;
  this->subscribers_[ DataReaders::changeRecDR_PLM] = Subscribers::pilotManSub;
  this->subscribers_[ DataReaders::flighAssignDR_PLM] = Subscribers::pilotManSub;
  this->subscribers_[ DataReaders::availabilityDR_SKO] = Subscribers::skyportOperatorSub;
  this->subscribers_[ DataReaders::flightCoordDR_SKO] = Subscribers::skyportOperatorSub;
  this->subscribers_[ DataReaders::assignFlightDR_SKM] = Subscribers::skyportsManSub;
  this->subscribers_[ DataReaders::FlightRequestDR_UASP] = Subscribers::uaspManSub;
  this->subscribers_[ DataReaders::trafficFlowsDR_UASP] = Subscribers::uaspManSub;
  this->subscribers_[ DataReaders::weatherInfoDR_UASP] = Subscribers::uaspManSub;
  this->subscribers_[ DataReaders::airspaceRestDR_UASP] = Subscribers::uaspManSub;
  this->subscribers_[ DataReaders::tolPadAvailabilityDR_UASP] = Subscribers::uaspManSub;
  this->subscribers_[ DataReaders::flightRequestDR_FAS] = Subscribers::FlightAuthSysSub;
  this->subscribers_[ DataReaders::routeDataDR_ANSP] = Subscribers::anspSub;
  this->subscribers_[ DataReaders::routeDataDR_USS] = Subscribers::ussSub;
  this->subscribers_[ DataReaders::tolPadAssignDR_TP] = Subscribers::tolPadManSub;

}

inline
void
Elements::Data::buildParticipantsQos()
{
  DomainParticipantQos participantQos;
  Participants::Values participant;
    
  participant = Participants::bookingPlatformDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;

  participant = Participants::fleetOperatorDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;

  participant = Participants::evtolManagerDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;

  participant = Participants::pilotManagerDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;

  participant = Participants::weatherSystemDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;

  participant = Participants::skyportOperatorDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;

  participant = Participants::skyportsManDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;

  participant = Participants::uaspManagerDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;

  participant = Participants::FlighAuthSysDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;

  participant = Participants::anspDP;
  participantQos = TheServiceParticipant->initial_DomainParticipantQos();
  this->participantsQos_[ participant] = participantQos;

  participant = Participants::ussDP;
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
    
  topic    = Topics::UATM__uatmDCPS__BookingFlightRequest;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = TRANSIENT_LOCAL_DURABILITY_QOS;
  topicQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
  topicQos.reliability.kind = RELIABLE_RELIABILITY_QOS;
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 50000000;
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__AvailabilityInfo;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = TRANSIENT_LOCAL_DURABILITY_QOS;
  topicQos.ownership.kind = SHARED_OWNERSHIP_QOS;
  topicQos.reliability.kind = BEST_EFFORT_RELIABILITY_QOS;
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 100000000;
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

  topic    = Topics::UATM__uatmDCPS__FlightRoutesInfo;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = TRANSIENT_DURABILITY_QOS;
  topicQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
  topicQos.reliability.kind = RELIABLE_RELIABILITY_QOS;
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 50000000;
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__FlightAssign;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = TRANSIENT_LOCAL_DURABILITY_QOS;
  topicQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
  topicQos.reliability.kind = RELIABLE_RELIABILITY_QOS;
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 50000000;
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__FlightCoordination;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = TRANSIENT_DURABILITY_QOS;
  topicQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
  topicQos.reliability.kind = RELIABLE_RELIABILITY_QOS;
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 50000000;
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__FlightAuthorizationRequest;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = TRANSIENT_DURABILITY_QOS;
  topicQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
  topicQos.reliability.kind = RELIABLE_RELIABILITY_QOS;
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 50000000;
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__TolPadRequest;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = TRANSIENT_LOCAL_DURABILITY_QOS;
  topicQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
  topicQos.reliability.kind = RELIABLE_RELIABILITY_QOS;
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 50000000;
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__TrafficFlowsInfo;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = TRANSIENT_LOCAL_DURABILITY_QOS;
  topicQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
  topicQos.reliability.kind = RELIABLE_RELIABILITY_QOS;
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 100000000;
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__AirspaceRestrictions;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = TRANSIENT_DURABILITY_QOS;
  topicQos.ownership.kind = SHARED_OWNERSHIP_QOS;
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

  topic    = Topics::UATM__uatmDCPS__FlightAuthorization;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = PERSISTENT_DURABILITY_QOS;
  topicQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
  topicQos.reliability.kind = RELIABLE_RELIABILITY_QOS;
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 50000000;
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__FlightChangeRec;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = TRANSIENT_DURABILITY_QOS;
  topicQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
  topicQos.reliability.kind = RELIABLE_RELIABILITY_QOS;
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 50000000;
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__FlightRequestInfo;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  topicQos.durability.kind = TRANSIENT_LOCAL_DURABILITY_QOS;
  topicQos.ownership.kind = EXCLUSIVE_OWNERSHIP_QOS;
  topicQos.reliability.kind = RELIABLE_RELIABILITY_QOS;
  topicQos.reliability.max_blocking_time.sec = 2147483647;
  topicQos.reliability.max_blocking_time.nanosec = 2147483647;
  topicQos.deadline.period.sec = 0;
  topicQos.deadline.period.nanosec = 50000000;
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__AvailabilityFOP;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  this->topicsQos_[ topic] = topicQos;

  topic    = Topics::UATM__uatmDCPS__AvailabilitySKO;
  topicQos = TheServiceParticipant->initial_TopicQos();
    
  this->topicsQos_[ topic] = topicQos;
}

inline
void
Elements::Data::buildPublishersQos()
{
  PublisherQos       publisherQos;
  Publishers::Values publisher;

  publisher    = Publishers::bookingPlatformPub;
  publisherQos = TheServiceParticipant->initial_PublisherQos();
  this->publishersQos_[ publisher] = publisherQos;

  publisher    = Publishers::fleetOperatorPub;
  publisherQos = TheServiceParticipant->initial_PublisherQos();
  this->publishersQos_[ publisher] = publisherQos;

  publisher    = Publishers::evtolManPub;
  publisherQos = TheServiceParticipant->initial_PublisherQos();
  this->publishersQos_[ publisher] = publisherQos;

  publisher    = Publishers::pilotManPub;
  publisherQos = TheServiceParticipant->initial_PublisherQos();
  this->publishersQos_[ publisher] = publisherQos;

  publisher    = Publishers::weatherSysPub;
  publisherQos = TheServiceParticipant->initial_PublisherQos();
  this->publishersQos_[ publisher] = publisherQos;

  publisher    = Publishers::skyportOperatorPub;
  publisherQos = TheServiceParticipant->initial_PublisherQos();
  this->publishersQos_[ publisher] = publisherQos;

  publisher    = Publishers::skyportsManPub;
  publisherQos = TheServiceParticipant->initial_PublisherQos();
  this->publishersQos_[ publisher] = publisherQos;

  publisher    = Publishers::uaspManPub;
  publisherQos = TheServiceParticipant->initial_PublisherQos();
  this->publishersQos_[ publisher] = publisherQos;

  publisher    = Publishers::FlightAuthSysPub;
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

  subscriber    = Subscribers::evtolManSub;
  subscriberQos = TheServiceParticipant->initial_SubscriberQos();
  this->subscribersQos_[ subscriber] = subscriberQos;

  subscriber    = Subscribers::pilotManSub;
  subscriberQos = TheServiceParticipant->initial_SubscriberQos();
  this->subscribersQos_[ subscriber] = subscriberQos;

  subscriber    = Subscribers::skyportOperatorSub;
  subscriberQos = TheServiceParticipant->initial_SubscriberQos();
  this->subscribersQos_[ subscriber] = subscriberQos;

  subscriber    = Subscribers::skyportsManSub;
  subscriberQos = TheServiceParticipant->initial_SubscriberQos();
  this->subscribersQos_[ subscriber] = subscriberQos;

  subscriber    = Subscribers::uaspManSub;
  subscriberQos = TheServiceParticipant->initial_SubscriberQos();
  this->subscribersQos_[ subscriber] = subscriberQos;

  subscriber    = Subscribers::FlightAuthSysSub;
  subscriberQos = TheServiceParticipant->initial_SubscriberQos();
  this->subscribersQos_[ subscriber] = subscriberQos;

  subscriber    = Subscribers::anspSub;
  subscriberQos = TheServiceParticipant->initial_SubscriberQos();
  this->subscribersQos_[ subscriber] = subscriberQos;

  subscriber    = Subscribers::ussSub;
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

  writer    = DataWriters::bookingFlightRequestDW_BP;
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

  writer    = DataWriters::evtolAvailabilityDW_ev;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = true;

  writer    = DataWriters::pilotAvailabilityDW_PLM;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = true;

  writer    = DataWriters::weatherInfoDW_WTR;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = true;

  writer    = DataWriters::FlightRoutesDW_SKO;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = true;

  writer    = DataWriters::TrafficFlowDW_SKO;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = true;

  writer    = DataWriters::airspaceRestDW_SKO;
  writerQos = TheServiceParticipant->initial_DataWriterQos();
  this->writersQos_[ writer] = writerQos;
  this->writerCopyTopicQos_[writer] = true;

  writer    = DataWriters::SkyportAvailabilityDW_SKM;
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

  writer    = DataWriters::FlightRequestInfoDW_FAS;
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

  reader    = DataReaders::flighAssignDR_PLM;
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

  reader    = DataReaders::assignFlightDR_SKM;
  readerQos = TheServiceParticipant->initial_DataReaderQos();
  this->readersQos_[ reader] = readerQos;
  this->readerCopyTopicQos_[reader] = true;

  reader    = DataReaders::FlightRequestDR_UASP;
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

  reader    = DataReaders::tolPadAvailabilityDR_UASP;
  readerQos = TheServiceParticipant->initial_DataReaderQos();
  this->readersQos_[ reader] = readerQos;
  this->readerCopyTopicQos_[reader] = true;

  reader    = DataReaders::flightRequestDR_FAS;
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
    case DataWriters::bookingFlightRequestDW_BP:
      break;

    case DataWriters::uaspFlightRequestDW_FOP:
      break;

    case DataWriters::assignFlightDW_FOP:
      break;

    case DataWriters::flightCoordDW_FOP:
      break;

    case DataWriters::evtolAvailabilityDW_ev:
      break;

    case DataWriters::pilotAvailabilityDW_PLM:
      break;

    case DataWriters::weatherInfoDW_WTR:
      break;

    case DataWriters::FlightRoutesDW_SKO:
      break;

    case DataWriters::TrafficFlowDW_SKO:
      break;

    case DataWriters::airspaceRestDW_SKO:
      break;

    case DataWriters::SkyportAvailabilityDW_SKM:
      break;

    case DataWriters::routeDataDW_UASP:
      break;

    case DataWriters::flightAuthDW_UASP:
      break;

    case DataWriters::changeRecDW_UASP:
      break;

    case DataWriters::tolPadReqDW_UASP:
      break;

    case DataWriters::FlightRequestInfoDW_FAS:
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

    case DataReaders::flightAssignDR_EV:
      break;

    case DataReaders::flightAuthDR_PLM:
      break;

    case DataReaders::changeRecDR_PLM:
      break;

    case DataReaders::flighAssignDR_PLM:
      break;

    case DataReaders::availabilityDR_SKO:
      break;

    case DataReaders::flightCoordDR_SKO:
      break;

    case DataReaders::assignFlightDR_SKM:
      break;

    case DataReaders::FlightRequestDR_UASP:
      break;

    case DataReaders::trafficFlowsDR_UASP:
      break;

    case DataReaders::weatherInfoDR_UASP:
      break;

    case DataReaders::airspaceRestDR_UASP:
      break;

    case DataReaders::tolPadAvailabilityDR_UASP:
      break;

    case DataReaders::flightRequestDR_FAS:
      break;

    case DataReaders::routeDataDR_ANSP:
      break;

    case DataReaders::routeDataDR_USS:
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
