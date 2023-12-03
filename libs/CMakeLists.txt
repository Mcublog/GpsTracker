include(FetchContent)

FetchContent_Declare(lwgps
    GIT_REPOSITORY https://github.com/MaJerle/lwgps.git
    GIT_TAG "v2.2.0"
    CONFIGURE_COMMAND ""
)
# make available
# FetchContent_MakeAvailable(bitmap)

FetchContent_Populate(lwgps)

# message("{lwgps_SOURCE_DIR}=" ${lwgps_SOURCE_DIR})