include(FetchContent)

# lwgps
FetchContent_Declare(lwgps
    GIT_REPOSITORY https://github.com/MaJerle/lwgps.git
    GIT_TAG "v2.2.0"
    CONFIGURE_COMMAND ""
)

FetchContent_Populate(lwgps)

# ring-buffer
FetchContent_Declare(ring-buffer
    GIT_REPOSITORY https://github.com/Mcublog/Ring-Buffer.git
    GIT_TAG "0.1.1"
    CONFIGURE_COMMAND ""
)

FetchContent_Populate(ring-buffer)

# cobs-c
FetchContent_Declare(cobs-c
    GIT_REPOSITORY https://github.com/cmcqueen/cobs-c.git
    GIT_TAG "0.5.0"
    CONFIGURE_COMMAND ""
)
# make available
# FetchContent_MakeAvailable(bitmap)
FetchContent_Populate(cobs-c)
