include(${PROJECT_SOURCE_DIR}/config/common.cmake)
include(${PROJECT_SOURCE_DIR}/config/libs.cmake)

set(TEST_SOURCES_FILES
   ${APP_DIR_SOURCES}
   ${DESKTOP_IMPL_SOURCES}
   ${LWGPS_LIB_SOURCES}
   ${RING_BUFFER_LIB_SOURCES}
   ${COBRSR_LIB_SOURCES})

set(TEST_INCLUDE_FILES
   ${PROJECT_SOURCE_DIR}
   ${APP_DIR}
   ${TARGETS_DIR}/desktop
   ${lwgps_SOURCE_DIR}/lwgps/src/include
   ${ring_buffer_SOURCE_DIR}
   ${cobsc_SOURCE_DIR})