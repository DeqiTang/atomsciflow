# Usage:
#   find_package(atomsciflow)
#   include_directories(${atomsciflow_INCLUDE_DIR})
#   link_libraries(${atomsciflow_LIBRARIES})
# if atomsciflow install dir is not in CMAKE MODULE PATH
# cmake -DCMAKE_PREFIX_PATH=/PATH/TO/atomsciflow/CMAKE_INSTALL_PREFIX
# or add it to the CMAKE_MODULE_PATH environment variable


cmake_minimum_required(VERSION 3.20)

project(atomsciflow LANGUAGES CXX VERSION 0.0.0)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)
#include_directories("./include")
include_directories("./src")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC ")
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # -fPIC needed for build in linux

find_package(GTest REQUIRED)

find_package(Boost COMPONENTS program_options filesystem system  REQUIRED)
include_directories( 
    ${Boost_INCLUDE_DIR}  
)
link_directories(${Boost_LIBRARY_DIRS})
link_libraries(${Boost_LIBRARIES})

find_package(PkgConfig)
set(YAML_CPP_PKGCONFIG_DIR "UNSET" CACHE STRING "Specify the dir path which contains the yaml-cpp.pc file")
if (NOT ${YAML_CPP_PKGCONFIG_DIR} STREQUAL "UNSET")
    if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
        set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH};${YAML_CPP_PKGCONFIG_DIR}") # separated by semicolon
    else()
        set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${YAML_CPP_PKGCONFIG_DIR}") # separated by colon
    endif()
    message(STATUS "PKG_CONFIG_PATH: $ENV{PKG_CONFIG_PATH}")
endif()    
pkg_check_modules(YAML_CPP yaml-cpp)
include_directories(${YAML_CPP_INCLUDE_DIRS})
link_directories(${YAML_CPP_LIBRARY_DIRS})
link_libraries(${YAML_CPP_LIBRARIES})
message(STATUS "YAML_CPP_INCLUDE_DIRS: ${YAML_CPP_INCLUDE_DIRS}")
message(STATUS "YAML_CPP_LIBRARY_DIRS: ${YAML_CPP_LIBRARY_DIRS}")
message(STATUS "YAML_CPP_LIBRARIES: ${YAML_CPP_LIBRARIES}")

