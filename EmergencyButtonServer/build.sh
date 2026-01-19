#!/bin/sh

ROOTDIR="$(cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
FREETZ_DIR="$ROOTDIR/external/freetz-ng"
NCPU="$(cat /proc/cpuinfo | grep processor | wc -l)"

build_freetz_toolchain () {
    cd "$FREETZ_DIR"
    make -j$NCPU toolchain
}

build_project () {
    . "$ROOTDIR/../Config/config.sh"
    EMERGENCY_BUTTON_SERIAL_BAUDRATE="B$EMERGENCY_BUTTON_SERIAL_BAUDRATE"

    DEFINES=""
    for DEFINE in \
      EMERGENCY_BUTTON_SERIAL_BAUDRATE \
      EMERGENCY_BUTTON_CRITICAL_CONNECTION_LOST_MILLIS \
      EMERGENCY_BUTTON_DISCONNECT_MILLIS \
      EMERGENCY_BUTTON_BROADCAST_PORT \
      EMERGENCY_BUTTON_BROADCAST_INTERVAL_MILLIS \
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

test -f "$FREETZ_DIR/toolchain/target/bin-ccache/mips-linux-uclibc-g++" || {
    build_freetz_toolchain
}
build_project
