# Snake - Work in Progress

## How to Copy the Project

1. **Clone the GitHub repository**
2. **Open the solution file** in Microsoft Visual Studio
3. **Run the project** by clicking the start button or pressing `F5`
4. **If you encounter issues, follow these steps:**
   - Watch this totorial video how to setup SFML: https://www.youtube.com/watch?v=lFzpkvrscs4
   - Copy appropriate files:
   - To x64/Debug:``` openal32.dll sfml-audio-d-2.dll sfml-graphics-d-2.dll sfml-system-d-2.dll sfml-window-d-2.dll ```
   - To x64/Release:``` openal32.dll sfml-audio-2.dll sfml-graphics-2.dll sfml-system-2.dll sfml-window-2.dll ```
   - `Then adjust what you have to type by following these:`
   - Go to **Project Properties**
   - Navigate to **C/C++ → General** and set **Additional Include Directories** to:
     ```
     $(SolutionDir)\include
     ```
   - Navigate to **Linker → General** and set **Additional Library Directories** to:
     ```
     $(SolutionDir)\lib
     ```
   - Navigate to **Linker → Input** and set **Additional Dependencies** to:
     ```
     sfml-system.lib; sfml-window.lib; sfml-graphics.lib; sfml-audio.lib
     ```
6. **If the above steps do not work,** please watch a tutorial on setting up SFML and copy the code from this repository.

## How to Play

### Game Modes:
- **Solo Mode:**
  - Register and gain points.
  - Compete for a spot in the **Hall of Fame** (Top 5 players).
- **Duo Mode:**
  - Player 1: Use `WASD` keys to move.
  - Player 2: Use `Arrow` keys to move.
  - Play against each other in a competitive match.
- **AI Training Mode:** *(Work in Progress)*
  - Snakes are controlled by AI for simulation and training.

Enjoy the game!
