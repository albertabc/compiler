file(GLOB HDRS "${PROJECT_SOURCE_DIR}/include/*.h")

message(${PROJECT_SOURCE_DIR})

include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(./ SRCS)

set(TARGET_NAME "utils")
add_library(${TARGET_NAME} SHARED ${HDRS} ${SRCS})