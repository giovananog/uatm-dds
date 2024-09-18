# Project Tree

```plaintext
pre-departure scenario
├── general
│   └── model
└── sub-scenarios
    ├── scenario1
    │   ├── bookingPlatformDP
    │   │   └── Publisher
    │   ├── evtolManagerDP
    │   │   └── Publisher
    │   ├── fleetOperatorDP
    │   │   └── Subscriber
    │   ├── model
    │   └── pilotManagerDP
    │       └── Publisher
    ├── scenario2
    │   ├── evtolManagerDP
    │   │   └── Subscriber
    │   ├── fleetOperatorDP
    │   │   ├── Publisher
    │   │   └── Subscriber
    │   ├── model
    │   ├── pilotManagerDP
    │   │   └── Subscriber
    │   ├── skyportManagerDP
    │   │   └── Publisher
    │   ├── skyportOperatorDP
    │   │   └── Publisher
    │   └── weatherDP
    │       └── Publisher
    ├── scenario3
    │   ├── fleetOperatorDP
    │   │   └── Publisher
    │   ├── flightAuthSysDP
    │   │   ├── Publisher
    │   │   └── Subscriber
    │   ├── model
    │   ├── skyportOperatorDP
    │   │   └── Subscriber
    │   └── uaspManagerDP
    │       └── Subscriber
    ├── scenario4
    │   ├── model
    │   ├── tolPadManagerDP
    │   │   ├── Publisher
    │   │   └── Subscriber
    │   └── uaspManagerDP
    │       ├── Publisher
    │       └── Subscriber
    ├── scenario5
    │   ├── anspDP
    │   │   └── Subscriber
    │   ├── model
    │   ├── skyportOperatorDP
    │   │   └── Publisher
    │   ├── uaspManagerDP
    │   │   ├── Publisher
    │   │   └── Subscriber
    │   ├── ussDP
    │   │   └── Subscriber
    │   └── weatherDP
    │       └── Publisher
    └── scenario6
        ├── fleetOperatorDP
        │   └── Subscriber
        ├── model
        ├── pilotManagerDP
        │   └── Subscriber
        └── uaspManagerDP
            └── Publisher
