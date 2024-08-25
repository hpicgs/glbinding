
# 
# External dependencies
# 

find_package(glfw3)
find_package(${META_PROJECT_NAME} REQUIRED HINTS "${CMAKE_CURRENT_SOURCE_DIR}/../../../")


# 
# Executable name and options
# 

# Target name
set(target glbinding-test)

if (NOT glfw3_FOUND)
    message("Test ${target} skipped: glfw3 not found")
    return()
else()
    message(STATUS "Test ${target}")
endif()


# 
# Sources
# 

set(sources
    main.cpp
    AllVersions_test.cpp
    RingBuffer_test.cpp
    SharedBitfield_test.cpp
    Boolean_compilation_test.cpp
    MultiContext_test.cpp
    MultiThreading_test.cpp
    
    # regession tests
    Regression_test_82.cpp
    Regression_test_185.cpp
    Regression_test_198.cpp
)


# 
# Create executable
# 

# Build executable
add_executable(${target}
    ${sources}
)

# Create namespaced alias
add_executable(${META_PROJECT_NAME}::${target} ALIAS ${target})


# 
# Project options
# 

set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    FOLDER "${IDE_FOLDER}"
)


# 
# Include directories
# 

target_include_directories(${target}
    PRIVATE
    ${DEFAULT_INCLUDE_DIRECTORIES}
    ${PROJECT_BINARY_DIR}/source/include
    ${PROJECT_SOURCE_DIR}/../glbinding/source/
    ${PROJECT_SOURCE_DIR}/../glbinding-aux/source/
)

# 
# Libraries
# 

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LIBRARIES}
    glfw
    ${META_PROJECT_NAME}::glbinding
    ${META_PROJECT_NAME}::glbinding-aux
    googletest::googletest
)


# 
# Compile definitions
# 

target_compile_definitions(${target}
    PRIVATE
    ${DEFAULT_COMPILE_DEFINITIONS}
 	BUILD_GPU_TESTS
)


# 
# Compile options
# 

target_compile_options(${target}
    PRIVATE
    ${DEFAULT_COMPILE_OPTIONS_PRIVATE}
    PUBLIC
    ${DEFAULT_COMPILE_OPTIONS_PUBLIC}
)


# 
# Linker options
# 

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LINKER_OPTIONS}
)
