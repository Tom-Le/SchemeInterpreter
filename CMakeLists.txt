CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)
set(CMAKE_LEGACY_CYGWIN_WIN32 0)
CMAKE_POLICY(VERSION 3.2.3)

PROJECT(SchemeInterpreter VERSION 1.0)

ENABLE_TESTING()

set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")
set(CMAKE_C_STANDARD 99)

# Generate config file.
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/src/config-info.h.in
               ${CMAKE_BINARY_DIR}/generated/config-info.h)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/main/
                    ${CMAKE_SOURCE_DIR}/src/modules/
                    ${CMAKE_SOURCE_DIR}/src/types/
                    ${CMAKE_BINARY_DIR}/generated/)

ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/src/modules/)
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/src/types/)
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/src/procedures/)
ADD_SUBDIRECTORY(${CMAKE_SOURCE_DIR}/src/main/)

