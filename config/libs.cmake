FetchContent_GetProperties(lwgps)

message("{lwgps_SOURCE_DIR}=" ${lwgps_SOURCE_DIR})
file(GLOB LWGPS_LIB_SOURCES
    ${lwgps_SOURCE_DIR}/lwgps/src/lwgps/lwgps.c
)
