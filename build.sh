#!/bin/sh

ROOTDIR="$(cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
EMERGENCY_BUTTON_SERVER_DIR="$ROOTDIR/EmergencyButtonServer"
FREETZ_DIR="$EMERGENCY_BUTTON_SERVER_DIR/external/freetz-ng"

NCPU="$(cat /proc/cpuinfo | grep processor | wc -l)"
TARGET=$1
test -n "$TARGET" || TARGET=all

build_avr () {
    echo ""
}

build_freetz () {
    echo "Building freetz-ng.."
    cp "$ROOTDIR/Config/freetz-ng.config" "$FREETZ_DIR/.config" 
    cd "$FREETZ_DIR"
    make -j$NCPU || return

    echo "Done building freetz-ng"
}

build_server () {
    echo "Building server.."
    $EMERGENCY_BUTTON_SERVER_DIR/build.sh || return

    echo "Done building server"
}

build_button () {
}


if test "$TARGET" = "freetz" ; then
    build_freetz

elif test "$TARGET" = "server" ; then
    build_server

elif test "$TARGET" = "button" ; then
    build_button

elif test "$TARGET" = "all" ; then
    build_freetz
    build_button
    build_server

else
    echo "Unknown target: $TARGET"
fi

