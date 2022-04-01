cmake_minimum_required(VERSION 3.20)

project(atomsciflow-pybind11 LANGUAGES CXX)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC ")

find_package(Boost COMPONENTS program_options filesystem system REQUIRED)
link_directories(${Boost_LIBRARY_DIRS})
include_directories(
    ${Boost_INCLUDE_DIR}
)

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

if (DEFINED Libssh2_DIR)
    message("including ${Libssh2_DIR}/include")
    include_directories(${Libssh2_DIR}/include)
    link_directories("${Libssh2_DIR}/lib")
endif()
find_package(Libssh2 CONFIG)
if (Libssh2_FOUND)
    message("Libssh2_FOUND with find_package")
    set(LIBSSH2_FOUND_LIBS Libssh2::libssh2)
    include_directories(${Libssh2_INCLUDE_DIRS})
else ()
    find_package(PkgConfig)
    pkg_check_modules(LIBSSH2 libssh2)
    if (LIBSSH2_FOUND)
        include_directories(${LIBSSH2_INCLUDE_DIRS})
        link_directories(${LIBSSH2_LIBRARY_DIRS})
        message("Found libssh2 with PkgConfig, the prefix: ${LIBSSH2_PREFIX}")
        message("Found LIBSSH2_LIBRARY_DIRS: ${LIBSSH2_LIBRARY_DIRS}")
        message("Found LIBSSH2_LIBRARIES: ${LIBSSH2_LIBRARIES}")
        set(LIBSSH2_FOUND_LIBS ${LIBSSH2_LIBRARIES})
    endif()
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

# ------------------------------------------------------------------------
# Build commonly used sources under atomsicflow into individual static lib
# ------------------------------------------------------------------------
# There are many module binding to be built in atomsciflow via pybind 11.
# And some of the modules may share the same atomsciflow c++ codes, 
# We build these shared codes into static libs and link to the pybind11 
# output modules that need them, so that we don't need to compile them 
# repetively when building each module later.
file(GLOB ATOMSCIFLOW_BASE_SOURCES
    "../src/atomsciflow/base/*.cpp"
)
add_library(atomsciflow-base STATIC
    ${ATOMSCIFLOW_BASE_SOURCES}
)
file(GLOB ATOMSCIFLOW_VARIABLE_SOURCES
    "../src/atomsciflow/variable/*.cpp"
)
add_library(atomsciflow-variable STATIC
    ${ATOMSCIFLOW_VARIABLE_SOURCES}
)
file(GLOB ATOMSCIFLOW_SERVER_SOURCES
    "../src/atomsciflow/server/*.cpp"
)
add_library(atomsciflow-server STATIC
    ${ATOMSCIFLOW_SERVER_SOURCES}
)
file(GLOB ATOMSCIFLOW_REMOTE_SOURCES
    "../src/atomsciflow/remote/*.cpp"
)
add_library(atomsciflow-remote STATIC
    ${ATOMSCIFLOW_REMOTE_SOURCES}
)
file(GLOB ATOMSCIFLOW_CONFIG_SOURCES 
    "../src/atomsciflow/config/*.cpp"
)
add_library(atomsciflow-config STATIC
    ${ATOMSCIFLOW_CONFIG_SOURCES}
)
file(GLOB ATOMSCIFLOW_UTILS_SOURCES
    "../src/atomsciflow/utils/*.cpp"
)
add_library(atomsciflow-utils STATIC
    ${ATOMSCIFLOW_UTILS_SOURCES}
)

# -----------------------------------------------------
# for pybind11 and scikit-build
# -----------------------------------------------------
file(GLOB BINDING_UTILS_SOURCES
    "utils.cpp"
    "../src/atomsciflow/utils.cpp"
)
pybind11_add_module(utils MODULE ${BINDING_UTILS_SOURCES})
#add_library(utils MODULE ${BINDING_UTILS_SOURCES})
#target_link_libraries(utils PRIVATE pybind11::module)
#set_target_properties(utils PROPERTIES 
#    PREFIX "${PYTHON_MODULE_PREFIX}"
#    SUFFIX "${PYTHON_MODULE_EXTENSION}"
#)
#pybind11_add_module(utils MODULE ${BINDING_UTILS_SOURCES})

