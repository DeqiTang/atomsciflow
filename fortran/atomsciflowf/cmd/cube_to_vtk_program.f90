!> Usage:
!! asflow-cube-to-vtk.x CUBEFILE VTKFILE
program cube_to_vtk_program
    use asflowf_cube_to_vtk, only : cube_to_vtk
    use cmd_utils, only : log_cmd_start

    implicit none

    character(len=128) :: cube_file_in, vtk_file_out

    call log_cmd_start("asflow-cube-to-vtk.x")

    ! read cube file specified by the first command argument
    call get_command_argument(1, cube_file_in)
    call get_command_argument(2, vtk_file_out)
    ! 
    call cube_to_vtk(cube_file_in, vtk_file_out)

    stop
end program cube_to_vtk_program
