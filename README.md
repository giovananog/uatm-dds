# Project Setup with Docker

This project uses **OpenDDS** for distributed communication and provides a Docker-based environment for building and running the system.

## Prerequisites

1. Check that docker and docker-compose are installed.

```bash
docker --version
docker-compose --version
```

2. Install OpenDDS image.

```bash
docker pull ghcr.io/opendds/opendds:latest
```

3. Clone this repository and navigate to the project directory.

```bash
git clone https://github.com/giovananog/uatm-dds
cd uatm-dds/pre-departure-scenario/general
```

---

<br> 

## Building the example

1. Enter a container

```bash
docker run --rm -ti -v "$PWD:/opt/workspace" ghcr.io/opendds/opendds:latest
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

## Running the example with RTPS

1. Start the environment with Docker Compose:

```bash
docker-compose up
```
