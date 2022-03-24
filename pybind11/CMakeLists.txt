cmake_minimum_required(VERSION 3.20)

project(atomsciflow-pybind11 LANGUAGES CXX)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC ")

find_package(Boost COMPONENTS program_options filesystem system REQUIRED)
include_directories( ${Boost_INCLUDE_DIR}  )

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
#link_libraries(${YAML_CPP_LIBRARIES})
message(STATUS "YAML_CPP_INCLUDE_DIRS: ${YAML_CPP_INCLUDE_DIRS}")
message(STATUS "YAML_CPP_LIBRARY_DIRS: ${YAML_CPP_LIBRARY_DIRS}")
message(STATUS "YAML_CPP_LIBRARIES: ${YAML_CPP_LIBRARIES}")

find_package(OpenSSL REQUIRED)
if (OPENSSL_FOUND)
    message("Found OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
    message("Found OpenSSL SSL library: ${OPENSSL_SSL_LIBRARY}")
    message("Found OpenSSL Crypto library: ${OPENSSL_CRYPTO_LIBRARY}")
    #link_libraries(OpenSSL::SSL)
    #link_libraries(OpenSSL::Crypto)
endif()

# libssh2
find_package(Libssh2 CONFIG)
if (Libssh2_FOUND)
    set(LIBSSH2_FOUND_LIBS Libssh2::libssh2)
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


include_directories("../src")

# scikit-build
if(SKBUILD)
  message(STATUS "The project is built using scikit-build")
endif()


# pybind11
find_package(pybind11 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

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

# -----------------------------------------------------
# for pybind11 and scikit-build
# -----------------------------------------------------
file(GLOB UTILS_SOURCES
    "utils.cpp"
    "../src/atomsciflow/utils.cpp"
)
pybind11_add_module(utils MODULE ${UTILS_SOURCES})
#add_library(utils MODULE ${UTILS_SOURCES})
#target_link_libraries(utils PRIVATE pybind11::module)
#set_target_properties(utils PROPERTIES 
#    PREFIX "${PYTHON_MODULE_PREFIX}"
#    SUFFIX "${PYTHON_MODULE_EXTENSION}"
#)
#pybind11_add_module(utils MODULE ${UTILS_SOURCES})

file(GLOB CUBE_HANDLE_SOURCES
    "./cube_handle/*.cpp"
    "../src/atomsciflow/cube_handle/*.cpp"
    "../src/atomsciflow/parser/*.cpp"
    "../src/atomsciflow/base/*.cpp"
)
pybind11_add_module(cube_handle MODULE ${CUBE_HANDLE_SOURCES})

file(GLOB ATOMSCIFLOW_BASE_SOURCES
    "./base/*.cpp"
    "../src/atomsciflow/base/*.cpp"
)
#pybind11_add_module(base MODULE ${ATOMSCIFLOW_BASE_SOURCES})
add_library(base MODULE ${ATOMSCIFLOW_BASE_SOURCES})
target_link_libraries(base
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
)
set_target_properties(base PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(base MODULE ${ATOMSCIFLOW_BASE_SOURCES})

file(GLOB ATOMSCIFLOW_CP2K_SOURCES
    "./cp2k/*.cpp"
    "../src/atomsciflow/variable/*.cpp"
    "../src/atomsciflow/base/*.cpp"
    "../src/atomsciflow/cp2k/*.cpp" 
    "../src/atomsciflow/server/*.cpp"
    "../src/atomsciflow/remote/*.cpp"
    "../src/atomsciflow/config/*.cpp"
    "../src/atomsciflow/utils/*.cpp"
)
add_library(cp2k MODULE ${ATOMSCIFLOW_CP2K_SOURCES})
target_link_libraries(cp2k
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
)
set_target_properties(cp2k PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(cp2k MODULE ${ATOMSCIFLOW_CP2K_SOURCES})

file(GLOB ATOMSCIFLOW_QE_SOURCES
    "./qe/*.cpp"
    "../src/atomsciflow/variable/*.cpp"
    "../src/atomsciflow/base/*.cpp"
    "../src/atomsciflow/qe/*.cpp" 
    "../src/atomsciflow/server/*.cpp"
    "../src/atomsciflow/remote/*.cpp"
    "../src/atomsciflow/config/*.cpp"
    "../src/atomsciflow/utils/*.cpp"
)
add_library(qe MODULE ${ATOMSCIFLOW_QE_SOURCES})
target_link_libraries(qe
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
)
set_target_properties(qe PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(qe MODULE ${ATOMSCIFLOW_QE_SOURCES})

file(GLOB ATOMSCIFLOW_ABINIT_SOURCES
    "./abinit/abinit.cpp"
    "../src/atomsciflow/variable/*.cpp"
    "../src/atomsciflow/base/*.cpp"
    "../src/atomsciflow/abinit/*.cpp"
    "../src/atomsciflow/server/*.cpp"
    "../src/atomsciflow/remote/*.cpp"
    "../src/atomsciflow/config/*.cpp"
    "../src/atomsciflow/utils/*.cpp"
)
add_library(abinit MODULE ${ATOMSCIFLOW_ABINIT_SOURCES})
target_link_libraries(abinit
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
)
set_target_properties(abinit PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(abinit MODULE ${ATOMSCIFLOW_ABINIT_SOURCES})

file(GLOB PARSER_SOURCES
    "./parser/parser.cpp"
    "../src/atomsciflow/parser/*.cpp"
    "../src/atomsciflow/base/*.cpp"
)
#pybind11_add_module(parser MODULE ${PARSER_SOURCES})
add_library(parser MODULE ${PARSER_SOURCES})
target_link_libraries(parser
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
)
set_target_properties(parser PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(parser MODULE ${PARSER_SOURCES})

file(GLOB ATOMSCIFLOW_VARIABLE_SOURCES
    "./variable/variable.cpp"
    "../src/atomsciflow/variable/*.cpp"
)
pybind11_add_module(variable MODULE ${ATOMSCIFLOW_VARIABLE_SOURCES})

file(GLOB ATOMSCIFLOW_VASP_SOURCES
    "./vasp/vasp.cpp"
    "../src/atomsciflow/vasp/*.cpp"
    "../src/atomsciflow/vasp/post/*.cpp"
    "../src/atomsciflow/base/*.cpp"
    "../src/atomsciflow/variable/*.cpp"
    "../src/atomsciflow/server/*.cpp"
    "../src/atomsciflow/remote/*.cpp"
    "../src/atomsciflow/config/*.cpp"
    "../src/atomsciflow/utils/*.cpp"
)
#add_library(parser MODULE ${PARSER_SOURCES})
add_library(vasp MODULE ${ATOMSCIFLOW_VASP_SOURCES})
target_link_libraries(vasp
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
)
set_target_properties(vasp PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(vasp MODULE ${ATOMSCIFLOW_VASP_SOURCES})

file(GLOB ATOMSCIFLOW_CMD_SOURCES
    "./cmd/cmd.cpp"
    "../src/atomsciflow/cmd/*.cpp"
)
pybind11_add_module(cmd MODULE ${ATOMSCIFLOW_CMD_SOURCES})


file(GLOB ATOMSCIFLOW_CONFIG_SOURCES
    "./config/*.cpp"
    "../src/atomsciflow/config/*.cpp"
)
pybind11_add_module(config MODULE ${ATOMSCIFLOW_CONFIG_SOURCES})


file(GLOB ATOMSCIFLOW_GAMESSUS_SOURCES
    "./gamessus/gamessus.cpp"
    "../src/atomsciflow/gamessus/*.cpp"
    "../src/atomsciflow/base/*.cpp"
    "../src/atomsciflow/variable/*.cpp"
    "../src/atomsciflow/server/*.cpp"
    "../src/atomsciflow/remote/*.cpp"
    "../src/atomsciflow/config/*.cpp"
    "../src/atomsciflow/utils/*.cpp"
)
#add_library(gamessus MODULE ${ATOMSCIFLOW_GAMESSUS_SOURCES})
add_library(gamessus MODULE ${ATOMSCIFLOW_GAMESSUS_SOURCES})
target_link_libraries(gamessus
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
)
set_target_properties(gamessus PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(gamessus MODULE ${ATOMSCIFLOW_GAMESSUS_SOURCES})

file(GLOB ATOMSCIFLOW_ELK_SOURCES
    "./elk/elk.cpp"
    "../src/atomsciflow/elk/*.cpp"
    "../src/atomsciflow/base/*.cpp"
    "../src/atomsciflow/variable/*.cpp"
    "../src/atomsciflow/server/*.cpp"
    "../src/atomsciflow/remote/*.cpp"
    "../src/atomsciflow/config/*.cpp"
    "../src/atomsciflow/utils/*.cpp"
)
#add_library(elk MODULE ${ATOMSCIFLOW_ELK_SOURCES})
add_library(elk MODULE ${ATOMSCIFLOW_ELK_SOURCES})
target_link_libraries(elk
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
)
set_target_properties(elk PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(elk MODULE ${ATOMSCIFLOW_ELK_SOURCES})

file(GLOB ATOMSCIFLOW_QMCPACK_SOURCES
    "./qmcpack/qmcpack.cpp"
    "../src/atomsciflow/qmcpack/*.cpp"
    "../src/atomsciflow/base/*.cpp"
    "../src/atomsciflow/variable/*.cpp"
    "../src/atomsciflow/server/*.cpp"
    "../src/atomsciflow/remote/*.cpp"
    "../src/atomsciflow/config/*.cpp"
    "../src/atomsciflow/utils/*.cpp"
)
#add_library(elk MODULE ${ATOMSCIFLOW_QMCPACK_SOURCES})
add_library(qmcpack MODULE ${ATOMSCIFLOW_QMCPACK_SOURCES})
target_link_libraries(qmcpack
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
)
set_target_properties(qmcpack PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(elk MODULE ${ATOMSCIFLOW_QMCPACK_SOURCES})

file(GLOB ATOMSCIFLOW_SERVER_SOURCES
    "./server/*.cpp"
    "../src/atomsciflow/server/*.cpp"
    "../src/atomsciflow/remote/*.cpp"
    "../src/atomsciflow/config/*.cpp"
)
#pybind11_add_module(server MODULE ${ATOMSCIFLOW_SERVER_SOURCES})
add_library(server MODULE ${ATOMSCIFLOW_SERVER_SOURCES})
target_link_libraries(server
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
)
set_target_properties(server PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(server  MODULE ${ATOMSCIFLOW_SERVER_SOURCES})

# scikit-build
if(SKBUILD)
    install(TARGETS utils DESTINATION .)
    install(TARGETS cube_handle DESTINATION .)
    install(TARGETS base DESTINATION .)
    install(TARGETS cp2k DESTINATION .)
    install(TARGETS qe DESTINATION .)
    install(TARGETS abinit DESTINATION .)
    install(TARGETS parser DESTINATION .)
    install(TARGETS variable DESTINATION .)
    install(TARGETS vasp DESTINATION .)
    install(TARGETS cmd DESTINATION .)
    install(TARGETS config DESTINATION .)
    install(TARGETS gamessus DESTINATION .)
    install(TARGETS elk DESTINATION .)
    install(TARGETS qmcpack DESTINATION .)
    install(TARGETS server DESTINATION .)
endif()


