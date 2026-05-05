# Typing Master
### MVC Edition — Typing Speed Testing Game

> Language: C++17 | Library: Raylib 5.0 | Pattern: MVC | Platform: Windows / Linux / macOS

---

## About

**Typing Master** is a 3-level typing speed game built in **C++17** using the **Raylib** graphics library.
The final version is fully refactored using the **Model-View-Controller (MVC)** design pattern for clean, maintainable, and extensible code.

---

## Project Structure

```
Typing-master/
│
├── Assets/
│   └── Audio/
│       └── sound.mp3          <- Background/effect audio
│
├── BackgroundImg/
│   └── background.avif        <- Background image
│
├── Levels/
│   ├── level1.txt             <- Easy level text
│   ├── level2.txt             <- Medium level text
│   ├── level3.txt             <- Hard level text
│   └── t.txt                  <- Test text file
│
├── Include/                   <- All header files (.h)
│   ├── Controller/
│   │   └── GameController.h
│   ├── Model/
│   │   ├── Constants.h
│   │   └── GameModel.h
│   └── View/
│       └── GameView.h
│
├── Src/                       <- All source files (.cpp)
│   ├── Controller/
│   │   └── GameController.cpp <- Input handling & state transitions
│   ├── Model/
│   │   └── GameModel.cpp      <- Game data & business logic
│   └── View/
│       └── GameView.cpp       <- All rendering / drawing
│
├── Runnable Program/          <- ONE-CLICK build and run scripts
│   ├── build_and_run.bat      <- For Windows
│   └── build_and_run.sh       <- For Linux / macOS
│
├── Others/
│   └── CMakeLists.txt         <- Build configuration
│
└── README.md
```

---

## MVC Architecture

```
+----------------+     reads      +----------------+
|                |<---------------|                |
|     MODEL      |                |      VIEW      |
|                |                |                |
| - Game data    |                | - Rendering    |
| - WPM calc     |                | - DrawText     |
| - File I/O     |                | - Window       |
| - Stats        |                | - Colors       |
+-------+--------+                +----------------+
        |  mutates                        ^
        |                                 | render()
+-------+---------------------------------+---------+
|                   CONTROLLER                      |
|   - Keyboard input     - Timer checks             |
|   - State transitions  - Cursor blink             |
+---------------------------------------------------+
```

### Component Responsibilities

| File | Role | Allowed to touch |
|------|------|-----------------|
| `GameModel.h / .cpp` | Data + Business Logic | Nothing else |
| `GameView.h / .cpp` | Rendering only | Model (read-only) |
| `GameController.h / .cpp` | Input + State flow | Model + View |
| `Constants.h` | Shared constants and enums | — |

---

## Gameplay

| Level | Time Limit | Difficulty |
|-------|-----------|------------|
| Level 1 | 45 seconds | Easy |
| Level 2 | 75 seconds | Medium |
| Level 3 | 120 seconds | Hard |

### Controls

| Key | Action |
|-----|--------|
| ENTER | Start game / Go to next level |
| TAB | Finish current level early |
| BACKSPACE | Delete last typed character |
| ESC | Quit on Final Stats screen |

---

## Game States

```
MENU --> COUNTDOWN --> PLAYING --> LEVEL_RESULT --> FINAL
                          ^              |
                          +--------------+
                       (repeats for all 3 levels)
```

---

## How to Run (Quickest Way)

### The `Runnable Program` folder contains ready-made scripts that automatically install dependencies, compile the project, and launch the game — all in one step.

---

### Windows — use `build_and_run.bat`

**Step 1** — Open the `Runnable Program` folder

**Step 2** — Double-click `build_and_run.bat`

OR run it from terminal:

```bat
cd "Runnable Program"
build_and_run.bat
```

The script will:
- Check if g++ is installed
- Automatically download Raylib 5.0 if not found
- Compile all source files
- Launch `TypingMaster.exe`

---

### Linux / macOS — use `build_and_run.sh`

**Step 1** — Open a terminal in the project folder

**Step 2** — Run:

```bash
cd "Runnable Program"
chmod +x build_and_run.sh
./build_and_run.sh
```

The script will:
- Detect your OS (Ubuntu, Arch, Fedora, macOS)
- Auto-install Raylib if missing
- Compile all source files
- Launch the game

---

## How to Build Manually (Without Scripts)

### Requirements

- C++17 compiler (g++ or MSVC)
- CMake 3.16 or higher
- Raylib 5.0

---

### Windows (using MSYS2)

**Step 1 — Install MSYS2**

Download from: https://www.msys2.org
Open **MSYS2 MinGW 64-bit** terminal.

**Step 2 — Install tools**

```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-raylib
```

**Step 3 — Clone the repo and switch to MVC branch**

```bash
git clone https://github.com/Ornob566/Typing-master.git
cd Typing-master
git checkout MVC
```

**Step 4 — Build**

```bash
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
```

**Step 5 — Run**

```bash
cd build
./TypingMaster.exe
```

---

### Linux (Ubuntu / Debian)

**Step 1 — Install dependencies**

```bash
sudo apt update
sudo apt install g++ cmake libraylib-dev
```

**Step 2 — Clone and checkout MVC branch**

```bash
git clone https://github.com/Ornob566/Typing-master.git
cd Typing-master
git checkout MVC
```

**Step 3 — Build and Run**

```bash
cmake -S . -B build
cmake --build build
./build/TypingMaster
```

---

### macOS

**Step 1 — Install dependencies**

```bash
brew install cmake raylib
```

**Step 2 — Clone and checkout MVC branch**

```bash
git clone https://github.com/Ornob566/Typing-master.git
cd Typing-master
git checkout MVC
```

**Step 3 — Build and Run**

```bash
cmake -S . -B build
cmake --build build
./build/TypingMaster
```

---

## Branches

| Branch | Description |
|--------|-------------|
| `main` | Original single-class version |
| `MVC` | Refactored MVC version (this branch) |

---

## Author

**Ornob566**
GitHub: https://github.com/Ornob566/Typing-master
