file(GLOB HDRS "../include/*.h")

aux_source_directory(./ SRCS)

include_directories(../include)
set(TARGET_NAME "grammar-analyzer")
add_library(${TARGET_NAME} SHARED ${HDRS} ${SRCS})