cmake_minimum_required(VERSION 3.5)

project(atomsciflow-test LANGUAGES CXX VERSION 0.0.1)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC ")

find_package(GTest REQUIRED)

find_package(Boost COMPONENTS program_options filesystem system REQUIRED)
include_directories( ${Boost_INCLUDE_DIR}  )

# OpenMP
find_package(OpenMP)
if(OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS"${CMAKE_EXE_LINKER_FLAGS}${OpenMP_EXE_LINKER_FLAGS}")
endif()

find_package(Armadillo REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})
link_libraries(${ARMADILLO_LIBRARIES})

#find_package(LAPACK REQUIRED)
#find_package(BLAS REQUIRED)

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

# set output path 
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # static lib
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # 
set(LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib) # 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)


add_library(atomsciflow STATIC
    ${PROJECT_LIB_SOURCES}
)
target_link_libraries(atomsciflow
    ${Boost_LIBRARIES}
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
    TARGETS atomsciflow atomsciflow-main-test.x
    EXPORT atomsciflow
    #LIBRARY DESTINATION lib
    #ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

