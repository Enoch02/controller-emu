# Controller Emu
A really simple (currently hardcoded) xbox controller emulator for my generic USB gamepads.
<a target="_blank" href="https://icons8.com/icon/sY9Y61aHdgZv/xbox-controller">Gamepad</a> icon by <a target="_blank" href="https://icons8.com">Icons8</a>

## Building Controller-emu from Source

### Prerequisites
- Windows 10/11
- MSYS2 with MinGW-w64 toolchain
- CMake (included with MSYS2)
- ViGEm Bus Driver installed
- HIDAPI (available via MSYS2)

### Installation Steps
- Install MSYS2 from https://www.msys2.org/

- Open MSYS2 MINGW64 terminal and install required packages: 
```
pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-gcc mingw-w64-x86_64-hidapi git make
```

- Clone the repository with submodules:
```
git clone --recursive https://github.com/Enoch02/controller-emu
cd controller-emu
```

- Create build directory and run CMake:
```
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
```

- Build the project:
```
make
```

- The compiled executable `controller-emu.exe` will be created in the `build` directory.

### Running
1. Make sure the ViGEm Bus Driver is installed
2. Connect your gamepad
3. Run `controller-emu.exe`
4. Select your gamepad from the list