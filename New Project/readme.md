# 🎹 Typing Master— SOLID Refactor

> A Raylib-based typing speed game refactored from a monolithic OOP design into a fully SOLID-compliant multi-class C++17 architecture.

---

## 📋 Table of Contents

- [Project Overview](#-project-overview)
- [Folder Structure](#-folder-structure)
- [SOLID Principles Applied](#-solid-principles-applied)
- [Class Responsibilities](#-class-responsibilities)
- [Full Class Reference](#-full-class-reference)
- [Dependencies](#-dependencies)
- [Build Instructions](#-build-instructions)
- [Git Workflow](#-git-workflow)
- [Commit History Guide](#-commit-history-guide)
- [AI Tools & Prompts Used](#-ai-tools--prompts-used)
- [Level Files](#-level-files)
- [Controls](#-controls)

---

## 🎮 Project Overview

**Typing Master** is a 3-level typing speed game built with [Raylib](https://www.raylib.com/). Players type displayed text as accurately and quickly as possible before the timer runs out. After all 3 levels, a final statistics screen shows WPM and accuracy per level.

### What Changed in the Refactor?

| Aspect | Original (`main` branch) | Refactored (`solid-refactor` branch) |
|---|---|---|
| Files | 1 `.cpp` file | 10 `.h` + 10 `.cpp` files |
| Classes | 1 monolithic `TypingGame` | 7 focused classes + 4 interfaces |
| Testability | Hard to unit test | Each class testable in isolation |
| Extensibility | Requires modifying existing code | New features via new classes |
| Dependencies | All concrete | Abstractions via interfaces |

---

## 📁 Folder Structure

```
TypingMasterPro/
│
├── main_original.cpp          ← Original monolithic code (main branch)
│
├── src/                       ← All .cpp implementation files
│   ├── main.cpp
│   ├── GameManager.cpp
│   ├── UIRenderer.cpp
│   ├── InputHandler.cpp
│   ├── LevelLoader.cpp
│   ├── Timer.cpp
│   ├── Stats.cpp
│
├── headers/                   ← All .h header files            ← Interface (ISP)
│   ├── GameManager.hpp       ← Interface (ISP)
│   ├── Istate.hpp        ← Interface (OCP + DIP)
│   ├── StatsManager.hpp     ← Interface (OCP + ISP)
│   ├── UIRenderer.hpp
│   ├── Common.hpp
│
├── assets/
│   └── bg.jpg                 ← Background image (add your own)
│
└── README.md
```

---

## 🧱 SOLID Principles Applied

---

### S — Single Responsibility Principle (SRP)

> *"A class should have only one reason to change."*

Each class owns exactly one concern. If that concern needs to change, only that one class is modified.

#### ❌ Before — Violation (original monolithic class)

```cpp
// TypingGame does EVERYTHING — window, draw, input, timers, file I/O, stats
class TypingGame {
    void LoadLevel(int idx);       // file I/O responsibility
    void HandleInput();            // input responsibility
    void CalculateResults();       // stats responsibility
    void Update();                 // game-logic responsibility
    void Draw();                   // rendering responsibility
    void DrawMenu();               // UI responsibility
    void DrawGame();               // UI responsibility
    void DrawFinalStats();         // UI responsibility
    // 7+ responsibilities crammed into one class
};
```

#### ✅ After — Each class has ONE job

```cpp
// StateManager — ONLY tracks FSM state
class StateManager {
public:
    GameState GetState()                const;
    void      SetState(GameState state);
    bool      IsLevelStarted()          const;
    bool      IsLevelFinished()         const;
    void      SetLevelStarted(bool v);
    void      SetLevelFinished(bool v);
private:
    GameState currentState_  = GameState::Menu;
    bool      levelStarted_  = false;
    bool      levelFinished_ = false;
};
```

```cpp
// TimerManager — ONLY tracks time
class TimerManager {
public:
    void   StartCountdown();
    bool   IsCountdownDone()    const;
    double CountdownRemaining() const;
    void   StartLevel();
    double ElapsedLevelTime()   const;
    void   UpdateBlink();
    bool   IsCursorVisible()    const;
    void   AccumulateTime(double seconds);
    double TotalSessionTime()   const;
};
```

```cpp
// InputHandler — ONLY reads keyboard
class InputHandler : public IInputHandler {
public:
    static constexpr int MAX_LEN = 1000;
    void ProcessInput(std::string& userInput, bool& levelFinished) override;
};
```

```cpp
// UIManager — ONLY draws to screen
class UIManager : public IRenderer {
public:
    void DrawMenu()       override;
    void DrawGame()       override;
    void DrawFinalStats() override;
    void DrawFrame();
    void LoadBackground(const std::string& path);
};
```

| Class | Single Responsibility |
|---|---|
| `GameManager` | Orchestrate the game loop only |
| `UIManager` | All drawing / rendering only |
| `InputHandler` | Keyboard polling and input mutation only |
| `StateManager` | FSM state tracking only |
| `LevelManager` | Level data management only |
| `TimerManager` | All time-tracking concerns only |
| `StatsManager` | Storing per-level statistics only |
| `FileLevelLoader` | Loading level text from disk only |
| `WpmStatsCalculator` | WPM + accuracy calculation only |

---

### O — Open/Closed Principle (OCP)

> *"Software entities should be open for extension, closed for modification."*

New behaviour is added by writing **new classes**, not by editing existing ones.

#### ❌ Before — Violation

```cpp
// To support loading from a network you had to edit LoadLevel() directly:
void TypingGame::LoadLevel(int levelIdx) {
    // Hardcoded to file system only
    // Adding network/database support requires modifying this function
    std::ifstream file("level" + std::to_string(levelIdx + 1) + ".txt");
    // ...
}
```

#### ✅ After — Extension via interface (no existing code touched)

```cpp
// include/ILevelLoader.h — the stable abstraction (never changes)
class ILevelLoader {
public:
    virtual ~ILevelLoader() = default;
    virtual std::string LoadLevel(int levelIndex) = 0;
};
```

```cpp
// Concrete implementation 1 — reads from disk (ships with game)
// src/FileLevelLoader.cpp
std::string FileLevelLoader::LoadLevel(int levelIndex) {
    std::string filename = levelsDir_ + "level"
                         + std::to_string(levelIndex + 1) + ".txt";
    std::ifstream file(filename);
    if (!file.is_open())
        return "Error: level file not found! (" + filename + ")";

    std::ostringstream oss;
    std::string line;
    bool first = true;
    while (std::getline(file, line)) {
        if (!first) oss << '\n';
        oss << line;
        first = false;
    }
    return oss.str();
}
```

```cpp
// Concrete implementation 2 — network loader (zero changes to existing code)
class NetworkLevelLoader : public ILevelLoader {
public:
    explicit NetworkLevelLoader(const std::string& apiUrl) : apiUrl_(apiUrl) {}
    std::string LoadLevel(int levelIndex) override {
        return fetchFromApi(apiUrl_ + "/level/" + std::to_string(levelIndex));
    }
private:
    std::string apiUrl_;
};
```

```cpp
// Concrete implementation 3 — procedural generation (also zero changes)
class ProceduralLevelLoader : public ILevelLoader {
public:
    std::string LoadLevel(int levelIndex) override {
        return generateText(/* difficulty = */ levelIndex);
    }
};
```

The same pattern applies to `IStatsCalculator`:

```cpp
// include/IStatsCalculator.h
class IStatsCalculator {
public:
    virtual ~IStatsCalculator() = default;
    virtual LevelStats Calculate(const std::string& target,
                                 const std::string& input,
                                 double elapsedSeconds) = 0;
};
```

```cpp
// Built-in: net WPM — counts only correct characters
// src/WpmStatsCalculator.cpp
LevelStats WpmStatsCalculator::Calculate(const std::string& target,
                                          const std::string& input,
                                          double elapsedSeconds) {
    LevelStats s;
    int correctChars = 0;
    int minLen = (int)std::min(input.size(), target.size());
    for (int i = 0; i < minLen; i++)
        if (input[i] == target[i]) correctChars++;

    s.accuracy  = target.size() > 0
                    ? (double)correctChars / (double)target.size() * 100.0
                    : 0.0;
    s.wpm       = elapsedSeconds > 0
                    ? (input.size() / 5.0) / (elapsedSeconds / 60.0)
                    : 0.0;
    s.timeTaken = elapsedSeconds;
    return s;
}
```

```cpp
// Extension: gross WPM (all keystrokes, no accuracy penalty)
// Add this class — no existing file needs editing
class GrossWpmCalculator : public IStatsCalculator {
public:
    LevelStats Calculate(const std::string& target,
                         const std::string& input,
                         double elapsedSeconds) override {
        LevelStats s;
        s.wpm       = elapsedSeconds > 0
                        ? (input.size() / 5.0) / (elapsedSeconds / 60.0)
                        : 0.0;
        s.accuracy  = 100.0;   // gross WPM does not penalise errors
        s.timeTaken = elapsedSeconds;
        return s;
    }
};
```

---

### L — Liskov Substitution Principle (LSP)

> *"Subtypes must be substitutable for their base types without altering correctness."*

Every concrete class satisfies its interface contract fully — no silent failures, no partial implementations.

#### ✅ FileLevelLoader fully satisfies ILevelLoader

```cpp
// Contract: LoadLevel(int) ALWAYS returns a std::string — never throws silently
std::string FileLevelLoader::LoadLevel(int levelIndex) {
    std::string filename = levelsDir_ + "level"
                         + std::to_string(levelIndex + 1) + ".txt";
    std::ifstream file(filename);

    // Graceful fallback — returns descriptive error string instead of throwing
    // ANY ILevelLoader* substitution behaves predictably for the caller
    if (!file.is_open())
        return "Error: level file not found! (" + filename + ")";

    std::ostringstream oss;
    std::string line;
    bool first = true;
    while (std::getline(file, line)) {
        if (!first) oss << '\n';
        oss << line;
        first = false;
    }
    return oss.str();
}
```

#### ✅ InputHandler fully satisfies IInputHandler

```cpp
// Contract: ProcessInput ALWAYS mutates userInput and levelFinished correctly
void InputHandler::ProcessInput(std::string& userInput, bool& levelFinished) {
    int key = GetCharPressed();
    while (key > 0) {
        if ((int)userInput.size() < MAX_LEN)
            userInput += static_cast<char>(key);
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && !userInput.empty())
        userInput.pop_back();
    if (IsKeyPressed(KEY_ENTER))
        userInput += '\n';
    if (IsKeyPressed(KEY_TAB))
        levelFinished = true;  // sets flag — never throws, never ignores contract
}
```

#### ✅ UIManager fully satisfies IRenderer

```cpp
// include/IRenderer.h — all three pure virtual methods MUST be implemented
class IRenderer {
public:
    virtual ~IRenderer() = default;
    virtual void DrawMenu()       = 0;
    virtual void DrawGame()       = 0;
    virtual void DrawFinalStats() = 0;
};

// UIManager leaves no pure virtual unimplemented
class UIManager : public IRenderer {
public:
    void DrawMenu()       override;   // ✅ fully implemented
    void DrawGame()       override;   // ✅ fully implemented
    void DrawFinalStats() override;   // ✅ fully implemented
};
```

#### ✅ Substitution demonstrated in GameManager

```cpp
// GameManager holds ILevelLoader* — works with ANY conforming subtype
std::shared_ptr<ILevelLoader> loader =
    std::make_shared<FileLevelLoader>("levels/");

// Swap to NetworkLevelLoader — GameManager code is COMPLETELY UNCHANGED:
// std::shared_ptr<ILevelLoader> loader =
//     std::make_shared<NetworkLevelLoader>("https://api.example.com");

LevelManager levels(loader);  // accepts any ILevelLoader — LSP in action
```

---

### I — Interface Segregation Principle (ISP)

> *"Clients should not be forced to depend on interfaces they don't use."*

Each interface contains only the methods relevant to one single concern.

#### ❌ Before — Fat implicit interface

```cpp
// Subclassing TypingGame forces you to carry ALL of this:
class TypingGame {
    void LoadLevel(int);     // forced — don't need it for drawing
    void HandleInput();      // forced — don't need it for drawing
    void CalculateResults(); // forced — don't need it for drawing
    void Update();           // forced — don't need it for drawing
    void Draw();             // only want these four...
    void DrawMenu();
    void DrawGame();
    void DrawFinalStats();
};
```

#### ✅ After — Four lean, single-purpose interfaces

```cpp
// include/IRenderer.h — only drawing
class IRenderer {
public:
    virtual ~IRenderer() = default;
    virtual void DrawMenu()       = 0;
    virtual void DrawGame()       = 0;
    virtual void DrawFinalStats() = 0;
};
```

```cpp
// include/IInputHandler.h — only input
class IInputHandler {
public:
    virtual ~IInputHandler() = default;
    virtual void ProcessInput(std::string& userInput,
                              bool& levelFinished) = 0;
};
```

```cpp
// include/ILevelLoader.h — only level content loading
class ILevelLoader {
public:
    virtual ~ILevelLoader() = default;
    virtual std::string LoadLevel(int levelIndex) = 0;
};
```

```cpp
// include/IStatsCalculator.h — only statistics computation
struct LevelStats {
    double wpm      = 0;
    double accuracy = 0;
    double timeTaken = 0;
};

class IStatsCalculator {
public:
    virtual ~IStatsCalculator() = default;
    virtual LevelStats Calculate(const std::string& target,
                                 const std::string& input,
                                 double elapsedSeconds) = 0;
};
```

Each client depends only on the interface it actually uses:

```cpp
// StatsManager only needs IStatsCalculator — not ILevelLoader, not IRenderer
class StatsManager {
public:
    explicit StatsManager(std::shared_ptr<IStatsCalculator> calculator);
    void              Record(int lvl, const std::string& target,
                             const std::string& input, double elapsed);
    const LevelStats& Get(int levelIndex) const;
private:
    std::shared_ptr<IStatsCalculator>                  calculator_;
    std::array<LevelStats, LevelManager::TOTAL_LEVELS> stats_;
};

// src/StatsManager.cpp
StatsManager::StatsManager(std::shared_ptr<IStatsCalculator> calculator)
    : calculator_(std::move(calculator)) {}

void StatsManager::Record(int levelIndex, const std::string& target,
                           const std::string& input, double elapsedSeconds) {
    stats_[levelIndex] = calculator_->Calculate(target, input, elapsedSeconds);
}

const LevelStats& StatsManager::Get(int levelIndex) const {
    return stats_[levelIndex];
}
```

---

### D — Dependency Inversion Principle (DIP)

> *"High-level modules should not depend on low-level modules. Both should depend on abstractions."*

#### ❌ Before — High-level depends on low-level concrete details

```cpp
// TypingGame is tightly coupled to every implementation detail:
class TypingGame {
    Texture2D background;        // concrete Raylib struct embedded
    LevelStats stats[3];         // concrete array embedded
    void LoadLevel(int levelIdx) {
        std::ifstream file(...); // hardwired to std::ifstream — cannot swap
    }
    void CalculateResults() {
        // hardwired WPM formula — cannot swap algorithm without editing this
        stats[currentLevel].wpm =
            (userInput.length() / 5.0) / (finalTime / 60.0);
    }
};
```

#### ✅ After — GameManager depends only on abstractions

```cpp
// include/GameManager.h — owns interfaces, never concrete implementations
class GameManager {
private:
    StateManager                       state_;
    std::shared_ptr<ILevelLoader>      levelLoader_;  // ← abstraction
    LevelManager                       levels_;
    TimerManager                       timer_;
    std::shared_ptr<IStatsCalculator>  statsCalc_;    // ← abstraction
    StatsManager                       stats_;
    std::shared_ptr<IInputHandler>     input_;        // ← abstraction
    std::string                        userInput_;
    bool                               cursorVisible_ = true;
    std::unique_ptr<UIManager>         ui_;
};
```

```cpp
// src/GameManager.cpp — concrete types wired ONCE in the constructor
GameManager::GameManager()
    : levelLoader_(std::make_shared<FileLevelLoader>("levels/")),
      levels_(levelLoader_),
      statsCalc_(std::make_shared<WpmStatsCalculator>()),
      stats_(statsCalc_),
      input_(std::make_shared<InputHandler>())
{
    InitWindow(SCREEN_W, SCREEN_H, "Typing Master Pro ++");
    InitAudioDevice();
    SetTargetFPS(60);

    // UIManager receives const-refs — it never constructs its own dependencies
    ui_ = std::make_unique<UIManager>(
        state_, levels_, timer_, stats_, userInput_, cursorVisible_
    );
    ui_->LoadBackground("assets/bg.jpg");
}
```

```cpp
// UIManager uses constructor injection — all deps arrive from outside
// include/UIManager.h
class UIManager : public IRenderer {
public:
    UIManager(const StateManager&  state,
              const LevelManager&  levels,
              const TimerManager&  timer,
              const StatsManager&  stats,
              const std::string&   userInput,
              const bool&          cursorVisible);
    // UIManager never calls "new StateManager()" — reads only what is given
};
```

```cpp
// src/main.cpp — entry point knows NOTHING about concrete types
#include "GameManager.h"

// Dependency Inversion Principle (DIP):
// main() only touches GameManager — the high-level orchestrator.
// All concrete dependencies are wired inside GameManager's constructor.
int main() {
    GameManager game;
    game.Run();
    return 0;
}
```

---

## 🗂 Class Responsibilities

```
main()
  └── GameManager              [orchestrator — game loop only]
        ├── StateManager       [FSM: Menu / Game / Final]
        ├── LevelManager       [which level, its text, time limit]
        │     └── ILevelLoader ←── FileLevelLoader
        ├── TimerManager       [countdown, elapsed, blink, session total]
        ├── StatsManager       [store per-level WPM / accuracy]
        │     └── IStatsCalculator ←── WpmStatsCalculator
        ├── IInputHandler      ←── InputHandler
        └── UIManager          [all drawing — implements IRenderer]
```

---

## 📖 Full Class Reference

### `StateManager`

```cpp
// include/StateManager.h
enum class GameState { Menu, Game, Final };

class StateManager {
public:
    GameState GetState()                const;
    void      SetState(GameState state);
    bool      IsLevelStarted()          const;
    bool      IsLevelFinished()         const;
    void      SetLevelStarted(bool v);
    void      SetLevelFinished(bool v);
private:
    GameState currentState_  = GameState::Menu;
    bool      levelStarted_  = false;
    bool      levelFinished_ = false;
};

// src/StateManager.cpp
GameState StateManager::GetState()          const { return currentState_;  }
void      StateManager::SetState(GameState s)     { currentState_ = s;     }
bool      StateManager::IsLevelStarted()    const { return levelStarted_;  }
bool      StateManager::IsLevelFinished()   const { return levelFinished_; }
void      StateManager::SetLevelStarted(bool v)   { levelStarted_  = v;   }
void      StateManager::SetLevelFinished(bool v)  { levelFinished_ = v;   }
```

### `TimerManager`

```cpp
// include/TimerManager.h
class TimerManager {
public:
    static constexpr int    COUNTDOWN_SECONDS = 3;
    static constexpr double BLINK_INTERVAL    = 0.5;

    void   StartCountdown();
    bool   IsCountdownDone()    const;
    double CountdownRemaining() const;
    void   StartLevel();
    double ElapsedLevelTime()   const;
    void   UpdateBlink();
    bool   IsCursorVisible()    const;
    void   AccumulateTime(double seconds);
    double TotalSessionTime()   const;
private:
    double countdownStart_   = 0;
    double levelStart_       = 0;
    double blinkTimer_       = 0;
    double totalSessionTime_ = 0;
    bool   cursorVisible_    = true;
};

// src/TimerManager.cpp (key methods)
void TimerManager::StartCountdown()      { countdownStart_ = GetTime(); }
bool TimerManager::IsCountdownDone() const {
    return (GetTime() - countdownStart_) >= COUNTDOWN_SECONDS;
}
double TimerManager::CountdownRemaining() const {
    double r = COUNTDOWN_SECONDS - (GetTime() - countdownStart_);
    return r < 0 ? 0 : r;
}
void   TimerManager::StartLevel()        { levelStart_ = GetTime(); }
double TimerManager::ElapsedLevelTime() const { return GetTime() - levelStart_; }
void TimerManager::UpdateBlink() {
    if (GetTime() - blinkTimer_ >= BLINK_INTERVAL) {
        cursorVisible_ = !cursorVisible_;
        blinkTimer_    = GetTime();
    }
}
```

### `LevelManager`

```cpp
// include/LevelManager.h
class LevelManager {
public:
    static constexpr int TOTAL_LEVELS = 3;
    explicit LevelManager(std::shared_ptr<ILevelLoader> loader);

    void               LoadLevel(int index);
    const std::string& GetTargetText()   const;
    double             GetTimeLimit()    const;
    int                GetCurrentIndex() const;
    bool               HasNextLevel()    const;
private:
    std::shared_ptr<ILevelLoader> loader_;
    std::string                   targetText_;
    int                           currentIndex_ = 0;
    double timeLimits_[TOTAL_LEVELS] = { 45.0, 75.0, 120.0 };
};

// src/LevelManager.cpp
void LevelManager::LoadLevel(int index) {
    currentIndex_ = index;
    targetText_   = loader_->LoadLevel(index);
}
bool   LevelManager::HasNextLevel()    const { return (currentIndex_ + 1) < TOTAL_LEVELS; }
double LevelManager::GetTimeLimit()    const { return timeLimits_[currentIndex_]; }
int    LevelManager::GetCurrentIndex() const { return currentIndex_; }
const std::string& LevelManager::GetTargetText() const { return targetText_; }
```

### `StatsManager`

```cpp
// include/StatsManager.h
class StatsManager {
public:
    explicit StatsManager(std::shared_ptr<IStatsCalculator> calculator);
    void              Record(int levelIndex, const std::string& target,
                             const std::string& input, double elapsedSeconds);
    const LevelStats& Get(int levelIndex) const;
private:
    std::shared_ptr<IStatsCalculator>                  calculator_;
    std::array<LevelStats, LevelManager::TOTAL_LEVELS> stats_;
};
```

### `InputHandler`

```cpp
// include/InputHandler.h
class InputHandler : public IInputHandler {
public:
    static constexpr int MAX_LEN = 1000;
    void ProcessInput(std::string& userInput, bool& levelFinished) override;
};

// src/InputHandler.cpp
void InputHandler::ProcessInput(std::string& userInput, bool& levelFinished) {
    int key = GetCharPressed();
    while (key > 0) {
        if ((int)userInput.size() < MAX_LEN)
            userInput += static_cast<char>(key);
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && !userInput.empty())
        userInput.pop_back();
    if (IsKeyPressed(KEY_ENTER))
        userInput += '\n';
    if (IsKeyPressed(KEY_TAB))
        levelFinished = true;
}
```

### `GameManager` — main loop

```cpp
// src/GameManager.cpp — complete game loop logic
void GameManager::Run() {
    while (!WindowShouldClose()) {
        timer_.UpdateBlink();
        cursorVisible_ = timer_.IsCursorVisible();  // sync bool ref each frame
        Update();
        ui_->DrawFrame();
    }
}

void GameManager::UpdateGame() {
    if (!state_.IsLevelStarted()) {
        // ── Countdown phase ───────────────────────────────────────────────
        if (timer_.IsCountdownDone()) {
            state_.SetLevelStarted(true);
            timer_.StartLevel();
        }
    } else if (!state_.IsLevelFinished()) {
        // ── Typing phase ──────────────────────────────────────────────────
        bool finished = false;
        input_->ProcessInput(userInput_, finished);

        if (finished || timer_.ElapsedLevelTime() >= levels_.GetTimeLimit()) {
            state_.SetLevelFinished(true);
            stats_.Record(levels_.GetCurrentIndex(),
                          levels_.GetTargetText(),
                          userInput_,
                          timer_.ElapsedLevelTime());
        }
    } else {
        // ── Results / advance phase ───────────────────────────────────────
        if (IsKeyPressed(KEY_ENTER)) {
            timer_.AccumulateTime(
                stats_.Get(levels_.GetCurrentIndex()).timeTaken);

            if (levels_.HasNextLevel()) {
                int nextIdx = levels_.GetCurrentIndex() + 1;
                ResetForLevel();           // reset state & timer
                levels_.LoadLevel(nextIdx); // then load next level
            } else {
                state_.SetState(GameState::Final);
            }
        }
    }
}

void GameManager::ResetForLevel() {
    userInput_ = "";
    state_.SetLevelStarted(false);
    state_.SetLevelFinished(false);
    timer_.StartCountdown();
}
```

---

## 📦 Dependencies

| Dependency | Version | Purpose |
|---|---|---|
| [Raylib](https://www.raylib.com/) | ≥ 4.5 | Window, rendering, input, audio |
| CMake | ≥ 3.16 | Build system |
| C++ Standard | C++17 | `std::shared_ptr`, `std::array` |
| GCC / Clang | Any modern | Compiler |

---

## 🔨 Build Instructions

### Install Raylib (Ubuntu / Debian)

```bash
sudo apt update
sudo apt install libraylib-dev cmake build-essential
```

### Install Raylib from source (if package not available)

```bash
git clone https://github.com/raysan5/raylib.git
cd raylib/src
make PLATFORM=PLATFORM_DESKTOP
sudo make install
```

### Compile the Project

```bash
# 1. Clone the repository
git clone https://github.com/<your-username>/TypingMasterPro.git
cd TypingMasterPro

# 2. Switch to the refactored branch
git checkout solid-refactor

# 3. Create build directory
mkdir build && cd build

# 4. Configure with CMake
cmake ..

# 5. Compile
make -j$(nproc)

# 6. Run
./TypingMasterPro
```

### CMakeLists.txt (key sections explained)

```cmake
cmake_minimum_required(VERSION 3.16)
project(TypingMasterPro CXX)
set(CMAKE_CXX_STANDARD 17)

# All source files listed explicitly — safer than globbing for CMake
set(SOURCES
    src/main.cpp
    src/GameManager.cpp
    src/UIManager.cpp
    src/InputHandler.cpp
    src/StateManager.cpp
    src/LevelManager.cpp
    src/TimerManager.cpp
    src/StatsManager.cpp
    src/FileLevelLoader.cpp
    src/WpmStatsCalculator.cpp
)

add_executable(TypingMasterPro ${SOURCES})

# include/ added so #include "GameManager.h" resolves correctly
target_include_directories(TypingMasterPro PRIVATE include)

target_link_libraries(TypingMasterPro PRIVATE raylib m pthread dl)

# Auto-copy assets and levels next to the executable after every build
add_custom_command(TARGET TypingMasterPro POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:TypingMasterPro>/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/levels $<TARGET_FILE_DIR:TypingMasterPro>/levels
    COMMENT "Copying assets and levels to build directory"
)
```

> **Note:** Place your `bg.jpg` in the `assets/` folder. The game still runs without it — falls back to a plain dark background.

---

## 🌿 Git Workflow

### Step 1 — Initialise repo and commit original code to `main`

```bash
git init
git add main_original.cpp CMakeLists.txt levels/ assets/ README.md
git commit -m "feat: add original monolithic TypingGame implementation"
git branch -M main
git remote add origin https://github.com/<your-username>/TypingMasterPro.git
git push -u origin main
```

### Step 2 — Create the refactor branch

```bash
# Branch from main — original code is preserved untouched on main
git checkout -b solid-refactor
```

### Step 3 — Stage and commit each SOLID principle separately

```bash
# ── Commit 1: Interfaces (ISP + DIP) ────────────────────────────────────────
git add include/IRenderer.h \
        include/IInputHandler.h \
        include/ILevelLoader.h \
        include/IStatsCalculator.h
git commit -m "refactor(isp+dip): add segregated interfaces IRenderer, IInputHandler, ILevelLoader, IStatsCalculator"

# ── Commit 2: SRP — focused manager classes ──────────────────────────────────
git add include/StateManager.h   src/StateManager.cpp \
        include/TimerManager.h   src/TimerManager.cpp \
        include/LevelManager.h   src/LevelManager.cpp \
        include/StatsManager.h   src/StatsManager.cpp
git commit -m "refactor(srp): extract StateManager, TimerManager, LevelManager, StatsManager — each with single responsibility"

# ── Commit 3: OCP + LSP — concrete strategy classes ─────────────────────────
git add include/FileLevelLoader.h    src/FileLevelLoader.cpp \
        include/WpmStatsCalculator.h src/WpmStatsCalculator.cpp
git commit -m "refactor(ocp+lsp): add FileLevelLoader and WpmStatsCalculator as substitutable concrete strategies"

# ── Commit 4: SRP + LSP — InputHandler ──────────────────────────────────────
git add include/InputHandler.h src/InputHandler.cpp
git commit -m "refactor(srp+lsp): extract InputHandler — sole responsibility is keyboard input processing"

# ── Commit 5: SRP + DIP — UIManager ─────────────────────────────────────────
git add include/UIManager.h src/UIManager.cpp
git commit -m "refactor(srp+dip): extract UIManager implementing IRenderer — all drawing isolated, deps via constructor injection"

# ── Commit 6: DIP + SRP — GameManager + main ────────────────────────────────
git add include/GameManager.h src/GameManager.cpp src/main.cpp
git commit -m "refactor(dip+srp): add GameManager orchestrator and clean main() — all dependencies injected via interfaces"

# ── Commit 7: Build system ───────────────────────────────────────────────────
git add CMakeLists.txt
git commit -m "build: add CMakeLists.txt for multi-file SOLID-refactored project"

# ── Commit 8: Levels and assets ─────────────────────────────────────────────
git add levels/ assets/
git commit -m "assets: add sample level text files and assets directory"

# ── Commit 9: Documentation ──────────────────────────────────────────────────
git add README.md
git commit -m "docs: add comprehensive README with SOLID explanations, code examples, build steps, and Git workflow"
```

### Step 4 — Push the branch to GitHub

```bash
git push -u origin solid-refactor
```

### Step 5 — Verify your commit log

```bash
git log --oneline

# Expected output:
# a1b2c3d docs: add comprehensive README
# b2c3d4e assets: add sample level text files and assets directory
# c3d4e5f build: add CMakeLists.txt for multi-file SOLID-refactored project
# d4e5f6a refactor(dip+srp): add GameManager orchestrator and clean main()
# e5f6a7b refactor(srp+dip): extract UIManager implementing IRenderer
# f6a7b8c refactor(srp+lsp): extract InputHandler
# a7b8c9d refactor(ocp+lsp): add FileLevelLoader and WpmStatsCalculator
# b8c9d0e refactor(srp): extract StateManager, TimerManager, LevelManager, StatsManager
# c9d0e1f refactor(isp+dip): add segregated interfaces
# d0e1f2a feat: add original monolithic TypingGame implementation
```

### Step 6 — Open a Pull Request on GitHub

```
Title:   refactor: apply all SOLID principles to TypingGame
Base:    main
Compare: solid-refactor
```

---

## 📝 Commit History Guide

| # | Commit Message | Principle(s) |
|---|---|---|
| 1 | `refactor(isp+dip): add segregated interfaces` | ISP, DIP |
| 2 | `refactor(srp): extract manager classes` | SRP |
| 3 | `refactor(ocp+lsp): concrete strategy implementations` | OCP, LSP |
| 4 | `refactor(srp+lsp): extract InputHandler` | SRP, LSP |
| 5 | `refactor(srp+dip): extract UIManager with IRenderer` | SRP, DIP |
| 6 | `refactor(dip+srp): GameManager orchestrator + main()` | DIP, SRP |
| 7 | `build: add CMakeLists.txt` | — |
| 8 | `assets: sample levels and assets directory` | — |
| 9 | `docs: comprehensive README` | — |

---

## 🤖 AI Tools & Prompts Used

### Tool Used

| Tool | Model | Purpose |
|---|---|---|
| [Claude.ai](https://claude.ai) | Claude Sonnet 4.5 | Architecture design, SOLID refactoring, code generation, README authoring |

### Prompts Submitted

**Prompt 1 — Refactoring Request:**
```
I have a monolithic C/C++ project (Raylib Typing Game). I need to refactor it
into a SOLID-based Object-Oriented Design and prepare it for a GitHub assignment
submission. Follow these assignment requirements strictly:

Step 1: Assume the original project exists in the main branch. Do NOT modify original logic.
Step 2: Create a new branch named: solid-refactor
Step 3: Refactor applying all SOLID principles (SRP, OCP, LSP, ISP, DIP)
Step 4: Break into: GameManager, InputHandler, UIManager, LevelManager,
        TimerManager, StatsManager, StateManager
Step 5: Create separate header and source files for each class
Step 6: Explain how each SOLID principle is applied
Step 7: Provide folder structure: src/ include/ assets/ levels/
Step 8: Provide Git commands for creating branch, committing, pushing
Step 9: Generate commit messages for each SOLID principle
Step 10: Ensure code compiles and functionality remains unchanged

Here is my original code: [pasted main_original.cpp]
```

**Prompt 2 — README Request:**
```
give readme.md file for this using all necessary commands and used ai tools,
prompts and make the file describable for our project
```

**Prompt 3 — Code Examples Request:**
```
add code example to make it more details
```

### How AI Assisted

- **Architecture Design** — proposed the 7-class decomposition and 4-interface structure
- **SOLID Mapping** — identified which original lines violated each principle with before/after examples
- **Code Generation** — all `.h` and `.cpp` files generated and validated, preserving 100% original game logic
- **Bug Detection** — caught a double-load issue in `ResetForLevel()` during code review
- **Git Workflow** — generated atomic commit messages per principle and full push commands
- **Documentation** — this README authored from full project context with inline code examples for every principle

> **Academic Integrity Note:** All AI-generated code was reviewed, understood, and validated before submission. Use of AI tools is fully disclosed here as required.

---

## 🎮 Level Files

Plain `.txt` files in `levels/` — edit freely:

```
levels/level1.txt   ← Easy   (45 second limit)
levels/level2.txt   ← Medium (75 second limit)
levels/level3.txt   ← Hard   (120 second limit)
```

**Example `level1.txt`:**
```
The quick brown fox jumps over the lazy dog near the river bank every morning.
```

**Example `level2.txt`:**
```
Programming requires patience, precision, and a willingness to debug even the most obscure errors.
```

**Example `level3.txt`:**
```
Object-oriented design principles such as SOLID help developers build software that is maintainable and extensible.
```

Keep each level under ~200 characters per line for best display at 1300×720.

---

## ⌨️ Controls

| Key | Action |
|---|---|
| `Enter` | Start game / confirm level result |
| `Backspace` | Delete last character |
| `Tab` | Force-finish current level early |
| `ESC` | Exit (Final Stats screen only) |

---

## 📄 License

Submitted for academic purposes.  
Raylib is licensed under the [zlib License](https://github.com/raysan5/raylib/blob/master/LICENSE).

