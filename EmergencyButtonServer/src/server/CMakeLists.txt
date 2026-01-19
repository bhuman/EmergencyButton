add_executable(emergency_button_server
    BroadcastSocket.cpp
    EmergencyButton.cpp
    Serial.cpp
    main.cpp
)

target_compile_definitions(emergency_button_server PRIVATE CMAKE_EXPORT_COMPILE_COMMANDS=1)