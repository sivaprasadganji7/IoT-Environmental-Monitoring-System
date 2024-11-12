# IoT-Based Environmental Monitoring System Using ESP32 and DHT11 Sensor with MQTT and Node-RED Integration

This project describes the design and implementation of an IoT-based environmental monitoring system that utilizes the ESP32 microcontroller, DHT11 sensor, MQTT protocol, and Node-RED for real-time data visualization. The system continuously monitors temperature and humidity, sending data to the cloud where it can be visualized and analyzed.

## Table of Contents

- [Introduction](#introduction)
- [System Design](#system-design)
- [Installation](#installation)
- [How to Use](#how-to-use)
- [Video Demonstration](#video-demonstration)
- [Contributing](#contributing)
- [License](#license)

## Introduction

This project uses an **ESP32 microcontroller** to gather environmental data from the **DHT11 sensor** (temperature and humidity). The data is sent via the **MQTT protocol** to a cloud platform for storage and real-time visualization in **Node-RED**. An **LED indicator** is used for visual status alerts based on temperature thresholds.

### Objectives:
- Design and implement an IoT-based environmental monitoring system.
- Collect real-time temperature and humidity data using the DHT11 sensor.
- Transmit data to the cloud using the MQTT protocol.
- Visualize data using Node-RED.
- Provide status alerts with an LED indicator based on temperature thresholds.

## System Design

The system consists of the following components:
1. **ESP32 Microcontroller**: The core of the system, which connects to the internet and controls other devices (sensor, LED).
2. **DHT11 Sensor**: Used for measuring temperature and humidity.
3. **MQTT Protocol**: Transmits the sensor data to the cloud in real-time.
4. **Node-RED**: Visualizes the data and allows user interactions for dynamic reporting intervals and controlling alerts.
5. **LED Indicator**: Provides status alerts based on the temperature data.

The full system architecture integrates these components, ensuring seamless data transmission, storage, and visualization. A detailed flowchart and system UML diagram are included in the paper.

## Installation

### Requirements:
- **ESP32** (or compatible microcontroller)
- **DHT11 Sensor**
- **Node-RED** (installed on a local server or cloud platform)
- **MongoDB** (for storing sensor data)
- **MQTT Broker** (e.g., Mosquitto or cloud MQTT service)

### Steps to Set Up:
1. Clone the repository:
    ```bash
    git clone https://github.com/your-username/environment-monitoring-system.git
    cd environment-monitoring-system
    ```

2. **Configure the ESP32**:
    - Install the required libraries (`DHT`, `WiFi`, `PubSubClient` for MQTT).
    - Update your **Wi-Fi credentials** and **MQTT broker settings** in the `config.h` file.

3. **Upload the code** to the ESP32 using Arduino IDE.

4. **Set up Node-RED**:
    - Install Node-RED on your server or local machine.
    - Import the provided Node-RED flow from the `node-red-flow.json` file.

5. **Set up MongoDB**:
    - Install MongoDB for storing sensor data.
    - Ensure that Node-RED is connected to MongoDB for data storage and visualization.

## How to Use

1. **Run the ESP32**: Once the system is powered up, the ESP32 will start reading temperature and humidity data from the DHT11 sensor.
2. **Monitor Data**: Open the Node-RED dashboard in your browser to view real-time temperature and humidity readings.
3. **Adjust Reporting Interval**: Use the Node-RED interface to change the reporting frequency based on your needs.
4. **Receive Alerts**: The LED indicator will turn on when the temperature exceeds a predefined threshold.

## Video Demonstration

You can view a demonstration of the system in action on [YouTube](https://www.youtube.com/your-video-link).

## Contributing

Feel free to fork this repository and submit pull requests. If you have suggestions, improvements, or bug fixes, create an issue or contribute directly.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

