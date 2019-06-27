# Troubleshooting

## General Dlib FAQs

Dlib has a great [FAQ](http://dlib.net/faq.html) with answers to lots of common questions (e.g. _Why is dlib slow?_).

## Compile Time Errors

```console
/openFrameworks/addons/ofxDlib/libs/dlib/include/dlib/dnn/../matrix/matrix_fft.h:12:22: fatal error: mkl_dfti.h: No such file or directory
```

*Your version of MKL was likely compiled with MKL support. This is probably because you already have MKL installed. If so, simply uncomment the relevant section in your  `addon_config.mk` file. If you do not have MKL support installed, you likely need to recompile dlib without MKL support.*

```console
/openFrameworks/addons/ofxDlib/libs/dlib/lib/linux64/libdlib.a(cudnn_dlibapi.cpp.o): In function `dlib::cuda::tensor_descriptor::set_size(int, int, int, int)':
cudnn_dlibapi.cpp:(.text+0x4b): undefined reference to `cudnnDestroyTensorDescriptor'
cudnn_dlibapi.cpp:(.text+0x6f): undefined reference to `cudnnCreateTensorDescriptor'
cudnn_dlibapi.cpp:(.text+0x99): undefined reference to `cudnnSetTensor4dDescriptor'
/openFrameworks/addons/ofxDlib/libs/dlib/lib/linux64/libdlib.a(cudnn_dlibapi.cpp.o): In function `dlib::cuda::tensor_descriptor::get_size(int&, int&, int&, int&) const':
cudnn_dlibapi.cpp:(.text+0x3de): undefined reference to `cudnnGetTensor4dDescriptor'
/openFrameworks/addons/ofxDlib/libs/dlib/lib/linux64/libdlib.a(cudnn_dlibapi.cpp.o): In function `dlib::cuda::tensor_conv::clear()':
cudnn_dlibapi.cpp:(.text+0x6ae): undefined reference to `cudnnDestroyFilterDescriptor'
```

*Adjust your `addons_config.mk` file to include CUDA and / or cuDNN. See the commented sections in `addons_config.mk` for help.*

```console
Linking bin/example_face_landmark_detection for linux64
obj/linux64/Release/src/ofApp.o: In function `int dlib::lapack::gesvd<double, 0l, 3l, 0l, 3l, 3l, 1l, 3l, 3l, dlib::memory_manager_stateless_kernel_1<char> >(char, char, dlib::matrix<double, 0l, 3l, dlib::memory_manager_stateless_kernel_1<char>, dlib::row_major_layout>&, dlib::matrix<double, 3l, 1l, dlib::memory_manager_stateless_kernel_1<char>, dlib::row_major_layout>&, dlib::matrix<double, 0l, 3l, dlib::memory_manager_stateless_kernel_1<char>, dlib::row_major_layout>&, dlib::matrix<double, 3l, 3l, dlib::memory_manager_stateless_kernel_1<char>, dlib::row_major_layout>&)':
ofApp.cpp:(.text._ZN4dlib6lapack5gesvdIdLl0ELl3ELl0ELl3ELl3ELl1ELl3ELl3ENS_33memory_manager_stateless_kernel_1IcEEEEiccRNS_6matrixIT_XT0_EXT4_ET8_NS_16row_major_layoutEEERNS4_IS5_XT1_EXT5_ES6_S7_EERNS4_IS5_XT2_EXT6_ES6_S7_EERNS4_IS5_XT3_EXT7_ES6_S7_EE[_ZN4dlib6lapack5gesvdIdLl0ELl3ELl0ELl3ELl3ELl1ELl3ELl3ENS_33memory_manager_stateless_kernel_1IcEEEEiccRNS_6matrixIT_XT0_EXT4_ET8_NS_16row_major_layoutEEERNS4_IS5_XT1_EXT5_ES6_S7_EERNS4_IS5_XT2_EXT6_ES6_S7_EERNS4_IS5_XT3_EXT7_ES6_S7_EE]+0x189): undefined reference to `dgesvd_'
ofApp.cpp:(.text._ZN4dlib6lapack5gesvdIdLl0ELl3ELl0ELl3ELl3ELl1ELl3ELl3ENS_33memory_manager_stateless_kernel_1IcEEEEiccRNS_6matrixIT_XT0_EXT4_ET8_NS_16row_major_layoutEEERNS4_IS5_XT1_EXT5_ES6_S7_EERNS4_IS5_XT2_EXT6_ES6_S7_EERNS4_IS5_XT3_EXT7_ES6_S7_EE[_ZN4dlib6lapack5gesvdIdLl0ELl3ELl0ELl3ELl3ELl1ELl3ELl3ENS_33memory_manager_stateless_kernel_1IcEEEEiccRNS_6matrixIT_XT0_EXT4_ET8_NS_16row_major_layoutEEERNS4_IS5_XT1_EXT5_ES6_S7_EERNS4_IS5_XT2_EXT6_ES6_S7_EERNS4_IS5_XT3_EXT7_ES6_S7_EE]+0x293): undefined reference to `dgesvd_'
```

*Adjust your `addons_config.mk` file to `libblas` and `liblapack`. See the commented sections in `addons_config.mk` for help.*

## Runtime Errors

```console
./example_dnn_inception: error while loading shared libraries: libcudnn.so.5: cannot open shared object file: No such file or directory
/openFrameworks/libs/openFrameworksCompiled/project/makefileCommon/compile.project.mk:190: recipe for target 'run' failed
make: *** [run] Error 127
```

*You probably don't have `LD_LIBRARY_PATH` setup for using CUDA with dynamic libraries. One way to fix this is to call `export LD_LIBRARY_PATH=/usr/local/cuda/lib64` (or wherever your CUDA libraries are located) before you launch the program or add a line like*

```console
export LD_LIBRARY_PATH=/usr/local/cuda/lib64
```

*to the bottom of your `~/.bashrc` file or other appropriate location for your development environment.*

```console
Traning NN...
terminate called after throwing an instance of 'dlib::cuda_error'
  what():  Error while calling cudaGetDevice(&dev) in file /openFrameworks/scripts/apothecary/apothecary/build/dlib/dlib/dnn/cuda_dlib.cu:26. code: 35, reason: CUDA driver version is insufficient for CUDA runtime version
Segmentation fault
```

*This can happen (especially on Linux) when CUDA 8 is installed and the underlying NVIDIA drivers are too old, e.g. `nvidia-370` or `nvidia-375`. The solution is to make sure you have the latest NVIDIA drivers installed with the Driver Manager (which may require installing a more [here](https://launchpad.net/~graphics-drivers/+archive/ubuntu/ppa)).*
