#!/bin/sh

ROOTDIR="$(cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
EMERGENCY_BUTTON_SERVER_DIR="$ROOTDIR/EmergencyButtonServer"
EMERGENCY_BUTTON_AVR_DIR="$ROOTDIR/EmergencyButtonEmbedded"
FREETZ_DIR="$EMERGENCY_BUTTON_SERVER_DIR/external/freetz-ng"
OUTPUT_DIR="$ROOTDIR/output"

NCPU="$(cat /proc/cpuinfo | grep processor | wc -l)"
TARGET=$1
test -n "$TARGET" || {
    echo "Please specify a target"
}

deploy_button () {
    #TODO
}

deploy_freetz () {
    echo "Is freetz-ng already running on your Fritz!Box? [y/N]"
    read ANSWER
    test -n "$ANSWER" || ANSWER=N
    ANSWER=$(echo "$ANSWER" | tr 'a-z' 'A-Z')
    if test "$ANSWER" = "Y" ; then
        echo "Then please log onto the freetz-ng interface, i.e. http://10.0.0.1:81, and deploy the image there."
        echo "The default login credentials are:"
        echo "Username: admin"
        echo "Password: freetz"
    else
        echo "Set up your NIC to have the static IP 192.168.178.2/24"
        echo "Press any button to continue"
        read
        $FREETZ_DIR/tools/push_firmware || {
            echo "Failed to push firmware, did you build freetz-ng already?"
        }
    fi
}

deploy_server () {
    echo "Please specify the address of your freetz-ng router"
    read ADDRESS
    test -n "$ADDRESS" || {
        echo "No address specified, exiting.."
        return
    }
    
    echo "Please specify the password of freetz' root user (\"freetz\" when empty)"
    read PASSWD
    test -n "$PASSWD" || PASSWD="freetz"
    
    scp "$OUTPUT_DIR/emergency_button_server" "root@$ADDRESS:/usr/bin/"

}


if test "$TARGET" = "freetz" ; then
    deploy_freetz

#elif test "$TARGET" = "server" ; then
#    deploy_server

elif test "$TARGET" = "button" ; then
    deploy_button

else
    echo "Unknown target: $TARGET"
fi

