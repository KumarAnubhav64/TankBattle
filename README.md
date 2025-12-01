# Tank Battle (C++ / Graphics.h)

A real-time multiplayer Tank Battle game built using C++ and the `graphics.h` library. Supports both Local Multiplayer (Shared Keyboard) and LAN Multiplayer (TCP Sockets).

## Features
- **Multiplayer**: Play locally on one PC or over a Local Area Network (LAN).
- **Gameplay**: Move, rotate, and shoot to destroy the opponent.
- **Scoring**: First to kill the other tank gets a point. Tanks respawn automatically.
- **Graphics**: Custom drawn tanks with tracks and turrets, grid background, and UI.

## Installation

### Prerequisites
- Linux OS
- `g++` compiler
- `libgraph` / `graphics.h` installed (`libXbgi`, `libX11`)

### Compilation
Navigate to the project directory and run `make`:
```bash
cd TankBattle
make
```

## How to Play

### 1. Local Mode (Single Computer)
Two players share the same keyboard.
```bash
./tank_battle local
```
- **Player 1 (Green)**:
    - Move: `W`, `A`, `S`, `D`
    - Shoot: `Space`
- **Player 2 (Red)**:
    - Move: `I`, `J`, `K`, `L`
    - Shoot: `Enter`

### 2. LAN Mode (Two Computers)
Play across two computers on the same WiFi/Network.

**Host (Player 1 - Green)**:
1. Find your IP address: `hostname -I`
2. Run:
   ```bash
   ./tank_battle server
   ```
3. Wait for the client to connect.

**Client (Player 2 - Red)**:
1. Run:
   ```bash
   ./tank_battle client <HOST_IP_ADDRESS>
   ```
   Example: `./tank_battle client 192.168.1.5`

*Note: In LAN mode, both players use `WASD` and `Space` on their own keyboards.*

## Controls Summary
| Action | Player 1 (Local/LAN) | Player 2 (Local) | Player 2 (LAN Client) |
| :--- | :--- | :--- | :--- |
| **Forward** | `W` | `I` | `W` |
| **Backward** | `S` | `K` | `S` |
| **Left** | `A` | `J` | `A` |
| **Right** | `D` | `L` | `D` |
| **Shoot** | `Space` | `Enter` | `Space` |

## Screenshots
![Gameplay 1](Screenshot%20From%202025-12-02%2003-22-22.png)
![Gameplay 2](Screenshot%20From%202025-12-02%2003-23-25.png)
![Gameplay 3](Screenshot%20From%202025-12-02%2003-23-25-1.png)

## Gameplay Video
![Gameplay Demo](Screencast%20From%202025-12-02%2003-27-21.mp4)
