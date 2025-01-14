# Retro Centipede Arcade Game

This project is a simplified recreation of the **Retro Centipede Arcade Game**, designed using **C++** with **SFML** (Simple and Fast Multimedia Library). The game features a 2D graphical interface, incorporating gameplay elements like centipede movement, laser blasts, mushroom obstacles, and a score tracking system.

---

## Objective

To understand and apply the principles of **2D graphics** and **class creation** in C++ by replicating the gameplay of the original **Centipede Arcade Game**.

---

## Features

1. **Game Areas**:
   - **Top Information Area**: Displays the score and remaining lives.
   - **Main Game Area**: Hosts the centipede, mushrooms, and laser blasts.
   - **Bottom Mushroom-Free Area**: Movement area for the player’s spaceship.

2. **Gameplay**:
   - The centipede moves through the game area, navigating around mushrooms.
   - Mushrooms are randomly placed at the start of the game and can be destroyed by laser blasts.
   - The player controls the spaceship using arrow keys and fires lasers using the space bar.
   - A spider moves randomly, destroying mushrooms on collision. It reduces player lives on contact with the spaceship.
   - The game ends when all centipede segments are destroyed or the player runs out of lives.

3. **Graphics and Animations**:
   - Laser blasts and mushroom health are visually represented.
   - Player’s score updates dynamically.
   - Lives are displayed using icons at the top of the screen.

4. **Additional Mechanics**:
   - Randomized mushroom placement using `std::uniform_int_distribution`.
   - Collision detection for all game entities.
   - Centipede splits into smaller segments when hit in the middle.

---

## File Structure

### Source Files

- **lab1.cpp**: Contains the main implementation of the game, including class definitions for the centipede and laser blasts.

### Game Assets

- **Graphics**:
  - `Background.png`: The game background.
  - `Mushroom0.png`, `Mushroom1.png`: Represent different states of mushrooms.
  - `CentipedeHead.png`, `CentipedeBody.png`: Sprites for centipede head and body segments.
  - `StarShip.png`: Player-controlled spaceship.
  - `Spider.png`: Spider enemy sprite.

- **Fonts**:
  - `KOMIKAP_.ttf`: Used for displaying the score and other text.

---

## Classes

### 1. **ECE_Centipede**
   - Handles centipede movement and collision detection.
   - Divides the centipede into smaller segments upon being hit.

### 2. **ECE_LaserBlast**
   - Represents laser blasts fired by the player.
   - Manages collision detection with centipedes, mushrooms, and spiders.

### Structs

- **Mushroom**:
  - Contains texture and health information for mushrooms.

- **CentipedeParticle**:
  - Represents individual segments of the centipede.

---

## How to Run

### Prerequisites

- **SFML Library**: Install SFML to support 2D graphics and multimedia functionality.
- **C++ Compiler**: A compiler with C++11 or later support (e.g., GCC, Clang).

### Steps

1. Clone the repository:
   ```bash
   git clone <repository_url>
   cd RetroCentipedeGame
   ```
2. Build the Project Using CMake:
   ```bash
   cmake --build .
   cmake ..
   ```
3. run the executable:
   ```bash
   ./RetroCentipedeGame
   ```

## Gameplay Instructions

### Controls:
- Use the **arrow keys** to move the spaceship.
- Press the **space bar** to fire laser blasts.

### Objective:
- Destroy all centipede segments to win.
- Avoid the spider and prevent it from destroying mushrooms.

### Game Over:
- The game ends when the player runs out of lives or the centipede is entirely destroyed.

---

## Testing and Debugging

### Key Elements to Test:
1. **Random Mushroom Placement**:
   - Ensure 30 mushrooms are randomly positioned in the main game area at the start.

2. **Centipede Movement**:
   - Verify the centipede navigates around mushrooms and splits when hit.

3. **Collision Detection**:
   - Test interactions between laser blasts, mushrooms, centipedes, and spiders.

4. **Score and Lives**:
   - Confirm the score updates correctly and the player loses lives appropriately.

5. **Spider Behavior**:
   - Validate the spider moves randomly and destroys mushrooms on contact.

### Debugging Tools:
- Use `std::cout` statements to trace the game’s flow during development.
- Leverage a debugger like `gdb` for runtime issue resolution.
