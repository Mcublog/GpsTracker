include(${PROJECT_SOURCE_DIR}/config/emu.cmake)

# For testing using desktop gcc compiler
set(CMAKE_C_COMPILER gcc)
SET(CMAKE_CXX_COMPILER g++)

# Set defines
add_definitions(-DprojCOVERAGE_TEST=0)
add_definitions(-D_WINDOWS_)
add_definitions(-DDESKTOP)

# Buid DESKTOP target
add_executable(${EMU_TARGET}
    ${EMU_SOURCES}
)

# Include paths
target_include_directories(${EMU_TARGET} PRIVATE
    ${EMU_INCLUDES}
)

# Add pthread
target_link_libraries(${EMU_TARGET} pthread)