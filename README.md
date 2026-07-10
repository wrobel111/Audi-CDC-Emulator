# Audi CDC Emulator

Embedded implementation of an Audi CD changer emulator with Bluetooth audio streaming and hands-free phone functionality.

The project is designed to integrate modern Bluetooth audio capabilities with factory Audi head units by emulating the communication behavior of an original CD changer module.

## Features

- Audi CD changer (CDC) protocol emulation
- Bluetooth audio integration
- Hands-free phone functionality
- AT command control of Bluetooth module
- Custom embedded firmware running on ATmega328P
- Hardware interface designed for integration with factory car audio systems

## Hardware

### Main controller

- Microcontroller: ATmega328PB

### Bluetooth module

- BK3266 or BK3254 Bluetooth audio module

The Bluetooth module is controlled through UART using AT commands.

Reference:
- BK3254/BK3266 supported commands: https://github.com/tomaskovacik/BK3254/wiki/Supported-commands-and-event-send-from-module

## Development notes

During development, multiple hardware revisions were tested to improve reliability and compatibility.

### V1.1 improvements and considerations

- Added AUX detection using enable signal from 3.5 mm jack connection
- Standardized Bluetooth module usage around BK3266
- Verified amplifier enable signal behavior
- Updated connector pinout to standard 6-pin ISP layout
- Improved UART connector layout:
  - TX
  - RX
  - GND
  - +5V

## Technical aspects

The project involved:

- Reverse engineering of Audi CDC communication
- Embedded firmware development in C/C++
- UART communication
- Bluetooth module integration
- Hardware debugging and prototyping

## Future improvements

- Additional Audi head unit compatibility
- Improved Bluetooth module support
- Further hardware revisions
- Better documentation of CDC protocol
