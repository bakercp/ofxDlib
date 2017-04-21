## Getting Started

### General Notes
- Recommended configuration:
  - Ubuntu 16.04+ (or derived distro like Linux Mint 18.\*).
  - NVIDIA 10xx Graphics Card or better.
  - NVIDIA 375+ graphics driver.
  - NVIDIA Cuda 8.0+
  - NVIDIA cuDNN 5.1+

Efforts have been made to make everything run on OSX and Linux, with and without modern NVidia graphics cards.

### On all platforms

Highly Recommended for DNN examples
  - Install CUDA 8+ and NVIDIA drivers if needed.
  - Install cuDNN 6+.

From the `scripts/` directory ...
- Install `coreutils` for `realpath` support (required for apothecary).
  - OSX `brew install coreutils`
  - Ubuntu ` sudo apt-get intall coreutils`
- Compile and install `dlib` _This uses `apothecary` and runs the dlib cmake scripts._
  - `./install_dlib.sh`
  - Pay attention to what options were detected during the compilation process (e.g. did it detect `CUDA` / `cuDNN`?) as your `addons_config.mk` file may require minor changes to make everything compile / link correctly.
- Download and install the example data and models. _This reads each `required_models.txt` and `required_data.txt` scripts in each `bin/data` directory._
  - `./download_models_and_data.sh`
- Check the `addons_config.mk` file to make sure all of the includes / variables are correct for your configuration. For example, if you compiled with CUDA, cuBLAS, liblapack, libblas, Intel MKL, etc, then you will likely need to uncomment a few lines in your `addons_config.mk` file.

### On OSX
- CUDA/cuDNN will not work on OSX without Xcode 8+ due to missing `std::thread_local` support and it may not work after that depending on how old your hardware is. [reference](https://stackoverflow.com/questions/28094794/why-does-apple-clang-disallow-c11-thread-local-when-official-clang-supports)

### On Linux
#### Ubuntu
  - Run the `./scripts/linux/ubuntu/install_dependencies.sh` script.

### On Windows
- Untested, but dlib is compatible, so it should be possible.
