cmake_minimum_required(VERSION 3.20)

project(atomsciflow-pybind11 LANGUAGES CXX)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC ")

find_package(Boost COMPONENTS program_options filesystem system REQUIRED)
include_directories( ${Boost_INCLUDE_DIR}  )

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

file(GLOB BASE_SOURCES
    "./base/*.cpp"
    "../src/atomsciflow/base/*.cpp"
)
pybind11_add_module(base MODULE ${BASE_SOURCES})
#add_library(base MODULE ${BASE_SOURCES})
#target_link_libraries(base PRIVATE pybind11::module)
#set_target_properties(base PROPERTIES 
#    PREFIX "${PYTHON_MODULE_PREFIX}"
#    SUFFIX "${PYTHON_MODULE_EXTENSION}"
#)
#pybind11_add_module(base MODULE ${BASE_SOURCES})

file(GLOB CP2K_SOURCES
    "./cp2k/*.cpp"
    "../src/atomsciflow/variable/*.cpp"
    "../src/atomsciflow/base/*.cpp"
    "../src/atomsciflow/cp2k/*.cpp" 
)
pybind11_add_module(cp2k MODULE ${CP2K_SOURCES})

file(GLOB QE_SOURCES
    "./qe/*.cpp"
    "../src/atomsciflow/variable/*.cpp"
    "../src/atomsciflow/base/*.cpp"
    "../src/atomsciflow/qe/*.cpp" 
)
pybind11_add_module(qe MODULE ${QE_SOURCES})

file(GLOB ABINIT_SOURCES
    "./abinit/abinit.cpp"
    "../src/atomsciflow/variable/*.cpp"
    "../src/atomsciflow/base/*.cpp"
    "../src/atomsciflow/abinit/*.cpp"
)
pybind11_add_module(abinit MODULE ${ABINIT_SOURCES})

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
endif()
