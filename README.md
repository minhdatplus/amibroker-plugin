# amibroker-plugin

```
git clone https://github.com/minhdatplus/stocker-app
```

# Buiding
Building using Visual Studio 2019 and `vcpkg`
Visual Studio 2019 install with feature `Desktop development with C++` and `Individual feature: Windows 10 SDK, MSVC v140 - VS 2015 C++ buiding tools (v14.00)`

## Install `vcpkg`
### Install vcpkg via `git` source:
    git clone https://github.com/Microsoft/vcpkg.git
    cd vcpkg
    ./bootstrap-vcpkg.sh
###  Custom triplet
Edit default windows triplet: `x86-windows`, `x64-windows` :
```
#%vcpkg_root%/triplets/x64-windows.cmake
set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE dynamic)
set(VCPKG_PLATFORM_TOOLSET "v140")
set(VCPKG_DEP_INFO_OVERRIDE_VARS "v140")

#%vcpkg_root%/triplets/x86-windows.cmake
set(VCPKG_TARGET_ARCHITECTURE x86)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE dynamic)
set(VCPKG_PLATFORM_TOOLSET "v140")
set(VCPKG_DEP_INFO_OVERRIDE_VARS "v140")
```
### Install `cpprestsdk` via `vcpkg` and integrate with Visual Studio
```
cd %vcpkg_root%
vcpkg install cpprestsdk
vcpkg install cpprestsdk:x64-windows
vcpkg integrate project
```
Copy output last command and run it in `Visual Studio`
