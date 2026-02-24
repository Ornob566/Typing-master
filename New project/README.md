# Typing Master — Raylib C Project

**Prompt:**

> "Develop a C program using the Raylib library for a 'Typing Master' clone.
>
> **Core Architecture:** Implement a state machine with three states: `MENU`, `GAME`, and `FINAL`.
>
> **Level & Data Management:**
> - Support 3 levels by loading target text from external files: `level1.txt`, `level2.txt`, and `level3.txt`.
> - Use a struct or arrays to store results (Time, WPM, and Accuracy) for each level to display later.
>
> **Gameplay Mechanics:**
> - **Countdown:** Start each level with a 3-second non-interactive countdown.
> - **Real-time Input:** Capture characters using `GetCharPressed()`. Support `BACKSPACE` for deletion and `ENTER` for manual line breaks.
> - **Visual Feedback:** Render the target text. Compare user input character-by-character; render correct characters in Green and incorrect ones in Red.
> - **Indicators:** Include a blinking yellow cursor at the current typing position.
> - **End Conditions:** A level ends when the time limit (45s, 75s, or 120s) is reached or when the user presses `TAB`.
>
> **Multimedia & UI:**
> - **Graphics:** Scale a background image (`bg.jpg`) to a 1300 × 720 window.
> - **Audio:** Implement looping background music (`sound.mp3`) and specific sound effects for key presses, enter, backspace, and level completion.
>
> **Result Summary:** After all levels, show a final screen displaying a table of statistics for each level and the total time taken."

---

## 2. Technical Breakdown of the Code

### A. The State Machine Logic

The program uses a `typedef enum` to control the flow. This prevents the menu logic from running while the user is typing.

- **MENU:** Checks for `IsKeyPressed(KEY_ENTER)` to transition to the Game.
- **GAME:** Contains the core typing engine. It handles the 3-second delay, starts the timer, and captures keystrokes.
- **FINAL:** Iterates through the stored data in `levelTimes[]`, `levelAccuracy[]`, and `levelWPM[]` to print the summary.

---

### B. Typing Engine & Visual "Diff"

The code performs a real-time comparison. Instead of checking the whole string at once, it loops through the `userInput` buffer:

```c
Color color = (i < strlen(targetText) && userInput[i] == targetText[i]) ? GREEN : RED;
```

It draws each character one by one. If it encounters a `\n` in the input, it resets the `x` coordinate and increments the `y` coordinate to simulate a new line.

---

### C. Performance Metrics (The Math)

The program calculates typing skill based on these formulas:

**1. Words Per Minute (WPM)**

The standard "word" is 5 characters.

$$WPM = \frac{(\text{Total Characters} / 5)}{\text{Time in Minutes}}$$

**2. Accuracy**

Calculated as the percentage of correct characters relative to the total length of the required text.

$$\text{Accuracy \%} = \left( \frac{\text{Correct Characters}}{\text{Target Length}} \right) \times 100$$

---

### D. Audio Management

Unlike simple sound effects, the background music uses **Streaming**. This is why `UpdateMusicStream(backgroundMusic)` is called inside the `while` loop — it tells the CPU to keep feeding chunks of the audio file to the speakers.

---

## 3. Suggested Improvements

- **Word Wrapping:** Currently, if a line in the text file is longer than the screen width, it will overflow. You could add logic using `MeasureText()` to automatically move to a new line.

- **Case Sensitivity Toggle:** You could add a feature to ignore Capital/Small letter mistakes.

- **Live WPM:** You could move the WPM calculation inside the typing loop so the user can see their speed changing in real-time.
