# CSOPESY Semi-Major Output 1: Marquee Console

An Operating System (OS) emulator console built in C++ that features a concurrent, multi-threaded text marquee interface and an interactive command interpreter. Developed as part of the course requirement for CSOPESY.

---

## 👥 Group Developers

* **Guillermo, Iain**
* **Lee, Hannah**
* **Lim, Jenrick**

**Version Date:** 2026-09-25  
**Course:** CSOPESY

---

## 📌 Project Features

1. **Concurrent Text Marquee:** A smooth-scrolling text banner implemented using C++ multi-threading (`std::thread`), atomics (`std::atomic`), and mutexes (`std::mutex`) to prevent screen tearing and data races.
2. **Interactive Console UI:** Non-blocking character input via `conio.h` allows users to type commands without interrupting the running marquee animation.
3. **Command Interpreter:**
   * `help`: Displays the commands and their descriptions.
   * `start_marquee`: Starts the marquee animation.
   * `stop_marquee`: Stops the marquee animation.
   * `set_text`: Sets custom marquee text (e.g., set_text Hello).
   * `set_speed`: Sets refresh rate in milliseconds (e.g., set_speed 100).
   * `exit`: Terminates the console.

---

## ⚙️ Prerequisites & System Requirements

* **Operating System:** Windows (uses Windows Console APIs `<windows.h>` and `<conio.h>`).
* **Compiler:** Any modern C++ compiler supporting standard C++11 or higher (e.g., MinGW `g++`, MSVC / Visual Studio).

---

## 📁 Program Entry Point

* **Main Source File:** `main.cpp`
* **Entry Function:** `main()` located at the bottom of `main.cpp`.

---

## 🚀 Build and Run Instructions

### Option 1: Using GCC / MinGW (Command Line)

1. Open **Command Prompt** (cmd) or **PowerShell**.
2. Navigate to the directory containing `main.cpp`:
   ```cmd
   cd path/to/your/project
   ```
3. Compile the C++ program:
   ```cmd
   g++ -std=c++11 main.cpp -o MarqueeConsole.exe
   ```
4. Run the executable:
   ```cmd
   MarqueeConsole.exe
   ```

### Option 2: Using Visual Studio / Visual Studio Code

1. Open `main.cpp` in Visual Studio or VS Code (configured with C++ environment).
2. Ensure C++11 or higher standard is enabled.
3. Build and run the project (**Ctrl + F5** in Visual Studio).

---

## 💻 Available Commands & Usage

Once the application is running, enter commands into the prompt:

| Command | Usage / Example | Description |
| :--- | :--- | :--- |
| `help` | `help` | Displays the commands and their descriptions. |
| `start_marquee` | `start_marquee` | Starts the marquee animation. |
| `stop_marquee` | `stop_marquee` | Stops the marquee animation. |
| `set_text` | `set_text Hello CSOPESY!` | Sets custom marquee text (e.g., set_text Hello). |
| `set_speed` | `set_speed 100` | Sets refresh rate in milliseconds (e.g., set_speed 100). |
| `exit` | `exit` | Terminates the console. |
