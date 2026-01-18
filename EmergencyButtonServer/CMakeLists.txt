cmake_minimum_required(VERSION 3.16)

project(EmergencyButton
    LANGUAGES C CXX
)

set(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS -static)




set(CMAKE_C_COMPILER ${PROJECT_SOURCE_DIR}/external/freetz-ng/toolchain/target/bin-ccache/mips-linux-uclibc-gcc)
set(CMAKE_CXX_COMPILER ${PROJECT_SOURCE_DIR}/external/freetz-ng/toolchain/target/bin-ccache/mips-linux-uclibc-g++)
add_subdirectory(src/server)


set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
add_subdirectory(src/testclient)
