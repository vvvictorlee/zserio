cmake_minimum_required(VERSION 3.10)

project(PmrTest)

set(CMAKE_CXX_STANDARD 11)

option(ZSERIO_MEMORY_RESOURCE_TRACING ON "Whether to trace Zserio Memory Resources.")
option(TRACE_ALLOCATIONS ON "Whether to use allocation tracing library.")

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
if (ZSERIO_MEMORY_RESOURCE_TRACING)
    target_compile_definitions(ZserioCppRuntime PUBLIC ZSERIO_MEMORY_RESOURCE_TRACING=1)
endif ()

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ../runtime/src)
target_link_libraries(${PROJECT_NAME} ZserioCppRuntime)

if (TRACE_ALLOCATIONS)
    add_library(AllocationTracing
        allocation_tracing/AllocationTracing.cpp
    )
    target_link_libraries(${PROJECT_NAME} AllocationTracing)
endif ()
