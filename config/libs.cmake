FetchContent_GetProperties(lwgps)
FetchContent_GetProperties(ring_buffer)
FetchContent_GetProperties(cobsc)

# message("{lwgps_SOURCE_DIR}=" ${lwgps_SOURCE_DIR})
# message("{ring-buffer_SOURCE_DIR}=" ${ring_buffer_SOURCE_DIR})
# message("{cobsc_SOURCE_DIR}=" ${cobsc_SOURCE_DIR})

file(GLOB LWGPS_LIB_SOURCES
    ${lwgps_SOURCE_DIR}/lwgps/src/lwgps/lwgps.c
)

file(GLOB RING_BUFFER_LIB_SOURCES
    ${ring-buffer_SOURCE_DIR}/ringbuffer.c
)

file(GLOB COBRSR_LIB_SOURCES
    ${cobsc_SOURCE_DIR}/cobsr.c
)

