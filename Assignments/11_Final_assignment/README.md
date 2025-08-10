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
   - Uses a **single** shared buffer (`sbuffer`) for thread communication, protected by synchronization primitives (Req 2).  **(Need to improve)**
   - 
3. **SQLite Database**:
   - Stores sensor measurements processed by the storage manager.
   - Supports dynamic table creation and data insertion (Req 6, 9).

4. **Log Process**:
   - Receives log events from the main process via a FIFO (`logFifo`) (Req 7).  **(Need to improve)**
   - Writes formatted log messages to `gateway.log` in the format: `<sequence number> <timestamp> <log-event info message>` (Req 8).  **(Need to improve)**

  
### Data Flow
1. Sensor nodes send temperature measurements over TCP to the sensor gateway.
2. The connection manager thread accepts connections, reads packets, and stores data in the shared buffer (`sbuffer`).
3. The data manager thread reads from the shared buffer, calculates running averages, and logs "too hot/cold" events.
4. The storage manager thread reads from the shared buffer and inserts data into the SQLite database.
5. All threads send log events to the log process via the FIFO, which writes to `gateway.log`.  **(Need to improve)**

### Thread Synchronization
- A single `sbuffer` is used for data sharing between the connection, data, and storage manager threads (Req 2).
- Access to `sbuffer` is protected using a **mutex** to ensure thread safety, preventing simultaneous writes or read/write conflicts.
- The FIFO (`logFifo`) is also protected by a mutex to handle concurrent access by multiple threads (Req 7).  **(Need to improve)**
- Synchronization ensures:
  - **Correctness**: Only one thread accesses the shared buffer at a time.
  - **Deadlock Avoidance**: Simple lock ordering and no nested locks.
  - **Fairness**: Threads access the buffer immediately if not locked, avoiding starvation.
  - **Efficiency**: Minimal locking overhead with a mutex, suitable for the workload.

## Directory Structure

├── bin
│   ├── gateway
│   ├── sensor_1
│   └── sensor_2
├── inc
│   ├── connection_manager.h
│   ├── data_manager.h
│   ├── shared_data_pipe.h
│   └── storage_data.h
├── log.txt
├── Makefile
├── obj
│   ├── connection_manager.o
│   ├── data_manager.o
│   ├── main.o
│   ├── shared_data_pipe.o
│   └── storage_data.o
├── sensor
│   ├── sensor_connect_1.c
│   └── sensor_connect_2.c
├── sensor.db
└── src
    ├── connection_manager.c
    ├── data_manager.c
    ├── main.c
    ├── shared_data_pipe.c
    └── storage_data.c


