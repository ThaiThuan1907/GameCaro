### 1. Game Engine & Core Logic
- **Win Condition:** Precise 5-in-a-row algorithm for horizontal, vertical, and diagonal patterns.
- **Control System:** Keyboard-first navigation using **Arrow keys** and **Space/Enter** for move placement.

### 2. Heuristic AI System
- **Difficulty Levels:** 
    - **Easy:** Random move generation.
    - **Medium/Hard:** Intelligent heuristic scoring prioritizing offensive connectivity and defensive blocking.
- **Auto-Move:** Seamless integration into the main game loop for PvAI matches.

### 3. Data Persistence (Save/Load)
- **Match State:** Persists board configuration, current player, and time to `.sav` files.
- **Context-Aware Loading:** Automatically preserves game mode (PvP vs PvAI) and AI difficulty settings upon loading.

### 4. UI/UX Enhancements
- **Dynamic HUD:** Real-time countdown timers and match clocks.
- **Game Over Transitions:** Dark-overlay screen with interactive **Restart** and **Menu** buttons.
- **Safety Dialogs:** Professional confirmation pop-ups for Saving, Restarting, and Exiting.