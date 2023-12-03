FetchContent_GetProperties(lwgps)
FetchContent_GetProperties(ring-buffer)

message("{lwgps_SOURCE_DIR}=" ${lwgps_SOURCE_DIR})
message("{ring-buffer_SOURCE_DIR}=" ${ring-buffer_SOURCE_DIR})

file(GLOB LWGPS_LIB_SOURCES
    ${lwgps_SOURCE_DIR}/lwgps/src/lwgps/lwgps.c
)

file(GLOB RING_BUFFER_LIB_SOURCES
    ${ring-buffer_SOURCE_DIR}/ringbuffer.c
)
