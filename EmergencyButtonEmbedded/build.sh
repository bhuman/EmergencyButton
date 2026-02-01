#!/bin/sh

ROOTDIR="$(cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
GCC_DIR="$ROOTDIR/external/gcc"
BINUTILS_DIR="$ROOTDIR/external/binutils-gdb"
LIBC_DIR="$ROOTDIR/external/avr-libc"
AVRDUDE_DIR="$ROOTDIR/external/avrdude"
TOOLCHAIN_DIR="$ROOTDIR/toolchain"
TOOLCHAIN_BUILD_DIR="$ROOTDIR/build/toolchain"
NCPU="$(cat /proc/cpuinfo | grep processor | wc -l)"


build_binutils () {
    echo "Building binutils.."
    mkdir -p "$TOOLCHAIN_BUILD_DIR/binutils"
    mkdir -p "$TOOLCHAIN_DIR"

    cd "$TOOLCHAIN_BUILD_DIR/binutils"
    "$BINUTILS_DIR/configure" --prefix="$TOOLCHAIN_DIR" --target=avr --disable-nls || {
        echo "Failed to configure binutils"
        return 1
    }

    make -j$NCPU || {
        echo "Failed to build binutils"
        return 1
    }

    make install || {
        echo "Failed to install binutils"
        return 1
    }

    return 0
}


build_gcc () {
    echo "Building GCC.."
    mkdir -p "$TOOLCHAIN_BUILD_DIR/gcc"
    mkdir -p "$TOOLCHAIN_DIR"

    cd "$GCC_DIR"
    ./contrib/download_prerequisites

    cd "$TOOLCHAIN_BUILD_DIR/gcc"
    $GCC_DIR/configure --prefix="$TOOLCHAIN_DIR" --target=avr --enable-languages=c,c++,lto --disable-nls --disable-libssp --with-dwarf2 || {
        echo "Failed to configure GCC"
        return 1
    }
    make -j$NCPU || {
        echo "Failed to build GCC"
        return 1
    }
    make install-strip || {
        echo "Failed to install GCC"
        return 1
    }

    return 0
}

build_libc () {
    echo "Building AVR-LibC.."
    mkdir -p "$TOOLCHAIN_BUILD_DIR/libc"
    mkdir -p "$TOOLCHAIN_DIR"

    cd "$LIBC_DIR"
    ./bootstrap || {
        echo "Failed to bootstrap AVR-LibC"
        return 1
    }

    cd "$TOOLCHAIN_BUILD_DIR/libc"
    $LIBC_DIR/configure --prefix="$TOOLCHAIN_DIR" --build=`./config.guess` --host=avr || {
        echo "Failed to configure AVR-LibC"
        return 1
    }
    make -j$NCPU || {
        echo "Failed to build AVR-LibC"
        return 1
    }
    make install || {
        echo "Failed to install AVR-LibC"
        return 1
    }

    return 0
}

build_avrdude () {
    echo "Building avrdude.."
    mkdir -p "$TOOLCHAIN_BUILD_DIR/avrdude"


    cd "$TOOLCHAIN_BUILD_DIR/avrdude"
    cmake "$AVRDUDE_DIR" || {
        echo "Failed to configure avrdude"
        return 1
    }

    make -j$NCPU || {
        echo "Failed to build avrdude"
        return 1
    }

    cd "$TOOLCHAIN_DIR/bin"
    ln -fs "../../build/toolchain/avrdude/src/avrdude" .
    
    return 0
}

build_project () {
    . "$ROOTDIR/../Config/config.sh"

    DEFINES=""
    for DEFINE in \
      EMERGENCY_BUTTON_SERIAL_BAUDRATE \
    ; do
        VALUE="$(eval echo \$$DEFINE)"
        DEFINES="$DEFINES -D$DEFINE=$VALUE"
        echo "Setting $DEFINE to $VALUE"
    done

    mkdir -p "$ROOTDIR/build"
    cd "$ROOTDIR/build"
    cmake $DEFINES "$ROOTDIR"
    make -j$NCPU
}

test -f "$TOOLCHAIN_DIR/bin/avr-ar" || {
    build_binutils || exit
}

test -f "$TOOLCHAIN_DIR/bin/avr-gcc" || {
    build_gcc || exit
}

test -f "$TOOLCHAIN_DIR/avr/lib/libc.a" || {
    build_libc || exit
}

test -r "$TOOLCHAIN_DIR/bin/avrdude" || {
    build_avrdude || exit
}

build_project
