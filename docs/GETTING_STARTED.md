# Getting Started

## General Notes

- For performance, run in RELEASE mode. Running in DEBUG mode is _significantly_ slower. For more see [http://dlib.net/faq.html#Whyisdlibslow](http://dlib.net/faq.html#Whyisdlibslow).

- Recommended configuration:
  - Ubuntu 16.04+ (or derived distro like Linux Mint 18.\*).
  - NVIDIA 10xx Graphics Card or better.
  - NVIDIA 375+ graphics driver.
  - NVIDIA Cuda 8.0+
  - NVIDIA cuDNN 5.1+
  - Intel® Math Kernel Library (Intel® MKL)

Efforts have been made to make everything run on macOS and Linux, with and without modern NVidia graphics cards.

## On All Platforms

### CUDA / cuDNN

CUDA/cuDNN can be installed on Linux, macOS and Windows with NVIDIA hardware. Take look [here](https://developer.nvidia.com/cudnn) for cuDNN and [here](https://developer.nvidia.com/cuda-zone) for CUDA.  Then run `scripts/./bootstrap.sh` and recompile dlib with CUDA/cuDNN support.

> CUDA/cuDNN will not work on OSX without Xcode 8+ due to missing `std::thread_local` support and it may not work after that depending on how old your hardware is. [reference](https://stackoverflow.com/questions/28094794/why-does-apple-clang-disallow-c11-thread-local-when-official-clang-supports).

If you compile dlib with CUDA make sure you add the appropriate includes / flags in the `addons_config.make` file.

### Intel® Math Kernel Library (Intel® MKL)

You can download MKL [here](https://software.intel.com/en-us/mkl). Follow the instructions for installation, then run `scripts/bootstrap.sh` and recompile dlib with MKL support.

Then make sure that the appropriate includes / flags in the `addons_config.make` file are available.
