# Zserio C++ runtime library and tests.
#
# This CMake file defines a static library which contains the zserio C++ runtime and an executable which
# contains the zserio C++ runtime tests.
#
# This CMake file is designed to be included directly without any further dependencies.
#

cmake_minimum_required(VERSION 2.8.12.2)

project(ZserioCppRuntimeTests)

enable_testing()

set(CPPCHECK_HOME "" CACHE PATH "Home directory of cppcheck tool. If empty, cppcheck tool is not called.")

set(ZSERIO_PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../..")
set(CMAKE_MODULE_PATH "${ZSERIO_PROJECT_ROOT}/cmake")

# setup compiler
include(compiler_utils)
compiler_set_static_clibs()
compiler_set_warnings()
compiler_set_warnings_as_errors()
set(CMAKE_CXX_STANDARD 11)

# cmake helpers
include(cmake_utils)

add_subdirectory(src)
add_subdirectory(test)

# install Zserio C++ runtime library (we release sources only)
install(DIRECTORY src/ DESTINATION cpp
        FILES_MATCHING
        PATTERN "CMakeLists.txt"
        PATTERN "*.h"
        PATTERN "*.cpp")

# add cppcheck custom command
include(cppcheck_utils)
cppcheck_add_custom_command(TARGET ZserioCppRuntime
                            SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src"
                                       "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.h"
                                       "${CMAKE_CURRENT_SOURCE_DIR}/src/*/*/*.h"
                            OPTIONS -UZSERIO_STANDARD_TYPES_HEADER)

# build and install doxygen documentation
find_package(Doxygen REQUIRED)
add_custom_target(ZserioCppRuntime-doxygen ALL
    ${CMAKE_COMMAND} -E env ZSERIO_CPP_RUNTIME_SOURCES="${CMAKE_CURRENT_SOURCE_DIR}/src"
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/doc/doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating C++ runtime HTML documentation"
)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/zserio_doc DESTINATION cpp)
