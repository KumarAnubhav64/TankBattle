# Tank Battle Architecture

This document explains the code structure and design of the Tank Battle game.

## Overview
The game uses a **Modular Object-Oriented Architecture** in C++. It separates concerns into distinct classes for Game Logic, Entities (Tank, Bullet), Networking, and Utilities.

## Class Structure

### 1. `Game` (`Game.h`, `Game.cpp`)
- **Responsibility**: Manages the main game loop, input handling, updating entities, and rendering.
- **Key Methods**:
    - `run()`: The main loop (Input -> Update -> Render).
    - `handleInput()`: Reads keyboard input and updates local tank.
    - `syncNetwork()`: Sends local state to peer and receives remote state.
    - `checkCollisions()`: Detects bullet hits and manages scoring/respawning.

### 2. `Tank` (`Tank.h`, `Tank.cpp`)
- **Responsibility**: Represents a player's tank.
- **Attributes**: Position (`x`, `y`), Rotation (`angle`), `health`, `color`.
- **Key Methods**:
    - `update()`: Handles movement and shooting logic.
    - `draw()`: Renders the tank using `graphics.h` primitives (polygons for body, circle for turret).
    - `getState()` / `setState()`: Helper methods for network serialization.

### 3. `Bullet` (`Bullet.h`, `Bullet.cpp`)
- **Responsibility**: Represents a projectile fired by a tank.
- **Attributes**: Position, Angle, Speed, Owner ID.
- **Key Methods**:
    - `update()`: Moves the bullet forward based on its angle.
    - `draw()`: Renders the bullet.

### 4. `Network` (`Network.h`, `Network.cpp`)
- **Responsibility**: Abstracts low-level BSD Socket communication.
- **Key Methods**:
    - `initServer()`: Sets up a TCP listener and accepts a connection.
    - `initClient(ip)`: Connects to a TCP server.
    - `sendTankState()` / `receiveTankState()`: Transmits `TankState` structs.

### 5. `Utils.h`
- **Responsibility**: Shared constants and data structures.
- **Key Structs**:
    - `TankState`: A compact struct used to synchronize data over the network (x, y, angle, health, score).

## Networking Logic
- The game uses **TCP Sockets** for reliability (though UDP is often preferred for fast-paced games, TCP is sufficient for a simple LAN game).
- **Peer-to-Peer State Sync**:
    - Each frame, the local player sends their `TankState` to the peer.
    - The peer receives this state and updates the `oppTank` (Opponent Tank) object.
    - This ensures that Player 2 sees exactly what Player 1 is doing, and vice versa.

## Graphics
- Built using `graphics.h` (via `libXbgi` on Linux).
- Uses `initwindow` for a fixed 640x480 resolution.
- Double buffering is simulated by clearing and redrawing each frame (simple approach).
