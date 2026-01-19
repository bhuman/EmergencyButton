cmake_minimum_required(VERSION 3.16)

project(EmergencyButton
    LANGUAGES C CXX
)

set(CMAKE_SYSTEM_NAME Linux)

set(F_CPU 16000000)
set(MCU atmega32u4)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_COMPILER ${PROJECT_SOURCE_DIR}/toolchain/bin/avr-gcc)
set(CMAKE_CXX_COMPILER ${PROJECT_SOURCE_DIR}/toolchain/bin/avr-g++)
set(CMAKE_C_FLAGS "-g -Wall -mcall-prologues -mmcu=${MCU} -Os -Wl,-gc-sections -Wl,-relax -MMD")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}  -fno-threadsafe-statics")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_compile_definitions(
    EMERGENCY_BUTTON_SERIAL_BAUDRATE=${EMERGENCY_BUTTON_SERIAL_BAUDRATE}
    F_CPU=${F_CPU}L
    USB_VID=0
    USB_PID=0
    ARDUINO=106
)

set(ARDUINO_LIB_DIR ${PROJECT_SOURCE_DIR}/external/ArduinoCore-avr)
include_directories(
    ${ARDUINO_LIB_DIR}/cores/arduino
    ${ARDUINO_LIB_DIR}/variants/leonardo
)

#link_libraries(
#    ${PROJECT_SOURCE_DIR}/toolchain/avr/lib/avr5/libatmega32u4.a
#    ${PROJECT_SOURCE_DIR}/toolchain/avr/lib/avr5/crtatmega32u4.o
#)

add_executable(emergency_button_avr
    src/main.cpp
    ${ARDUINO_LIB_DIR}/cores/arduino/WInterrupts.c
    ${ARDUINO_LIB_DIR}/cores/arduino/hooks.c
    ${ARDUINO_LIB_DIR}/cores/arduino/wiring.c
    ${ARDUINO_LIB_DIR}/cores/arduino/wiring_analog.c
    ${ARDUINO_LIB_DIR}/cores/arduino/wiring_digital.c
    ${ARDUINO_LIB_DIR}/cores/arduino/wiring_pulse.c
    ${ARDUINO_LIB_DIR}/cores/arduino/wiring_shift.c
    ${ARDUINO_LIB_DIR}/cores/arduino/CDC.cpp
    ${ARDUINO_LIB_DIR}/cores/arduino/HardwareSerial.cpp
    ${ARDUINO_LIB_DIR}/libraries/HID/src/HID.cpp
    ${ARDUINO_LIB_DIR}/cores/arduino/IPAddress.cpp
    ${ARDUINO_LIB_DIR}/cores/arduino/main.cpp
    ${ARDUINO_LIB_DIR}/cores/arduino/new.cpp
    ${ARDUINO_LIB_DIR}/cores/arduino/Print.cpp
    ${ARDUINO_LIB_DIR}/cores/arduino/Stream.cpp
    ${ARDUINO_LIB_DIR}/cores/arduino/USBCore.cpp
    ${ARDUINO_LIB_DIR}/cores/arduino/PluggableUSB.cpp
    ${ARDUINO_LIB_DIR}/cores/arduino/WMath.cpp
    ${ARDUINO_LIB_DIR}/cores/arduino/WString.cpp
)


