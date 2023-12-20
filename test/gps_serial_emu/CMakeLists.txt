include(${PROJECT_SOURCE_DIR}/config/tests.cmake)

# For testing using desktop gcc compiler
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# Set defines
add_definitions(-DprojCOVERAGE_TEST=0)
add_definitions(-D_WINDOWS_)
add_definitions(-DDESKTOP)

# Set target name
set(TEST_TARGET "gps_serial_emu_test")

# Buid TEST_TARGET target
add_executable(${TEST_TARGET}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${TEST_SOURCES_FILES}
)

# Include paths
target_include_directories(${TEST_TARGET} PRIVATE
    ${TEST_INCLUDE_FILES}
)

# Add pthread
target_link_libraries(${TEST_TARGET} pthread)