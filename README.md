# LoRaWAN Weather Station for Environmental Monitoring

## Overview

This project implements a LoRaWAN-based weather monitoring station designed for long-range and low-power environmental data acquisition.

The system collects meteorological measurements from multiple sensors connected to an Arduino-compatible platform and periodically transmits the data through a LoRaWAN network using OTAA (Over-The-Air Activation).

The project was developed as part of an educational and experimental IoT initiative focusing on wireless sensor networks, environmental monitoring, and low-power communications.

## Features
- LoRaWAN communication using OTAA activation
- Environmental data acquisition
- Wind speed measurement using an anemometer
- Wind direction measurement using a weather vane
- Rainfall detection through a tipping-bucket rain gauge
- Atmospheric pressure measurement using a BMP280 sensor
- Temperature monitoring
- UV radiation measurement using an ML8511 UV sensor
- Periodic wireless transmission to a LoRaWAN network server

## Hardware Components

The weather station integrates the following sensors and modules:

| Component | Function |
|-----------|----------|
| BMP280 | Temperature, pressure and altitude estimation |
| ML8511 | UV radiation measurement |
| Anemometer | Wind speed measurement |
| Wind vane | Wind direction measurement |
| Rain gauge | Rainfall detection |
| LoRaWAN Transceiver (SX127x) | Long-range wireless communication |
| Arduino-compatible MCU | Data acquisition and processing |

## Software Architecture

The project is organized into several source files:

#### weather_station.ino

Main application file responsible for:

- Sensor initialization
- LoRaWAN configuration
- Interrupt management
- Wind speed calculations
- Periodic scheduling
#### LectureCapteurs.ino

Sensor acquisition module:

- Wind direction reading
- BMP280 measurements
- UV sensor measurements
- Environmental data formatting
#### Envoi_donnees.ino

Payload generation and transmission:

- Data serialization
- LoRaWAN packet preparation
- Uplink transmission
#### Lorawan_functions.ino

LoRaWAN event management:

- Network join procedure
- Transmission status monitoring
- Session management
- Event logging

## Communication Protocol

The device uses:

- LoRaWAN Class A
- OTAA activation
- Encrypted communication
- Periodic uplink transmissions

The current implementation transmits sensor data every 15 seconds for testing and development purposes.

## Measured Parameters

The station is capable of monitoring:

- Air temperature
- Atmospheric pressure
- Estimated altitude
- UV radiation level
- Wind speed
- Wind direction
- Rainfall activity

## Research Objectives

This project aims to evaluate the effectiveness of LoRaWAN technology for environmental monitoring applications by focusing on:

- Long-range communication performance
- Energy efficiency
- Sensor integration
- Reliability of meteorological measurements
- Scalability for distributed IoT deployments

## Potential Applications
- Smart agriculture
- Environmental observation
- Climate studies
- Remote monitoring stations
- Educational IoT platforms
- Smart city infrastructures

## System Architecture

```text
+------------------+
| Weather Sensors  |
+------------------+
         |
         v
+------------------+
| Arduino MCU      |
+------------------+
         |
         v
+------------------+
| LoRa Transceiver |
|    (SX127x)      |
+------------------+
         |
         v
+------------------+
| LoRaWAN Gateway  |
+------------------+
         |
         v
+------------------+
| Network Server   |
+------------------+
         |
         v
+------------------+
| Data Storage &   |
| Visualization    |
+------------------+
```

## Future Improvements

Planned developments include:

- Battery-powered autonomous operation
- Solar energy harvesting
- Additional environmental sensors
- Data visualization dashboard
- Adaptive transmission intervals
- Edge-based anomaly detection
- Integration with cloud IoT platforms

## Installation

1. Clone the repository.
2. Open the project using Arduino IDE.
3. Install the required libraries.
4. Configure LoRaWAN credentials.
5. Upload the firmware to the target device.

## Network Infrastructure

The weather station was designed to operate on a LoRaWAN network such as The Things Network (TTN), allowing secure and long-range transmission of environmental data.

## License

This project is intended for educational, research, and non-commercial purposes.
