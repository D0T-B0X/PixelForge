Problem Statement: Mini Fantasy Console

Build a small retro-style “fantasy console” that can run simple mini-games written in a tiny instruction format or JSON script.

Instead of building a full Chip-8 emulator, which may be too difficult for a 2-person team in 36 hours, this project asks the team to create a simpler virtual game system.

The console should have a tiny screen, limited colors, keyboard input, and the ability to load and run at least one simple game.

Core Requirements

The fantasy console must include:

A fixed-size display, for example:
16×16 grid
32×32 grid
64×32 pixel canvas
A simple game runtime with:
Player position
Input handling
Collision detection
Game loop
A simple “cartridge” format using JSON or a small script.

Example cartridge:

{
  "title": "Coin Collector",
  "playerStart": { "x": 1, "y": 1 },
  "walls": [[0,0], [1,0], [2,0]],
  "coins": [[5,5], [8,3]],
  "enemies": [[10,6]],
  "goal": [14,14]
}
At least one playable mini-game cartridge.
Retro-style constraints:
Limited colors
Pixel/grid graphics
Simple sound or text feedback
Keyboard-only input
Minimum Viable Product

A successful MVP can be:

A browser or Python app.
One built-in game: collect all coins and reach the goal.
A JSON loader for different maps.
A simple retro display.
Suggested Tech Stack
HTML Canvas
Pygame
Phaser
JavaScript
Python
Stretch Goals
Multiple cartridges
Built-in cartridge editor
Sound effects
Sprite support
Save/load cartridges
Debug view showing game state
Tiny scripting commands such as MOVE, SPAWN, WIN
