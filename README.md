# Base Object Swapper

NVSE plugin and framework that allows swapping base forms at runtime
[Fallout New Vegas](https://www.nexusmods.com/newvegas/mods/83934)

## Requirements
* [CMake](https://cmake.org/)
	* Add this to your `PATH`
* [PowerShell](https://github.com/PowerShell/PowerShell/releases/latest)
* [Vcpkg](https://github.com/microsoft/vcpkg)
	* Add the environment variable `VCPKG_ROOT` with the value as the path to the folder containing vcpkg
* [Visual Studio Community 2026](https://visualstudio.microsoft.com/)
	* Desktop development with C++

## Register Visual Studio as a Generator
* Open `x86 Native Tools Command Prompt`
* Run `cmake`
* Close the cmd window

## Building
```
git clone https://github.com/powerof3/BaseObjectSwapperNV.git
cd BaseObjectSwapperNV
# pull commonlib /extern to override the path settings
git submodule init
# to update submodules to checked in build
git submodule update --recursive
```

```
cmake --preset vs2026-windows-vcpkg
cmake --build build --config Release
```
## License
[MIT](LICENSE)
