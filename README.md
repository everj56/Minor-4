# Minor-4

## Overview
This repository contains the implementation of a simple UDP-based client-server application in C. The client sends "PING" messages to the server, calculates the round-trip time (RTT) for the responses, and computes packet loss statistics. The server simulates real-world network conditions by introducing a 30% packet loss rate, making this project suitable for studying UDP communication and network performance.

## Features

- Measures round-trip time (RTT) for UDP packets.
- Simulates packet loss for real-world network behavior.
- Provides detailed statistics on packet loss and RTT (minimum, maximum, and average).
- Demonstrates basic UDP socket programming.

## Files

- `client.c`: The client-side program.
- `server.c`: The server-side program.

## Compilation

Use `gcc` to compile the client and server programs:

```bash
gcc -o client client.c
gcc -o server server.c
```
## Usage

Start the server with a specified port number:
./server <port_num>

Start the client with the server's hostname or IP address and port number:
./client <hostname> <port_num>

## Output
The client program prints the RTT for each "PING" and a summary of packet statistics:

PING 1: Sent... RTT=12.345 ms  
PING 2: Sent... RTT=11.234 ms  
PING 3: Sent... Timed out  
...  
10 pkts xmited, 8 pkts rcvd, 20% pkt loss  
min: 11.234, max: 15.678, avg: 13.456

The server program logs incoming messages and simulated packet drops:

ready to receive data...  
[client]: PING  
[server]: dropped packet  
[client]: PING  
[server]: sent response  
