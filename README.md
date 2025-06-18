# GBA-Texture-Editor
Tool for viewing, editing, and exporting textures for GBA development. Mainly being developed on Linux.

## Setup and Building
The project uses [GLFW](https://www.glfw.org/docs/3.3/quick.html), [Glad](https://glad.dav1d.de/), and [ImGui](https://github.com/ocornut/imgui/tree/v1.91.9b-docking) for rendering and UI. Glad and ImGui are built in the project but GLFW is installed via apt.

```bash
sudo apt install libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev
```

Then, configuring with cmake:

```bash
mkdir build
cd build/
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

There are VS Code configs for building and running the app with gdb. Just press F5 and the system will build and run.
