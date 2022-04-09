add_executable(atomsciflow-version.x
    "./atomsciflow_version.cpp"
)
target_link_libraries(atomsciflow-version.x
    atomsciflow
    ${Boost_LIBRARIES}
)

add_executable(atomsciflow.x 
    "./atomsciflow.cpp"
)
target_link_libraries(atomsciflow.x
    atomsciflow
    ${Boost_LIBRARIES}
)


add_executable(atomsciflow-traj.x
    "./atomsciflow_traj.cpp"
)
target_link_libraries(atomsciflow-traj.x
    atomsciflow
    ${Boost_LIBRARIES}
)

add_executable(atomsciflow-cube.x
    "./atomsciflow_cube.cpp"
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
    "./atomsciflow_calc_vasp.cpp"
    "./atomsciflow_calc_gamessus.cpp"
    "./atomsciflow_calc_elk.cpp"
    "./atomsciflow_calc_qmcpack.cpp"
)
target_link_libraries(atomsciflow-calc.x
    atomsciflow
    ${Boost_LIBRARIES}
    #-lstdc++fs
)

add_executable(atomsciflow-post.x
    "./atomsciflow_post.cpp"
    "./atomsciflow_post_vasp.cpp"
)
target_link_libraries(atomsciflow-post.x
    atomsciflow
    ${Boost_LIBRARIES}
    #-lstdc++fs
)

add_executable(atomsciflow-ssh.x
    "./atomsciflow_ssh.cpp"
)
target_link_libraries(atomsciflow-ssh.x
    atomsciflow
    ${Boost_LIBRARIES}
)

add_executable(atomsciflow-yaml.x
    "./atomsciflow_yaml.cpp"
)
target_link_libraries(atomsciflow-yaml.x
    atomsciflow
    ${Boost_LIBRARIES}
)

install(
    TARGETS 
    atomsciflow-version.x 
    atomsciflow.x 
    atomsciflow-traj.x 
    atomsciflow-cube.x 
    atomsciflow-calc.x 
    atomsciflow-post.x 
    atomsciflow-ssh.x
    atomsciflow-yaml.x
    EXPORT atomsciflow
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

