file(GLOB HDRS "${PROJECT_SOURCE_DIR}/include/*.h")

include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(./ SRCS)

set(TARGET_NAME "front-handler")
add_library(${TARGET_NAME} SHARED ${HDRS} ${SRCS})
target_link_libraries(${TARGET_NAME} utils)