file(GLOB BINDING_CUBE_HANDLE_SOURCES
    "./cube_handle/*.cpp"
    "../src/atomsciflow/cube_handle/*.cpp"
    "../src/atomsciflow/parser/*.cpp"
    "../src/atomsciflow/base/*.cpp"
)
pybind11_add_module(cube_handle MODULE ${BINDING_CUBE_HANDLE_SOURCES})

file(GLOB BINDING_BASE_SOURCES
    "./base/*.cpp"
    #"../src/atomsciflow/base/*.cpp"
)
#pybind11_add_module(base MODULE ${BINDING_BASE_SOURCES})
add_library(base MODULE ${BINDING_BASE_SOURCES})
target_link_libraries(base
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    atomsciflow-base
)
set_target_properties(base PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(base MODULE ${BINDING_BASE_SOURCES})

file(GLOB BINDING_CP2K_SOURCES
    "./cp2k/*.cpp"
    "../src/atomsciflow/cp2k/*.cpp"
)
add_library(cp2k MODULE ${BINDING_CP2K_SOURCES})
target_link_libraries(cp2k
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
    atomsciflow-base
    atomsciflow-variable 
    atomsciflow-server
    atomsciflow-remote
    atomsciflow-config
    atomsciflow-utils
)
set_target_properties(cp2k PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(cp2k MODULE ${BINDING_CP2K_SOURCES})

file(GLOB BINDING_QE_SOURCES
    "./qe/*.cpp"
    "../src/atomsciflow/qe/*.cpp"
)
add_library(qe MODULE ${BINDING_QE_SOURCES})
target_link_libraries(qe
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
    atomsciflow-base
    atomsciflow-variable 
    atomsciflow-server
    atomsciflow-remote
    atomsciflow-config
    atomsciflow-utils
)
set_target_properties(qe PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(qe MODULE ${BINDING_QE_SOURCES})

file(GLOB BINDING_ABINIT_SOURCES
    "./abinit/abinit.cpp"
    "../src/atomsciflow/abinit/*.cpp"
)
add_library(abinit MODULE ${BINDING_ABINIT_SOURCES})
target_link_libraries(abinit
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
    atomsciflow-base
    atomsciflow-variable 
    atomsciflow-server
    atomsciflow-remote
    atomsciflow-config
    atomsciflow-utils
)
set_target_properties(abinit PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(abinit MODULE ${BINDING_ABINIT_SOURCES})

file(GLOB BINDING_PARSER_SOURCES
    "./parser/parser.cpp"
    "../src/atomsciflow/parser/*.cpp"
    #"../src/atomsciflow/base/*.cpp"
)
#pybind11_add_module(parser MODULE ${BINDING_PARSER_SOURCES})
add_library(parser MODULE ${BINDING_PARSER_SOURCES})
target_link_libraries(parser
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    atomsciflow-base
)
set_target_properties(parser PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(parser MODULE ${BINDING_PARSER_SOURCES})

file(GLOB BINDING_VARIABLE_SOURCES
    "./variable/variable.cpp"
    "../src/atomsciflow/variable/*.cpp"
)
pybind11_add_module(variable MODULE ${BINDING_VARIABLE_SOURCES})

file(GLOB BINDING_VASP_SOURCES
    "./vasp/vasp.cpp"
    "../src/atomsciflow/vasp/*.cpp"
    "../src/atomsciflow/vasp/post/*.cpp"
)
#add_library(parser MODULE ${BINDING_VASP_SOURCES})
add_library(vasp MODULE ${BINDING_VASP_SOURCES})
target_link_libraries(vasp
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
    atomsciflow-base
    atomsciflow-variable 
    atomsciflow-server
    atomsciflow-remote
    atomsciflow-config
    atomsciflow-utils
)
set_target_properties(vasp PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(vasp MODULE ${BINDING_VASP_SOURCES})

file(GLOB BINDING_CMD_SOURCES
    "./cmd/cmd.cpp"
    "../src/atomsciflow/cmd/*.cpp"
)
pybind11_add_module(cmd MODULE ${BINDING_CMD_SOURCES})

file(GLOB BINDING_CONFIG_SOURCES
    "./config/*.cpp"
)
#pybind11_add_module(config MODULE ${BINDING_CONFIG_SOURCES})
add_library(config MODULE ${BINDING_CONFIG_SOURCES})
target_link_libraries(config
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    atomsciflow-config
)
set_target_properties(config PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(config MODULE ${BINDING_CONFIG_SOURCES})

file(GLOB BINDING_GAMESSUS_SOURCES
    "./gamessus/gamessus.cpp"
    "../src/atomsciflow/gamessus/*.cpp"
)
#add_library(gamessus MODULE ${BINDING_GAMESSUS_SOURCES})
add_library(gamessus MODULE ${BINDING_GAMESSUS_SOURCES})
target_link_libraries(gamessus
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
    atomsciflow-base
    atomsciflow-variable 
    atomsciflow-server
    atomsciflow-remote
    atomsciflow-config
    atomsciflow-utils
)
set_target_properties(gamessus PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(gamessus MODULE ${BINDING_GAMESSUS_SOURCES})

file(GLOB BINDING_ELK_SOURCES
    "./elk/elk.cpp"
    "../src/atomsciflow/elk/*.cpp"
)
#add_library(elk MODULE ${BINDING_ELK_SOURCES})
add_library(elk MODULE ${BINDING_ELK_SOURCES})
target_link_libraries(elk
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
    atomsciflow-base
    atomsciflow-variable 
    atomsciflow-server
    atomsciflow-remote
    atomsciflow-config
    atomsciflow-utils
)
set_target_properties(elk PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(elk MODULE ${BINDING_ELK_SOURCES})

file(GLOB BINDING_QMCPACK_SOURCES
    "./qmcpack/qmcpack.cpp"
    "../src/atomsciflow/qmcpack/*.cpp"
)
#add_library(qmcpack MODULE ${BINDING_QMCPACK_SOURCES})
add_library(qmcpack MODULE ${BINDING_QMCPACK_SOURCES})
target_link_libraries(qmcpack
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
    atomsciflow-base
    atomsciflow-variable 
    atomsciflow-server
    atomsciflow-remote
    atomsciflow-config
    atomsciflow-utils
)
set_target_properties(qmcpack PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(qmcpack MODULE ${BINDING_QMCPACK_SOURCES})

file(GLOB BINDING_DALTON_SOURCES
    "./dalton/dalton.cpp"
    "../src/atomsciflow/dalton/*.cpp"
)
#add_library(dalton MODULE ${BINDING_DALTON_SOURCES})
add_library(dalton MODULE ${BINDING_DALTON_SOURCES})
target_link_libraries(dalton
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
    atomsciflow-base
    atomsciflow-variable 
    atomsciflow-server
    atomsciflow-remote
    atomsciflow-config
    atomsciflow-utils
)
set_target_properties(dalton PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(dalton MODULE ${BINDING_DALTON_SOURCES})

file(GLOB BINDING_SIESTA_SOURCES
    "./siesta/siesta.cpp"
    "../src/atomsciflow/siesta/*.cpp"
)
#add_library(siesta MODULE ${BINDING_SIESTA_SOURCES})
add_library(siesta MODULE ${BINDING_SIESTA_SOURCES})
target_link_libraries(siesta
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
    atomsciflow-base
    atomsciflow-variable 
    atomsciflow-server
    atomsciflow-remote
    atomsciflow-config
    atomsciflow-utils
)
set_target_properties(siesta PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(siesta MODULE ${BINDING_SIESTA_SOURCES})

file(GLOB BINDING_DFTBPLUS_SOURCES
    "./dftbplus/dftbplus.cpp"
    "../src/atomsciflow/dftbplus/*.cpp"
)
#add_library(dftbplus MODULE ${BINDING_DFTBPLUS_SOURCES})
add_library(dftbplus MODULE ${BINDING_DFTBPLUS_SOURCES})
target_link_libraries(dftbplus
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
    atomsciflow-base
    atomsciflow-variable 
    atomsciflow-server
    atomsciflow-remote
    atomsciflow-config
    atomsciflow-utils
)
set_target_properties(dftbplus PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(dftbplus MODULE ${BINDING_DFTBPLUS_SOURCES})

file(GLOB BINDING_NWCHEM_SOURCES
    "./nwchem/nwchem.cpp"
    "../src/atomsciflow/nwchem/*.cpp"
)
#add_library(nwchem MODULE ${BINDING_NWCHEM_SOURCES})
add_library(nwchem MODULE ${BINDING_NWCHEM_SOURCES})
target_link_libraries(nwchem
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
    atomsciflow-base
    atomsciflow-variable 
    atomsciflow-server
    atomsciflow-remote
    atomsciflow-config
    atomsciflow-utils
)
set_target_properties(nwchem PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(nwchem MODULE ${BINDING_NWCHEM_SOURCES})

file(GLOB BINDING_EXCITING_SOURCES
    "./exciting/exciting.cpp"
    "../src/atomsciflow/exciting/*.cpp"
    "../src/atomsciflow/qmcpack/input.cpp" # needed by exciting
)
#add_library(exciting MODULE ${BINDING_EXCITING_SOURCES})
add_library(exciting MODULE ${BINDING_EXCITING_SOURCES})
target_link_libraries(exciting
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
    atomsciflow-base
    atomsciflow-variable 
    atomsciflow-server
    atomsciflow-remote
    atomsciflow-config
    atomsciflow-utils
)
set_target_properties(exciting PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(exciting MODULE ${BINDING_EXCITING_SOURCES})

file(GLOB BINDING_GAUSSIAN_SOURCES
    "./gaussian/gaussian.cpp"
    "../src/atomsciflow/gaussian/*.cpp"
)
#add_library(gaussian MODULE ${BINDING_GAUSSIAN_SOURCES})
add_library(gaussian MODULE ${BINDING_GAUSSIAN_SOURCES})
target_link_libraries(gaussian
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
    atomsciflow-base
    atomsciflow-variable 
    atomsciflow-server
    atomsciflow-remote
    atomsciflow-config
    atomsciflow-utils
)
set_target_properties(gaussian PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(gaussian MODULE ${BINDING_GAUSSIAN_SOURCES})

file(GLOB BINDING_GROMACS_SOURCES
    "./gromacs/gromacs.cpp"
    "../src/atomsciflow/gromacs/*.cpp"
)
#add_library(gromacs MODULE ${BINDING_GROMACS_SOURCES})
add_library(gromacs MODULE ${BINDING_GROMACS_SOURCES})
target_link_libraries(gromacs
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
    atomsciflow-base
    atomsciflow-variable 
    atomsciflow-server
    atomsciflow-remote
    atomsciflow-config
    atomsciflow-utils
)
set_target_properties(gromacs PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(gromacx MODULE ${BINDING_GROMACS_SOURCES})

file(GLOB BINDING_LAMMPS_SOURCES
    "./lammps/lammps.cpp"
    "../src/atomsciflow/lammps/*.cpp"
)
#add_library(lammps MODULE ${BINDING_LAMMPS_SOURCES})
add_library(lammps MODULE ${BINDING_LAMMPS_SOURCES})
target_link_libraries(lammps
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
    atomsciflow-base
    atomsciflow-variable 
    atomsciflow-server
    atomsciflow-remote
    atomsciflow-config
    atomsciflow-utils
)
set_target_properties(lammps PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(lammps MODULE ${BINDING_LAMMPS_SOURCES})

file(GLOB BINDING_OCTOPUS_SOURCES
    "./octopus/octopus.cpp"
    "../src/atomsciflow/octopus/*.cpp"
)
#add_library(octopus MODULE ${BINDING_OCTOPUS_SOURCES})
add_library(octopus MODULE ${BINDING_OCTOPUS_SOURCES})
target_link_libraries(octopus
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
    atomsciflow-base
    atomsciflow-variable 
    atomsciflow-server
    atomsciflow-remote
    atomsciflow-config
    atomsciflow-utils
)
set_target_properties(octopus PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(octopus MODULE ${BINDING_OCTOPUS_SOURCES})

file(GLOB BINDING_ORCA_SOURCES
    "./orca/orca.cpp"
    "../src/atomsciflow/orca/*.cpp"
)
#add_library(orca MODULE ${BINDING_ORCA_SOURCES})
add_library(orca MODULE ${BINDING_ORCA_SOURCES})
target_link_libraries(orca
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
    atomsciflow-base
    atomsciflow-variable 
    atomsciflow-server
    atomsciflow-remote
    atomsciflow-config
    atomsciflow-utils
)
set_target_properties(orca PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(orca MODULE ${BINDING_ORCA_SOURCES})

file(GLOB BINDING_QCHEM_SOURCES
    "./qchem/qchem.cpp"
    "../src/atomsciflow/qchem/*.cpp"
)
#add_library(qchem MODULE ${BINDING_QCHEM_SOURCES})
add_library(qchem MODULE ${BINDING_QCHEM_SOURCES})
target_link_libraries(qchem
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
    atomsciflow-base
    atomsciflow-variable 
    atomsciflow-server
    atomsciflow-remote
    atomsciflow-config
    atomsciflow-utils
)
set_target_properties(qchem PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(qchem MODULE ${BINDING_QCHEM_SOURCES})

file(GLOB BINDING_XTB_SOURCES
    "./xtb/xtb.cpp"
    "../src/atomsciflow/xtb/*.cpp"
)
#add_library(xtb MODULE ${BINDING_XTB_SOURCES})
add_library(xtb MODULE ${BINDING_XTB_SOURCES})
target_link_libraries(xtb
    PRIVATE pybind11::module
    ${Boost_LIBRARIES}
    ${YAML_CPP_LIBRARIES}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${LIBSSH2_FOUND_LIBS}
    atomsciflow-base
    atomsciflow-variable 
    atomsciflow-server
    atomsciflow-remote
    atomsciflow-config
    atomsciflow-utils
)
set_target_properties(xtb PROPERTIES 
    PREFIX "${PYTHON_MODULE_PREFIX}"
    SUFFIX "${PYTHON_MODULE_EXTENSION}"
)
#pybind11_add_module(xtb MODULE ${BINDING_XTB_SOURCES})

file(GLOB BINDING_SERVER_SOURCES
    "./server/*.cpp"
    "../src/atomsciflow/server/*.cpp"
    "../src/atomsciflow/remote/*.cpp"
    "../src/atomsciflow/config/*.cpp"
)
#pybind11_add_module(server MODULE ${BINDING_SERVER_SOURCES})
add_library(server MODULE ${BINDING_SERVER_SOURCES})
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
#pybind11_add_module(server  MODULE ${BINDING_SERVER_SOURCES})

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
    install(TARGETS dalton DESTINATION .)
    install(TARGETS siesta DESTINATION .)
    install(TARGETS dftbplus DESTINATION .)
    install(TARGETS nwchem DESTINATION .)
    install(TARGETS exciting DESTINATION .)
    install(TARGETS gaussian DESTINATION .)
    install(TARGETS gromacs DESTINATION .)
    install(TARGETS lammps DESTINATION .)
    install(TARGETS octopus DESTINATION .)
    install(TARGETS orca DESTINATION .)
    install(TARGETS qchem DESTINATION .)
    install(TARGETS xtb DESTINATION .)
    install(TARGETS server DESTINATION .)
endif()


