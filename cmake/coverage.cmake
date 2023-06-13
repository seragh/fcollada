#[[

Simple lcov support

Adds compile and linker flags as well as a target coverage

]]

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fkeep-inline-functions -fkeep-static-functions")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -fno-elide-constructors -fprofile-instr-generate -fcoverage-mapping")
endif()

if(NOT GCOV)
    find_program(GCOV gcov)
endif()
find_program(LCOV lcov)
find_program(GENHTML genhtml)

set(covname cov.info)
add_custom_target(coverage DEPENDS ${covname})
add_custom_command(
    OUTPUT  ${covname}
    COMMAND ${LCOV} -c -o ${covname} -d ${CMAKE_BINARY_DIR} -b ${CMAKE_BINARY_DIR}
        --gcov-tool ${GCOV} --ignore-errors mismatch
    COMMAND ${LCOV} -r ${CMAKE_BINARY_DIR}/${covname} -o ${CMAKE_BINARY_DIR}/${covname}
        '/usr/*' '*/FColladaTest/*'
    COMMAND ${LCOV} -l ${CMAKE_BINARY_DIR}/${covname}
    COMMAND ${GENHTML} ${CMAKE_BINARY_DIR}/${covname} -output ${CMAKE_BINARY_DIR}/coverage
)
