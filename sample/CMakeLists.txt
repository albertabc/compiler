file(GLOB HDRS ${PROJECT_SOURCE_DIR}"/include/*.h")


include_directories(${PROJECT_SOURCE_DIR}/include)

function(add_sample SAMPLE_NAME)
    add_executable(${SAMPLE_NAME} ${HDRS} ${SAMPLE_NAME}.cpp)
    target_link_libraries(${SAMPLE_NAME} front-handler)
endfunction()

add_sample(grammar_analyze_test)
add_sample(syntax-test)
add_sample(my_script)