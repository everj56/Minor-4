# Minor-4

## Overview
This repository contains the implementation of a simple UDP-based client-server application in C.

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


