#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC ")

find_package(GTest REQUIRED)

#include_directories("./include")
include_directories("../../src")

file(GLOB PROJECT_LIB_SOURCES
    "../../src/atomsciflow/*.cpp"
    "../../src/atomsciflow/base/*.cpp"
    "../../src/atomsciflow/variable/*.cpp"
    "../../src/atomsciflow/parser/*.cpp"
    "../../src/atomsciflow/abinit/*.cpp"
    "../../src/atomsciflow/cp2k/*.cpp"
    "../../src/atomsciflow/qe/*.cpp"
    "../../src/atomsciflow/qmcpack/*.cpp"
    "../../src/atomsciflow/cube_handle/*.cpp"
    "../../src/atomsciflow/config/*.cpp"
    "../../src/atomsciflow/server/*.cpp"
    "../../src/atomsciflow/remote/*.cpp"
)

file(GLOB PROJECT_HEADER_FILES
    "../../src/atomsciflow/*.h"
    "../../src/atomsciflow/base/*.h"
    "../../src/atomsciflow/parser/*.h"
    "../../src/atomsciflow/abinit/*.h"
    "../../src/atomsciflow/cp2k/*.h"
    "../../src/atomsciflow/qe/*.h"
    "../../src/atomsciflow/qmcpack/*.h"
    "../../src/atomsciflow/cube_handle/*.h"
    "../../src/atomsciflow/config/*.h"
    "../../src/atomsciflow/server/*.h"
    "../../src/atomsciflow/remote/*.h"
)

file(GLOB PROJECT_TEST_SOURCES
    "utils/*.cpp"
    "cp2k/*.cpp"
    "qmcpack/*.cpp"
    "config/*.cpp"
)

add_executable(atomsciflow-main-test.x 
    "main.cpp"
    ${PROJECT_TEST_SOURCES}
)
target_link_libraries(atomsciflow-main-test.x
    atomsciflow
    GTest::gtest
    GTest::gtest_main
)

install(
    TARGETS atomsciflow-main-test.x
    EXPORT atomsciflow
    #LIBRARY DESTINATION lib
    #ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

