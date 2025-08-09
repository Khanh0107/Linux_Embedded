# Sensor Monitoring System

## Overview 

This project implements a Sensor Monitoring System consisting of:
- Sensor nodes (software-simulated) that measure room temperature.
- A Sensor Gateway that collects, processes, and stores data from multiple sensors and hve an SQLite database to store processed measurements.

The system is designed to dynamically handle any number of sensors connecting/disconnecting over time, without requiring a fixed number at startup.

The architecture ensures thread-safe communication, dynamic sensor node connections, and robust logging and data storage mechanisms.

#re
