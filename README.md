
# 🧊 CubeWidget

**CubeWidget** is a minimal C++ application that creates a transparent, always-on-top window rendering a rotating 3D cube using OpenGL. It is ideal for embedding an unobtrusive visual element (a simple cube) on your desktop.

## ⚙️ Features

- **Transparent Background**: Window background is fully transparent, showing only the 3D cube.
- **Always on Top**: The window stays above all other applications.
- **Simple Controls**: Customize rotation speed or cube color in the source code.
- **Lightweight**: No external UI frameworks; uses Win32 API and raw OpenGL.

## 📁 Project Structure

```
CubeWidget/
├── main.cpp            # Entry point; creates transparent OpenGL window and renders cube
├── CubeWidget.h        # Header for cube rendering and window setup
├── CubeWidget.cpp      # Implementation of OpenGL initialization and rendering logic
├── shaders/            # (Optional) Folder for vertex/fragment shader files if used
└── README.md           # This documentation file
```

## 🛠️ Dependencies

- **Windows 10 or 11**
- **C++ Compiler** (Visual Studio 2022 recommended or any compiler supporting C++11)
- **OpenGL** (built-in on Windows via `opengl32.lib`)
- **GLU** (via `glu32.lib`)
- **GDI32** (via `gdi32.lib` for Win32 window handling)

## 🚀 Build Instructions

### Using Visual Studio

1. Open Visual Studio 2022.
2. Create a new **Win32 Console Application** (or **Empty Project**).
3. Add `main.cpp`, `CubeWidget.cpp`, and `CubeWidget.h` to the project.
4. In **Project Properties**:
   - Under **C/C++ → Language**, set **C++ Language Standard** to `ISO C++11`.
   - Under **Linker → Input → Additional Dependencies**, add:
     ```
     opengl32.lib
     glu32.lib
     gdi32.lib
     ```
5. Build the project (`Ctrl+Shift+B`). The output executable will be generated (e.g., `CubeWidget.exe`).

### Using Command Line with g++

(Assuming you have Windows SDK and a MinGW-like environment installed)

```batch
g++ main.cpp CubeWidget.cpp -o CubeWidget.exe -lopengl32 -lglu32 -lgdi32 -std=c++11
```

## 🧪 Usage

1. Run `CubeWidget.exe`.
2. A transparent window will appear displaying a rotating cube.
3. To modify behavior (e.g., rotation speed, cube size, color), edit constants in `CubeWidget.cpp` and rebuild.

## 📄 License

This project currently has no license. Feel free to use and modify the code for personal or educational projects. For commercial use or redistribution, please contact the author.

## 👤 Author

- GitHub: [SigmaGit1eR](https://github.com/SigmaGit1eR)

---

Enjoy a floating, transparent cube on your desktop!
