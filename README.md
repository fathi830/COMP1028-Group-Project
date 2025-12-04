# Cyberbullying / Toxic Text Analyzer in C

This project is a C-based tool designed to detect and analyze toxic content in text files using modular programming.

---

## 1. Project Files
Ensure the following files are in the **same folder** before running:

* **Source Code:** `main.c`, `analyzer.c`, `analyzer.h`
* **Data Files:** `toxicwords.txt` (Dictionary), `comments.txt` (Input text)

---

## 2. Compile and Run

### Option A: Using Command Line (GCC/Clang) 

1. Open **Terminal** (Mac/Linux) or **Command Prompt** (Windows).
2. Navigate to the project folder using `cd`.
3. **Compile** the modular files together:

**Windows:**
```powershell
gcc main.c analyzer.c -o analyzer
```

**Mac / Linux:**
```Bash
clang main.c analyzer.c -o analyzer
```

4. Run the application:


**Windows:**
```PowerShell
.\analyzer.exe
```

**Mac / Linux:**
```Bash
./analyzer
```


### Option B: Using IDE (Visual Studio Community / VS Code) 🛠️

1. Create a new Console Project.

2. Add `main.c`, `analyzer.c`, and `analyzer.h` to the project source files.

3. Important: Ensure `toxicwords.txt` is placed inside the project directory (same folder where the `.exe` is generated).

4. Click Build and Run.

## 3. Input Requirements

1. The input text file (e.g., `comments.txt`) should be a standard `.txt` file.

2. The dictionary file (`toxicwords.txt`) must contain one toxic word per line.
