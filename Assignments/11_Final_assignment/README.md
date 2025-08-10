# Sensor Monitoring System

## Overview 

This project implements a Sensor Monitoring System consisting of:
- Sensor nodes (software-simulated) that measure room temperature.
- A Sensor Gateway that collects, processes, and stores data from multiple sensors and hve an SQLite database to store processed measurements.

The system is designed to dynamically handle any number of sensors connecting/disconnecting over time, without requiring a fixed number at startup.

The architecture ensures thread-safe communication, dynamic sensor node connections, and robust logging and data storage mechanisms.

## Components
### 1. Sensor Node

- Simulated devices sending temperature data over TCP to the gateway.

## 2. Gateway node

- Acts as the central hub for data collection, processing, and storage.
   - Consists of a main process and a log process (forked as a child process, Req 1).
   - Main process runs three threads:
     - **Connection Manager**: Listens for incoming TCP connections from sensor nodes, captures data packets, and writes to a shared buffer (Req 3, 4).
     - **Data Manager**: Processes sensor data, calculates running averages for temperature, and determines "too hot/cold" states (Req 5).
     - **Storage Manager**: Reads data from the shared buffer and stores it in an SQLite database, with retry logic for database connection failures (Req 6).
   - Uses a **single shared** buffer (`sbuffer`) for thread communication, protected by synchronization primitives (Req 2).

3. **SQLite Database**:
   - Stores sensor measurements processed by the storage manager.
   - Supports dynamic table creation and data insertion (Req 6, 9).

4. **Log Process**:
   - Receives log events from the main process via a FIFO (`logFifo`) (Req 7).
   - Writes formatted log messages to `gateway.log` in the format: `<sequence number> <timestamp> <log-event info message>` (Req 8).
  


