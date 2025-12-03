# 🚀 UATM-DDS-SEC: Securing Urban Air Traffic Management with DDS

<br> 

This repository hosts the implementation of a software prototype for an Urban Air Traffic Management (**UATM**) system based on the **Data Distribution Service (DDS)** middleware. The focus of this work is the rigorous application and evaluation of the **DDS Security Specification**.

This code base supports the research presented in the paper: **"Security Challenges and Recommendations For Data Distribution Service On Urban Air Traffic Management Systems"**.

<br> 

## 🛡️ Project Context & Security Focus

This prototype simulates the pre-departure phase of eVTOL (electric Vertical Take-Off and Landing) vehicle operations, acting as a live testbed for threat modeling within a safety-critical domain.

| Aspect | Detail |
| :--- | :--- |
| **Objective** | Simulate secure real-time data exchange among UATM stakeholders (UASP, Fleet Operator, Skyports, etc.). |
| **DDS Middleware** | **OpenDDS**. Chosen for its open-source nature and robust implementation of the DDS Security Specification. |
| **Core Security** | **DDS Security Plugins** enforcing mutual **Authentication (X.509 Certificates)** and fine-grained **Access Control (XML Permissions)** based on the Principle of Least Privilege (**PoLP**). |
| **Methodology Link**| The architecture is derived from a System-Theoretic Process Analysis (**STPA**) and was assessed using **STRIDE** and **DREAD** threat modeling. |
| **Scenario** | Pre-departure workflow (Flight Booking, Resource Assignment, Flight Authorization...). |

<br> 

## 📐 System Architecture (Domain Participants & Topics)

The system is comprised of 12 distinct **Domain Participants (DPs)**, each representing a UATM entity, exchanging data via 17 well-defined **DDS Topics**.

### Key Architectural Model

<p align="center">
  <img src="uatm-dds-diagram.png" 
       alt="Diagram: UATM DDS Security Architecture Model - Showing Participants, Topics, and Security Assets" 
       width="700"> 
  <br>
  <em>Figure 1: UATM DDS Security Architecture Model.</em>
</p>

### DDS Entities

The project is structured with one Domain Participant (DP) per critical entity:

| Entity Folder | Description | Role in UATM Scenario |
| :--- | :--- | :--- |
| `uaspManagerDP` | Urban Airspace Service Provider | Oversees traffic flows, manages airspace efficiency, optimizes routes, and issues `FlightAuthorization`. |
| `fleetOperatorDP` | Fleet Operator | Coordinates logistics, assigns pilots/eVTOLs, and publishes `FlightCoordination` details. |
| `skyportOperatorDP` | Skyport Operator | Manages skyport services, traffic flows (`trafficFlowsDW`), and airspace restrictions. |
| `pilotManagerDP` | Pilot Manager | Oversees pilot availability (`pilotAvailabilityDW`) and handles pilot assignments. |
| `evtolManagerDP` | eVTOL Manager | Manages availability of eVTOL vehicles and assigns them to flights. |
| `weatherDP` | Weather System | Supplies critical weather data (`weatherInfoDW`) for flight operations. |
| `bookingPlatformDP` | Booking Platform | Manages flight reservations and publishes `bookingFlightRequest`. |
| `anspDP` | Air Navigation Service Provider | Coordinates with UASP/USS to evaluate and approve urban flight routes. |
| `ussDP` | UAS Service Supplier | Coordinates with ANSP/UASP to identify acceptable routes for unmanned aircraft. |
| `flightAuthSysDP` | Flight Authorization System | Processes flight requests, evaluates compliance, and provides authorizations. |
| `skyportsManDP` | Skyports Manager | Oversees the general availability status and allocation of skyports. |
| `tolPadManagerDP` | TOL Pad Manager | Manages the specific allocation and status of Take-Off/Landing (TOL) pads. |

<br> 

## ⚙️ Repository Structure

```tree
uatm-dds/
└── pre-departure-scenario/
└── general/
├── {DP_NAME}DP/ # Folders for each of the 12 Domain Participants (C++ code)
├── security/ # CRITICAL: Contains all X.509 certificates and XML policies
├── model/ # IDL files and OpenDDS project definitions
├── docker-compose.yml# Defines and orchestrates the entire multi-process DDS system
├── rtps.ini # OpenDDS RTPS (Real-Time Publish-Subscribe) transport config
└── mwc.pl # OpenDDS configuration script
```

<br> 

### 📄 Example Output Logs

For a complete reference of the expected execution logs and system behavior, please check the following files included in this repository:

*   [**`example-output-docker.txt`**](example-output-docker.txt): Contains the full log output when running the simulation via **Docker Compose**.
*   [**`example-output-perl.txt`**](example-output-perl.txt): Contains the log output when running via the native Perl scripts.

<br> 

## 🛠️ Project Setup with Docker

This project uses **OpenDDS** for distributed communication and provides a Docker-based environment for building and running the system.

### Prerequisites

1. Check that docker and docker-compose are installed.

```bash
docker --version
docker-compose --version
```

2. Install OpenDDS image.

```bash
docker pull ghcr.io/opendds/opendds:latest
```

3. Clone this repository and navigate to the `general` directory within the project.

```bash
git clone https://github.com/giovananog/uatm-dds
cd uatm-dds/pre-departure-scenario/general
```

---

<br> 

### Building the example

1. Enter a container

```bash
docker run --rm -ti -v "$(PWD):/opt/workspace" ghcr.io/opendds/opendds:latest
```

2. Configure and build the example

```bash
source /opt/OpenDDS/setenv.sh
mwc.pl -type gnuace
make
```

3. Exit the container

```bash
exit
```
---

<br>

### Running the example with RTPS

1. Start the environment with Docker Compose:

```bash
docker-compose up
```

You will see a live log output showing discovery, topic data (e.g., weatherInfo, flightAuthorization), and the individual container actions.
