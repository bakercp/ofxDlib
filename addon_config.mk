meta:
	ADDON_NAME = ofxDlib
	ADDON_DESCRIPTION = An openFrameworks wrapper for dlib.
	ADDON_AUTHOR = Christopher Baker
	ADDON_TAGS = "computer vision" "machine learning" "deep learning"
	ADDON_URL = http://github.com/bakercp/ofxDlib

common:
	ADDON_DEFINES = 
	ADDON_CFLAGS = -O3 
	
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/any/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/appveyor/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/array/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/array2d/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/base64/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/bayes_utils/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/bigint/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/binary_search_tree/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/bits/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/bit_stream/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/bound_function_pointer/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/bridge/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/bsp/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/byte_orderer/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/clustering/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/cmake_utils/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/cmd_line_parser/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/compress_stream/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/conditioning_class/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/config_reader/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/control/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/cpp_pretty_printer/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/cpp_tokenizer/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/crc32/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/cuda/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/data_io/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/directed_graph/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/dir_nav/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/disjoint_subsets/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/dnn/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/entropy_decoder/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/entropy_decoder_model/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/entropy_encoder/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/entropy_encoder_model/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/external/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/filtering/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/general_hash/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/geometry/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/global_optimization/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/graph/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/graph_cuts/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/graph_utils/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/gui_core/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/gui_widgets/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/hash_map/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/hash_set/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/hash_table/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/http_client/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/image_keypoint/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/image_loader/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/image_processing/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/image_saver/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/image_transforms/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/interfaces/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/iosockstream/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/java/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/linker/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/logger/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/lsh/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/lz77_buffer/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/lzp_buffer/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/manifold_regularization/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/map/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/matlab/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/matrix/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/md5/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/member_function_pointer/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/memory_manager/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/memory_manager_global/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/memory_manager_stateless/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/misc_api/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/mlp/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/numerical_integration/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/opencv/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/optimization/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/pipe/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/python/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/quantum_computing/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/queue/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/rand/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/random_forest/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/reference_counter/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/sequence/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/server/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/set/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/set_utils/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/simd/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/sliding_buffer/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/smart_pointers/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/sockets/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/sockstreambuf/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/sqlite/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/stack/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/static_map/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/static_set/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/statistics/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/stl_checked/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/string/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/svm/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/sync_extension/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/test/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/threads/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/timeout/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/timer/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/tokenizer/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/travis/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/tuple/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/type_safe_union/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/unicode/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/vectorstream/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/xml_parser/%
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/algs.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/any.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/array.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/array2d.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/assert.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/base64.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/bayes_utils.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/bigint.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/binary_search_tree.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/bit_stream.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/bound_function_pointer.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/bridge.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/bsp.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/byte_orderer.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/cassert
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/clustering.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/cmake
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/CMakeLists.txt
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/cmd_line_parser.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/compress_stream.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/conditioning_class.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/config.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/config.h.in
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/config_reader.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/console_progress_indicator.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/control.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/cpp_pretty_printer.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/cpp_tokenizer.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/crc32.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/cstring
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/data_io.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/directed_graph.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/dir_nav.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/disjoint_subsets.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/dlib_basic_cpp_build_tutorial.txt
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/dlib_include_path_tutorial.txt
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/dnn.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/enable_if.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/entropy_decoder.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/entropy_decoder_model.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/entropy_encoder.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/entropy_encoder_model.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/error.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/filtering.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/float_details.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/fstream
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/geometry.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/global_optimization.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/graph.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/graph_cuts.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/graph_utils.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/graph_utils_threaded.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/gui_core.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/gui_widgets.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/hash.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/hash_map.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/hash_set.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/hash_table.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/image_io.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/image_keypoint.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/image_processing.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/image_transforms.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/iomanip
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/iosfwd
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/iosockstream.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/iostream
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/istream
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/is_kind.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/LICENSE.txt
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/linker.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/list.py
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/locale
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/logger.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/lsh.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/lz77_buffer.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/lzp_buffer.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/manifold_regularization.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/map.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/matrix.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/md5.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/member_function_pointer.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/memory_manager.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/memory_manager_global.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/memory_manager_stateless.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/metaprogramming.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/misc_api.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/mlp.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/noncopyable.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/numerical_integration.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/numeric_constants.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/opencv.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/optimization.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/ostream
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/pipe.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/pixel.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/platform.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/python.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/quantum_computing.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/queue.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/rand.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/random_forest.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/ref.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/reference_counter.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/revision.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/revision.h.in
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/sequence.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/serialize.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/server.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/set.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/set_utils.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/simd.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/sliding_buffer.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/smart_pointers.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/smart_pointers_thread_safe.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/sockets.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/sockstreambuf.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/sort.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/sparse_vector.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/sqlite.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/sstream
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/stack.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/stack_trace.cpp
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/stack_trace.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/static_map.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/static_set.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/statistics.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/std_allocator.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/stl_checked.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/string.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/svm.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/svm_threaded.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/sync_extension.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/test_for_odr_violations.cpp
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/test_for_odr_violations.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/threads.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/timeout.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/timer.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/time_this.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/timing.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/tokenizer.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/tuple.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/type_safe_union.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/uintn.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/unicode.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/unordered_pair.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/vectorstream.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/windows_magic.h
	ADDON_SOURCES_EXCLUDE += libs/dlib/include/dlib/xml_parser.h

	# Manually add the includes and source.
	ADDON_INCLUDES = libs/dlib/include
	ADDON_INCLUDES += libs/ofxDlib/include
	ADDON_INCLUDES += src

