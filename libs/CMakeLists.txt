include(FetchContent)

FetchContent_Declare(lwgps
    GIT_REPOSITORY https://github.com/MaJerle/lwgps.git
    GIT_TAG "v2.2.0"
    CONFIGURE_COMMAND ""
)
# make available
# FetchContent_MakeAvailable(bitmap)
FetchContent_Populate(lwgps)

FetchContent_Declare(ring-buffer
    GIT_REPOSITORY https://github.com/Mcublog/Ring-Buffer.git
    GIT_TAG "0.1.1"
    CONFIGURE_COMMAND ""
)
# make available
# FetchContent_MakeAvailable(bitmap)
FetchContent_Populate(ring-buffer)