find_package(OpenSSL REQUIRED)
if (OPENSSL_FOUND)
    message("Found OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
    message("Found OpenSSL SSL library: ${OPENSSL_SSL_LIBRARY}")
    message("Found OpenSSL Crypto library: ${OPENSSL_CRYPTO_LIBRARY}")
    link_libraries(OpenSSL::SSL)
    link_libraries(OpenSSL::Crypto)
endif()

# libssh2
find_package(Libssh2 CONFIG)
if (Libssh2_FOUND)
    set(LIBSSH2_FOUND_LIBS Libssh2::libssh2)
    message("Found Libssh2_INCLUDE_DIRS: ${Libssh2_INCLUDE_DIRS}")
    include_directories(${Libssh2_INCLUDE_DIRS})
else ()
    find_package(PkgConfig)
    pkg_check_modules(LIBSSH2 libssh2)
    link_directories(${LIBSSH2_LIBRARY_DIRS})
    message("Found libssh2 with PkgConfig")
    message("Found LIBSSH2_LIBRARY_DIRS: ${LIBSSH2_LIBRARY_DIRS}")
    message("Found LIBSSH2_LIBRARIES: ${LIBSSH2_LIBRARIES}")
    set(LIBSSH2_FOUND_LIBS ${LIBSSH2_LIBRARIES})
endif()

if (NOT DEFINED LIBSSH2_FOUND_LIBS)
    message(STATUS "libssh2 not found, neither with cmake, nor PkgConfig")
    message(FATAL_ERROR "Stop cmake due to failure in finding libssh2") # FATAL_ERROR will stop cmake
endif()

# OpenMP
find_package(OpenMP)
if(OpenMP_FOUND)
    message("OpenMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}${OpenMP_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${OpenMP_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${OpenMP_SHARED_LINKER_FLAGS}")
    link_libraries(OpenMP::OpenMP_CXX)
    include_directories(${OpenMP_CXX_INCLUDE_DIRS})
    link_libraries(${OpenMP_CXX_LIBRARIES})    
endif()

find_package(Armadillo REQUIRED)
include_directories(${ARMADILLO_INCLUDE_DIRS})
link_libraries(${ARMADILLO_LIBRARIES})

#find_package(LAPACK REQUIRED)
#find_package(BLAS REQUIRED)


#AUX_SOURCE_DIRECTORY(./src PROJECT_LIB_SOURCES)
#file(GLOB_RECURSE PROJECT_LIB_SOURCES 
#    "./src/*" 
#    "./include/*"
#    )
file(GLOB PROJECT_LIB_SOURCES
    "./src/atomsciflow/*.cpp"
    "./src/atomsciflow/base/*.cpp"
    "./src/atomsciflow/variable/*.cpp"
    "./src/atomsciflow/parser/*.cpp"
    "./src/atomsciflow/utils/*.cpp"
    "./src/atomsciflow/abinit/*.cpp"
    "./src/atomsciflow/cp2k/*.cpp"
    "./src/atomsciflow/cp2k/post/*.cpp"
    "./src/atomsciflow/qe/*.cpp"
    "./src/atomsciflow/qmcpack/*.cpp"
    "./src/atomsciflow/elk/*.cpp"
    "./src/atomsciflow/vasp/*.cpp"
    "./src/atomsciflow/vasp/post/*.cpp"
    "./src/atomsciflow/gamessus/*.cpp"
    "./src/atomsciflow/siesta/*.cpp"
    "./src/atomsciflow/dalton/*.cpp"
    "./src/atomsciflow/octopus/*.cpp"
    "./src/atomsciflow/dftbplus/*.cpp"
    "./src/atomsciflow/exciting/*.cpp"
    "./src/atomsciflow/gromacs/*.cpp"
    "./src/atomsciflow/lammps/*.cpp"
    "./src/atomsciflow/gaussian/*.cpp"
    "./src/atomsciflow/nwchem/*.cpp"
    "./src/atomsciflow/orca/*.cpp"
    "./src/atomsciflow/qchem/*.cpp"
    "./src/atomsciflow/cube_handle/*.cpp"
    "./src/atomsciflow/remote/*.cpp"
    "./src/atomsciflow/server/*.cpp"
    "./src/atomsciflow/cmd/*.cpp"
    "./src/atomsciflow/config/*.cpp"
    "./src/atomsciflow/plumed/*.cpp"
)

file(GLOB PROJECT_HEADER_FILES
    "./src/atomsciflow/*.h"
    "./src/atomsciflow/base/*.h"
    "./src/atomsciflow/variable/*.h"
    "./src/atomsciflow/parser/*.h"
    "./src/atomsciflow/utils/*.h"
    "./src/atomsciflow/abinit/*.h"
    "./src/atomsciflow/cp2k/*.h"
    "./src/atomsciflow/cp2k/post/*.h"
    "./src/atomsciflow/qe/*.h"
    "./src/atomsciflow/qmcpack/*.h"
    "./src/atomsciflow/elk/*.h"
    "./src/atomsciflow/vasp/*.h"
    "./src/atomsciflow/vasp/post/*.h"
    "./src/atomsciflow/gamessus/*.h"
    "./src/atomsciflow/siesta/*.h"
    "./src/atomsciflow/dalton/*.h"
    "./src/atomsciflow/octopus/*.h"
    "./src/atomsciflow/dftbplus/*.h"
    "./src/atomsciflow/exciting/*.h"
    "./src/atomsciflow/gromacs/*.h"
    "./src/atomsciflow/lammps/*.h"
    "./src/atomsciflow/gaussian/*.h"
    "./src/atomsciflow/nwchem/*.h"
    "./src/atomsciflow/orca/*.h"
    "./src/atomsciflow/qchem/*.h"
    "./src/atomsciflow/post/*.h"
    "./src/atomsciflow/cube_handle/*.h"
    "./src/atomsciflow/remote/*.h"
    "./src/atomsciflow/server/*.h"
    "./src/atomsciflow/cmd/*.h"
    "./src/atomsciflow/config/*.h"
    "./src/atomsciflow/plumed/*.h"
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
    ${LIBSSH2_FOUND_LIBS}
    ${YAML_CPP_LIBRARIES}
    ${Boost_LIBRARIES}
)

option(BUILD_CMD "Whether to build Command Line executables" OFF)
if (BUILD_CMD)
    add_subdirectory(./cmd)
endif()

option(BUILD_TEST "Whether to build test" OFF)
if (BUILD_TEST)
    add_subdirectory(./test/gtest)
endif()

# ------------------------------------------------
#                   set install
# ------------------------------------------------
install(
    DIRECTORY ${CMAKE_SOURCE_DIR}/src/
    DESTINATION include #include/atomsciflow
    FILES_MATCHING
    PATTERN "*.h"
)

install(
    TARGETS atomsciflow 
    EXPORT atomsciflow
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(
    EXPORT atomsciflow
    FILE atomsciflow.cmake
    DESTINATION lib/cmake/atomsciflow
)

configure_file(./cmake/atomsciflow-config.cmake.in ${PROJECT_BINARY_DIR}/atomsciflow-config.cmake @ONLY)
install(
    FILES ${CMAKE_BINARY_DIR}/atomsciflow-config.cmake
    DESTINATION lib/cmake/atomsciflow
)
