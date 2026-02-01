#!/bin/sh

ROOTDIR="$(cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
FREETZ_DIR="$ROOTDIR/external/freetz-ng"
NCPU="$(cat /proc/cpuinfo | grep processor | wc -l)"

build_freetz_toolchain () {
    cd "$FREETZ_DIR"
    make -j$NCPU toolchain || {
        echo "Failed to build freetz toolchain"
        return 1
    }

    return 0
}

build_project () {
    . "$ROOTDIR/../Config/config.sh"
    EMERGENCY_BUTTON_SERIAL_BAUDRATE="B$EMERGENCY_BUTTON_SERIAL_BAUDRATE"

    DEFINES=""
    for DEFINE in \
      EMERGENCY_BUTTON_SERVER_SOCKET_INTERFACE \
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
    cmake $DEFINES "$ROOTDIR" || {
        echo "Failed to configure project"
        return 1
    }
    make -j$NCPU || {
        echo "Failed to build project"
        return 1
    }

    return 0
}

build_freetz_image () {

    mkdir -p "$FREETZ_DIR/addon/bhuman/root/etc/init.d"
    mkdir -p "$FREETZ_DIR/addon/bhuman/root/etc/default.bhuman"
    mkdir -p "$FREETZ_DIR/addon/bhuman/root/usr/bin"
    mkdir -p "$FREETZ_DIR/addon/bhuman/root/usr/lib/cgi-bin"
    echo "bhuman" > "$FREETZ_DIR/addon/bhuman.pkg"

    rsync -a "$ROOTDIR/build/src/server/emergency_button_server" "$FREETZ_DIR/addon/bhuman/root/usr/bin/emergency_button_server"
    rsync -a "$ROOTDIR/extra/rc.bhuman" "$FREETZ_DIR/addon/bhuman/root/etc/init.d/rc.bhuman"
    rsync -a "$ROOTDIR/extra/bhuman.cfg" "$FREETZ_DIR/addon/bhuman/root/etc/default.bhuman/bhuman.cfg"
    rsync -a "$ROOTDIR/extra/bhuman.cgi" "$FREETZ_DIR/addon/bhuman/root/usr/lib/cgi-bin/bhuman.cgi"

    cd "$FREETZ_DIR"
    make -j$NCPU || {
        echo "Failed to build freetz image"
        return 1
    }

    return 0
}

test -f "$FREETZ_DIR/toolchain/target/bin-ccache/mips-linux-uclibc-g++" || {
    build_freetz_toolchain || exit
}

build_project || exit

build_freetz_image
