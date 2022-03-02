add_executable(atomsciflow-version.x
    "./atomsciflow_version.cpp"
    "./cmd_utils.cpp"
)
target_link_libraries(atomsciflow-version.x
    atomsciflow
    ${Boost_LIBRARIES}
)

add_executable(atomsciflow.x 
    "./atomsciflow.cpp"
    "./cmd_utils.cpp"
)
target_link_libraries(atomsciflow.x
    atomsciflow
    ${Boost_LIBRARIES}
)


add_executable(atomsciflow-traj.x
    "./atomsciflow_traj.cpp"
    "./cmd_utils.cpp"
)
target_link_libraries(atomsciflow-traj.x
    atomsciflow
    ${Boost_LIBRARIES}
)

add_executable(atomsciflow-cube.x
    "./atomsciflow_cube.cpp"
    "./cmd_utils.cpp"
)
target_link_libraries(atomsciflow-cube.x
    atomsciflow
    ${Boost_LIBRARIES}
)

add_executable(atomsciflow-calc.x
    "./atomsciflow_calc.cpp"
    "./atomsciflow_calc_abinit.cpp"
    "./atomsciflow_calc_cp2k.cpp"
    "./atomsciflow_calc_qe.cpp"
    "./cmd_utils.cpp"
)
target_link_libraries(atomsciflow-calc.x
    atomsciflow
    ${Boost_LIBRARIES}
    #-lstdc++fs
)

add_executable(atomsciflow-ssh.x
    "./atomsciflow_ssh.cpp"
    "./cmd_utils.cpp"
)
target_link_libraries(atomsciflow-ssh.x
    atomsciflow
    ${Boost_LIBRARIES}
)

add_executable(atomsciflow-yaml.x
    "./atomsciflow_yaml.cpp"
    "./cmd_utils.cpp"
)
target_link_libraries(atomsciflow-yaml.x
    atomsciflow
    ${Boost_LIBRARIES}
)


# main c++ test
add_executable(atomsciflow-main-test.x
    "../test/gtest/main.cpp"
    "./cmd_utils.cpp"
    ${PROJECT_TEST_SOURCES}
)
target_link_libraries(atomsciflow-main-test.x
    atomsciflow
    GTest::gtest
    GTest::gtest_main
)

install(
    TARGETS 
    atomsciflow-version.x 
    atomsciflow.x 
    atomsciflow-traj.x 
    atomsciflow-cube.x 
    atomsciflow-calc.x 
    atomsciflow-ssh.x
    atomsciflow-main-test.x
    atomsciflow-yaml.x
    EXPORT atomsciflow
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

