#!/bin/sh

ROOTDIR="$(cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
EMERGENCY_BUTTON_SERVER_DIR="$ROOTDIR/EmergencyButtonServer"
EMERGENCY_BUTTON_AVR_DIR="$ROOTDIR/EmergencyButtonEmbedded"
FREETZ_DIR="$EMERGENCY_BUTTON_SERVER_DIR/external/freetz-ng"
OUTPUT_DIR="$ROOTDIR/output"

NCPU="$(cat /proc/cpuinfo | grep processor | wc -l)"
TARGET=$1
test -n "$TARGET" || TARGET=all

build_button () {
    echo "Building button.."
    $EMERGENCY_BUTTON_AVR_DIR/build.sh || return

    echo "Saving output to \"$OUTPUT_DIR/emergency_button_avr\""
    mkdir -p "$OUTPUT_DIR"
    cp "$EMERGENCY_BUTTON_AVR_DIR/build/emergency_button_avr" "$OUTPUT_DIR"

    echo "Done building button"
}

build_freetz () {
    echo "Building freetz-ng.."
    cp "$ROOTDIR/Config/freetz-ng.config" "$FREETZ_DIR/.config" 
    cd "$FREETZ_DIR"
    make -j$NCPU || return
    
    echo "Saving output to \"$OUTPUT_DIR/freetz-image.img\""
    mkdir -p "$OUTPUT_DIR"
    cp "$(readlink -f "$FREETZ_DIR/images/latest.image")" "$OUTPUT_DIR/freetz-image.image"

    echo "Done building freetz-ng"
}

build_server () {
    echo "Building server.."
    $EMERGENCY_BUTTON_SERVER_DIR/build.sh || return
    
    echo "Saving output to \"$OUTPUT_DIR/emergency_button_server\" and \"$OUTPUT_DIR/emergency_button_client\""
    mkdir -p "$OUTPUT_DIR"
    cp "$EMERGENCY_BUTTON_SERVER_DIR/build/src/server/emergency_button_server" "$OUTPUT_DIR"
    cp "$EMERGENCY_BUTTON_SERVER_DIR/build/src/testclient/emergency_button_client" "$OUTPUT_DIR"
    cp "$(readlink -f "$FREETZ_DIR/images/latest.image")" "$OUTPUT_DIR/freetz-image.image"

    echo "Done building server"
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

