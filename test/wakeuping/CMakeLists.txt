include(${PROJECT_SOURCE_DIR}/config/emu.cmake)

# For testing using desktop gcc compiler
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# Set defines
add_definitions(-DprojCOVERAGE_TEST=0)
add_definitions(-D_WINDOWS_)
add_definitions(-DDESKTOP)

# Set target name
set(TEST_TARGET "working_wdt_unit")

# Buid TEST_TARGET target
add_executable(${TEST_TARGET}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${EMU_SOURCES}
)

# Include paths
target_include_directories(${TEST_TARGET} PRIVATE
    ${EMU_INCLUDES}
)

# Add pthread
target_link_libraries(${TEST_TARGET} pthread)

# Added test
add_test(NAME ${TEST_TARGET}
    COMMAND ${TEST_TARGET})