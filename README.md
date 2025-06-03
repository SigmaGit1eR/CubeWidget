# Transparent OpenGL Cube Widget with Position Picker (Windows only)

This application creates a semi-transparent, always-on-top, interactive OpenGL widget in the form of a rotating distorted cube. The position of the widget is chosen via a simple fullscreen overlay UI.

## Features
- Transparent, topmost OpenGL cube rendered in a separate window.
- Dynamic distortion and color transition animation.
- User can select the position via a fullscreen overlay.
- Saves and loads the cube position from `widget_pos.txt`.

## Technologies Used
- SFML (only included for linking and window context)
- Win32 API (window management)
- OpenGL + GLU for 3D rendering
- DWM API for transparency
- C++17

## Build Instructions (Windows)
Use Visual Studio or compile with the following:
```
cl /std:c++17 /EHsc main.cpp /link dwmapi.lib opengl32.lib glu32.lib
```

Make sure SFML headers and libraries are either not needed (only `<SFML/Graphics.hpp>` is included but unused) or available in your project includes.

## How to Use
1. Launch the application.
2. A fullscreen overlay will appear:
   - Click anywhere to select the cube's position.
   - Or press `S` to load the last saved position.
3. The cube will appear and begin rotating in place.

## File Storage
- The selected position is saved in `widget_pos.txt` in the current directory.

## Notes
- The SFML include is not used in the program, but may be used for future extensions.
- This application is intended for Windows only due to use of WinAPI, `dwmapi`, and GDI.
