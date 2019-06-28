#! /bin/bash
#
# DLIB
#
# uses CMake

# define the version
VER=19.17

# tools for git use
GIT_URL="https://github.com/davisking/dlib"
GIT_TAG="v$VER"
GIT_TAG=

FORMULA_TYPES=( "osx" "android" "linux64" "linuxarmv6l")

# download the source code and unpack it into LIB_NAME
function download() {
    # curl -Lk http://dlib.net/files/dlib-$VER.tar.bz2 -o dlib-$VER.tar.bz2
    # tar -xjvf dlib-$VER.tar.bz2
    # mv dlib-$VER dlib
    # rm dlib-$VER.tar.bz2
    git clone https://github.com/davisking/dlib.git
}

# prepare the build environment, executed inside the lib src dir
function prepare() {
    if [ "$TYPE" == "android" ] ; then
        cp -rf $FORMULA_DIR/jni/  jni
    fi
}

# executed inside the lib src dir
function build() {
    if [ "$TYPE" == "osx" ] || [[ "$TYPE" == linux* ]] ; then
        mkdir -p "build"
        pushd "build" || return
        if [ "$TYPE" == "linuxarmv6l" ] ; then
            export MAKEFLAGS="-j3 -s"
        else
            export MAKEFLAGS="-j$PARALLEL_MAKE -s"
        fi
        if [ "$TYPE" == "osx" ] ; then
            # Disable JPEG support because it conflicts with FreeImage's libjpeg (duplicate symbols).
            # This means we can't use dlib::load_image. Instead we just use ofLoadImage(...).
            # -D DLIB_JPEG_SUPPORT=OFF \
            #
            # Strangely, it works in Xcode but not w/ makefiles or QTCreator.
            # -D DLIB_ENABLE_ASSERTS=ON \
            cmake \
              -D DLIB_JPEG_SUPPORT=OFF \
              -DUSE_SSE2_INSTRUCTIONS=ON \
              -DUSE_SSE4_INSTRUCTIONS=ON \
              -DUSE_AVX_INSTRUCTIONS=ON \
              -D CMAKE_OSX_DEPLOYMENT_TARGET=10.9 \
              -D DLIB_NO_GUI_SUPPORT=YES \
              -D DLIB_GIF_SUPPORT=OFF \
              -D CMAKE_INSTALL_PREFIX="${LIBS_DIR}/dlib/install" \
              ..
        else
            cmake \
              -DUSE_SSE2_INSTRUCTIONS=ON \
              -DUSE_SSE4_INSTRUCTIONS=ON \
              -DUSE_AVX_INSTRUCTIONS=ON \
              -D DLIB_NO_GUI_SUPPORT=YES \
              -D CMAKE_INSTALL_PREFIX="${LIBS_DIR}/dlib/install" \
              ..
        fi

        cmake --build . --config Release
        popd || return
    elif [ "$TYPE" == "android" ] ; then
        ${NDK_ROOT}/ndk-build -j4 NDK_DEBUG=0 NDK_PROJECT_PATH=.
    fi
}

# executed inside the lib src dir, first arg $1 is the dest libs dir root
function copy() {
    # headers
    if [ -d $1/include ]; then
        rm -rf $1/include
    fi

    mkdir -p $1/include
    mkdir -p $1/lib/$TYPE

    if [ "$TYPE" == "osx" ] || [[ "$TYPE" == linux* ]] ; then
        cd "${BUILD_DIR}/dlib/build" || exit 1
        make install
        cd - || exit 1
        mv $1/install/lib/libdlib.a $1/lib/$TYPE/
        mv $1/install/include/dlib $1/include/
        rm -r $1/install
    elif [ "$TYPE" == "android" ] ; then
        cp -vr dlib/ $1/include/dlib
        rm -rf $1/include/dlib/build
        rm -rf $1/include/dlib/test
        rm  $1/include/dlib/all_gui.cpp
        cp -vr obj/local/armeabi-v7a/libdlib.a $1/lib/android/armeabi-v7a/libdlib.a
        cp -vr obj/local/x86/libdlib.a $1/lib/android/x86/libdlib.a
    fi
}

# executed inside the lib src dir
function clean() {
    if [ "$TYPE" == "osx" ] || [[ "$TYPE" == linux* ]] ; then
        # If we can't go to that dir, then there's nothing to clean.
        if [ -d "${BUILD_DIR}/dlib/build" ] ; then
            cd "${BUILD_DIR}/dlib/build"
            cmake clean .
            cd ..
            rm -rf build
            cd ..
        fi
    elif [ "$TYPE" == "android" ] ; then
        rm -rf obj
    fi

}
