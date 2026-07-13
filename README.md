<img width="1595" height="1021" alt="Audi CDC v1 1" src="https://github.com/user-attachments/assets/505bfb73-a71a-4355-bd7b-d4c9362d64be" />

# Audi CDC Emulator

Embedded CD changer emulator for Audi factory head units with Bluetooth audio streaming and hands-free phone functionality.

The project emulates an original Audi CD changer, allowing modern Bluetooth connectivity while preserving the original vehicle infotainment system. The hardware and firmware were designed to integrate seamlessly with factory radios without permanent vehicle modifications.

---

## Features

- Audi CD changer (CDC) protocol emulation
- Bluetooth audio streaming (A2DP)
- Hands-free phone functionality (HFP)
- Steering wheel (Rem-C line) and radio button support
- UART communication with Bluetooth module using AT commands
- Custom hardware designed for automotive applications
- AVR firmware written in C/C++

---

## Hardware

### Microcontroller

- ATmega328PB

### Bluetooth module

- BK3254

The Bluetooth module is controlled through UART using AT commands.

Supported AT commands:
https://github.com/tomaskovacik/BK3254/wiki/Supported-commands-and-event-send-from-module

---

## Hardware Design

The PCB was designed specifically for integration with the Audi factory audio system.

Main hardware components include:

- ATmega328PB microcontroller
- BK3254 Bluetooth module
- Audio amplifier
- Automotive power supply interface
- ISP programming connector
- UART service connector

Hardware design files are available in the `Hardware` directory.

---

## Firmware

The firmware is responsible for:

- emulating the Audi CD changer
- controlling the Bluetooth module
- handling button events
- switching between music playback and phone calls
- managing amplifier enable signals
- communication with the factory head unit

- ## Development History

### 24 May 2025

#### Added

- Support for BK3254 Bluetooth module
- Bluetooth music streaming
- Hands-free phone functionality

#### Changed

- Migrated timer implementation from Timer0 to Timer3 on ATmega328PB

---

## PCB Revision Notes (v1.1)

The following improvements are planned for the next PCB revision:

1. Add AUX detection using the switching contacts of the 3.5 mm jack connector.
2. Standardize the design to use the BK3254 Bluetooth module only.
3. Verify amplifier enable signal timing.
4. Replace the programming connector with a standard 6-pin ISP header.
5. Reorder the serial connector pinout:

   - +5 V
   - TX
   - RX
   - GND

6. Replace the audio amplifier with the SSM2211.
   - The SSM2211 uses an active LOW enable input, requiring inverted control logic.
7. Add series resistors and TVS protection diodes on the USB data lines.
8. Improve Digispark module pin placement for easier assembly and debugging.

<img width="2048" height="1536" alt="CDC v1 1 with inprovements" src="https://github.com/user-attachments/assets/5f518655-e0ce-474a-a1b8-336dca8e06db" />


---
