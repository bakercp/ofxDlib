## Getting Started

### General Notes
-   Recommended configuration:
    -   Ubuntu 16.04+ (or derived distro like Linux Mint 18.\*).
    -   NVIDIA 10xx Graphics Card or better.
    -   NVIDIA 375+ graphics driver.
    -   NVIDIA Cuda 8.0+
    -   NVIDIA cuDNN 5.1+
    -   Intel® Math Kernel Library (Intel® MKL)

Efforts have been made to make everything run on OSX and Linux, with and without modern NVidia graphics cards.

### On All Platforms

#### CUDA / cuDNN

CUDA/cuDNN can be installed easily on Linux, macOS and Windows with NVIDIA hardware. Take look [here](https://developer.nvidia.com/cudnn) for cuDNN and [here](https://developer.nvidia.com/cuda-zone) for CUDA.  Then run `scripts/bootstrap.sh` and recompile dlib with CUDA/cuDNN support.

> CUDA/cuDNN will not work on OSX without Xcode 8+ due to missing `std::thread_local` support and it may not work after that depending on how old your hardware is. [reference](https://stackoverflow.com/questions/28094794/why-does-apple-clang-disallow-c11-thread-local-when-official-clang-supports).

If you compile dlib with CUDA make sure you add the appropriate includes / flags in the `addons_config.make` file.

#### Intel® Math Kernel Library (Intel® MKL)

You can download MKL [here](https://software.intel.com/en-us/mkl). Follow the instructions for installation, then run `scripts/bootstrap.sh` and recompile dlib with MKL support.

Then make sure that the appropriate includes / flags in the `addons_config.make` file are available.

### On Visual Studio 

Since there is no script yet, you need to download and compile dlib by yourself. 

#### Download dlib

http://dlib.net/

#### Compile
You need CMake installed https://cmake.org/. Make sure you put the CMake folder in your Path system variable (default is `c:\Program Files\CMake\bin`).

Then configure and compile dlib using these commands in the terminal (opfen the dlib folder in the terminal)

```
mkdir build
cd build

cmake -G "Visual Studio 15 2017" -A x64 -DDLIB_USE_CUDA=1 -DUSE_AVX_INSTRUCTIONS=1 -DJEPG_INCLUDE_DIR=..\dlib\external\libjpeg -DJEPG_LIBRARY=..\dlib\external\libjpeg -DPNG_PNG_INCLUDE_DIR=..\dlib\external\libpng -DPNG_LIBRARY_RELEASE=..\dlib\external\libpng -DZLIB_INCLUDE_DIR=..\dlib\external\zlib \-DZLIB_LIBRARY_RELEASE=..\dlib\external\zlib -DCMAKE_INSTALL_PREFIX=install ..
```

```
cmake --build . --config Debug --target INSTALL
cmake --build . --config Release --target INSTALL
```

(This instruction roughly followed https://medium.com/beesightsoft/build-dlib-on-windows-534209e8340a)

#### Prepare ofxDlib
Now copy the dlib includes and libraries into the addon folder.

* folder `/build/install/include` from `[dlib download folder]` to `[of addons folder]/ofxDlib/libs/dlib/include`
* `dlib_xxx_debug_64bit_xxx.lib` from `[dlib download folder]/build/install/lib`  to `[of addons folder]/ofxDlib/libs/dlib/lib/vs/x64/Debug`
* `dlib_xxx_release_64bit_xxx.lib` from `[dlib download folder]/build/install/lib` to `[of addons folder]/ofxDlib/libs/dlib/lib/vs/x64/Release`

#### Create project
Use the project generator to create your project. Then you have to add some additional commands since the project generator is net yet able to add them automatically.

Go to project settings
* in _C/C++/preprocessor/preprocessor definitions_ add
        `DLIB_PNG_SUPPORT`  →  if you want to use the png functionality
        `DLIB_JPEG_SUPPORT` → if you want to use the jpg functionality

Now you're ready to compile!
