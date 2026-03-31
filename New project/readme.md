
# Typing Master Pro: SOLID Refactor Project

This repository contains the refactored version of the **Typing Master Pro** game. The project has been transformed from a single-file procedural C program into a modular, Object-Oriented inspired architecture following SOLID principles.

---

## 📑 Table of Contents

1. [Project Overview](#-project-overview)
2. [Code Structure & Modules](#-code-structure--modules)
3. [SOLID Implementation Details](#-solid-implementation-details)
4. [Comparison: Original vs. Refactored](#-comparison-original-vs-refactored)
5. [How to Build and Run](#-how-to-build-and-run)
6. [Git Workflow & Branching](#-git-workflow--branching)

---

## 🎯 Project Overview

The goal of this refactor was to eliminate the **"God Object" anti-pattern** (where `main.c` handled everything) and replace it with decoupled components. We simulated Object-Oriented Programming (OOP) in C using **Opaque Structs** for encapsulation and **Function Pointers** for polymorphism.

---

## 🏗 Code Structure & Modules

The code is now split into specific domains. Each domain consists of a `.h` (Interface) and a `.c` (Implementation) file.

### 📂 File Map

| File | Responsibility |
|------|----------------|
| `main.c` | Entry point. Initializes the window and starts the GameManager. |
| `common.h` | Shared definitions, constants, and color palettes. |
| `game.h/c` | Game Manager: Orchestrates the interaction between all sub-managers. |
| `state.h/c` | State Manager: Defines the `IState` interface for Menu, Game, and Final states. |
| `ui.h/c` | UI Renderer: Handles all Raylib drawing logic (Text, Rectangles, Textures). |
| `input.h/c` | Input Handler: Manages the keyboard buffer and user typing logic. |
| `level.h/c` | Level Loader: Responsible for File I/O and loading level strings. |
| `timer.h/c` | Timer Manager: Tracks game time, countdowns, and cursor blinking. |
| `stats.h/c` | Statistics Manager: Calculates WPM, Accuracy, and stores results. |

---

## 🛠 SOLID Implementation Details

### S — Single Responsibility Principle (SRP)

Each module has only one reason to change.

> **Example:** If you want to change the font or colors, you only modify `ui.c`. If you want to change how WPM is calculated, you only modify `stats.c`.

---

### O — Open/Closed Principle (OCP)

The system is open for extension but closed for modification.

> **Example:** We used a **State Pattern**. To add a "Pause Screen," you simply create a new `IState` implementation without touching the core `GameManager` loop.

---

### L — Liskov Substitution Principle (LSP)

The `GameManager` treats all states (Menu, Game, Final) as an `IState` abstraction.

> **Example:** The manager calls `currentState->Update()`. It doesn't matter which state is active; they are all interchangeable through the function pointer interface.

---

### I — Interface Segregation Principle (ISP)

Interfaces are kept small and specific.

> **Example:** The `UIRenderer` functions take only the specific data they need (e.g., a `double time`) rather than the entire `GameManager` object.

---

### D — Dependency Inversion Principle (DIP)

High-level logic depends on abstractions, not concrete details.

> **Example:** `GameManager` does not depend on a specific "Menu" implementation; it depends on the `IState` struct definition.

---

## 🔄 Comparison: Original vs. Refactored

| Feature | Original Code | Refactored (SOLID) |
|---------|--------------|-------------------|
| Organization | Single 250+ line `main.c` | 8+ Decoupled Modules |
| State Logic | Massive `switch(state)` in main loop | Polymorphic `State_Update()` calls |
| Data | Global `GameData` struct | Encapsulated Manager Structs |
| Extendability | Hard to add levels/states | Highly modular; plug-and-play |

---

## 🚀 How to Build and Run

### 1. Requirements

- GCC Compiler
- Raylib Library installed

### 2. Compilation

Run the following command in your terminal to compile all modules:

```bash
gcc main.c game.c ui.c input.c level.c timer.c stats.c state.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o TypingMaster
```

### 3. File Dependencies

Ensure the following assets are in the root directory:

- `level1.txt`, `level2.txt`, `level3.txt`
- `bg.jpg`

---

## 🌿 Git Workflow & Branching

This refactor was performed on the `solid-refactor` branch.

**Commits sequence:**

1. **Setup:** Define `common.h` and project architecture
2. **Refactor:** Extract `LevelLoader` and `StatsManager` (SRP)
3. **Refactor:** Implement `IState` interface and State Pattern (OCP/LSP)
4. **Refactor:** Separate UI Rendering from Game Logic (ISP)
5. **Refactor:** Finalize `GameManager` orchestrator (DIP)

---

## 📜 Code Snippet: The "Interface" (DIP)

The core of our abstraction resides in `state.h`:

```c
typedef struct IState {
    void (*Enter)(GameManager* gm);
    void (*Update)(GameManager* gm);
    void (*Draw)(GameManager* gm);
    void (*Exit)(GameManager* gm);
} IState;
```
