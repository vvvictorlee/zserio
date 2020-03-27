cmake_minimum_required(VERSION 3.10)

project(PmrTest)

set(CMAKE_CXX_STANDARD 11)

set(PMR_POC_GEN
    pmr_poc/ChildStruct.cpp
    pmr_poc/ChildStruct.h
    pmr_poc/SampleStruct.cpp
    pmr_poc/SampleStruct.h
)

add_executable(${PROJECT_NAME}
    PmrTest.cpp
    ${PMR_POC_GEN}
)

add_subdirectory(../runtime/src ${CMAKE_CURRENT_BINARY_DIR}/runtime)
target_compile_definitions(ZserioCppRuntime PUBLIC ZSERIO_MEMORY_RESOURCE_TRACING=1)

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ../runtime/src)
target_link_libraries(${PROJECT_NAME} ZserioCppRuntime)