osx:
	ADDON_CFLAGS += -Wno-strict-aliasing

	# Exclude includes and source.
	ADDON_SOURCES_EXCLUDE = libs/dlib/include/%
	ADDON_INCLUDES_EXCLUDE = libs/dlib/include/%
	
	ADDON_LIBS = libs/dlib/lib/osx/libdlib.a

	# Clang has problems with deep, recursive templates.
	# https://github.com/davisking/dlib/issues/229
	# Increase the depth if needed.
	ADDON_CPPFLAGS += -ftemplate-depth=512

	# If your processor supports SIMD AVX instructions.
	ADDON_CPPFLAGS += -mavx

	# If dlib is compiled with MKL support, you need to add these.
	# ADDON_INCLUDES += /opt/intel/mkl/include
	# ADDON_INCLUDES += /opt/intel/include
	# ADDON_LDFLAGS += /opt/intel/mkl/lib/libmkl_intel_ilp64.a
	# ADDON_LDFLAGS += /opt/intel/mkl/lib/libmkl_sequential.a
	# ADDON_LDFLAGS += /opt/intel/mkl/lib/libmkl_core.a
	# ADDON_LDFLAGS += -lm -ldl
	# ADDON_DEFINES += MKL_ILP64

linux64:
	ADDON_CFLAGS += -Wno-strict-aliasing

	# Exclude includes and source.
	ADDON_SOURCES_EXCLUDE = libs/dlib/include/%
	ADDON_INCLUDES_EXCLUDE = libs/dlib/include/%

	ADDON_LIBS = libs/dlib/lib/linux64/libdlib.a
	ADDON_PKG_CONFIG_LIBRARIES = libpng libjpeg
	ADDON_LDFLAGS += -lgif

	# If your processor supports SIMD AVX instructions.
	ADDON_CPPFLAGS += -mavx

	# If dlib is compiled with libblas/liblapack support, you may need to include these.
	ADDON_PKG_CONFIG_LIBRARIES += blas lapack

	# If dlib is compiled with CUDA support, you need to include these.
	# ADDON_LDFLAGS += -L/usr/local/cuda/lib64 -lcuda -lcudart -lcudnn -lcublas -lcurand -lcusolver

	# If dlib is compiled with MKL support, you need to include these.
	# ADDON_INCLUDES += /opt/intel/mkl/include

android/armeabi-v7a:
	ADDON_CFLAGS += -Wno-strict-aliasing

	# Exclude includes and source.
	ADDON_SOURCES_EXCLUDE = libs/dlib/include/%
	ADDON_INCLUDES_EXCLUDE = libs/dlib/include/%

	ADDON_LIBS = libs/dlib/lib/android/armeabi-v7a/libdlib.a
	# If your processor supports SIMD NEON instructions.
	# ADDON_CPPFLAGS += -mfpu=neon

android/x86:
	ADDON_CFLAGS += -Wno-strict-aliasing

	# Exclude includes and source.
	ADDON_SOURCES_EXCLUDE = libs/dlib/include/%
	ADDON_INCLUDES_EXCLUDE = libs/dlib/include/%
	
	ADDON_LIBS = libs/dlib/lib/android/x86/libdlib.a
	# If your processor supports SIMD AVX instructions.
	ADDON_CPPFLAGS += -mavx

vs:
	ADDON_DEFINES += DLIB_PNG_SUPPORT
	ADDON_CFLAGS = /bigobj
	
	# only in newer version of PG 
	# ADDON_CFLAGS += /bigobj

	ADDON_SOURCE_INCLUDES += libs/dlib/include/dlib/all
	ADDON_INCLUDES += libs/dlib/include/dlib/external/libpng
