## Seat Reservation System

This repository contains a simple client-server application that implements a seat reservation system. The system allows clients to reserve seats in a grid layout either manually or automatically based on availability.
Features

    Server Application: Manages the seat reservations, tracks available and occupied seats, and handles client requests.
    Client Application: Allows users to reserve seats manually by specifying the desired row and column, or automatically, where the system picks a random seat.
    Configuration via INI File: For automatic mode, the client can specify settings like server IP, port, and timeout through a .txt file.

# System Requirements

    Linux operating system
    GCC for compiling the C files
    Standard C libraries

# Installation

    Clone the repository to your local machine:

    bash

git clone <repository-url>

Compile the source code:

bash

    gcc server.c -o server
    gcc client.c -o client

# Usage
Server

Run the server application:

bash

./server [length width]

    length and width are optional parameters that define the dimensions of the seating grid. If not provided, a default 10x10 grid is used.

# Client

Run the client application:

bash

./client manual

or

bash

./client automatic myinifile.txt

    For manual mode, you will be prompted to enter the row and column numbers.
    For automatic mode, ensure the myinifile.txt contains the necessary configuration.

Configuration File Example (myinifile.txt)

makefile

ip = 127.0.0.1
port = 5437
timeout = 100

# Server Logic

    Initializes a seat grid and listens for incoming client connections.
    Processes each reservation request, updates the seat status, and responds to the client with the reservation outcome.

# Client Logic

    Connects to the server either in manual or automatic mode.
    In automatic mode, reads server connection details from an INI file.
    Sends seat reservation requests and handles server responses.

# Contributors

Feel free to fork this project and contribute to its development. If you encounter any issues or have suggestions for improvements, please open an issue in this repository.
