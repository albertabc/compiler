file(GLOB HDRS "../include/*.h")

aux_source_directory(./ SRCS)

include_directories(../include)
set(TARGET_NAME "grammar-test")
add_executable(${TARGET_NAME} ${HDRS} ${SRCS})
target_link_libraries(${TARGET_NAME}  grammar-analyzer)