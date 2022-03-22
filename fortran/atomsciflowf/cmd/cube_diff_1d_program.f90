!> The entry for program doing plane averaged 1D charge difference density
!! Usage:
!! asflowf-cube-diff-1d.x CUBEFILE_1 CUBE_FILE_2 CUBE_FILE_3
program cube_diff_1d_program
    use asflowf_cube_diff_1d, only : cube_diff_1d
    use cmd_utils, only : log_cmd_start

    implicit none
    
    character(len=128), dimension(3) :: cube_file_in_iii

    ! command line output 
    call log_cmd_start("asflowf-cube-diff-1d.x")

    ! read cube file specified by the first command argument
    call get_command_argument(1, cube_file_in_iii(1))
    call get_command_argument(2, cube_file_in_iii(2))
    call get_command_argument(3, cube_file_in_iii(3))

    !
    call cube_diff_1d(cube_file_in_iii)

    stop
end program cube_diff_1d_program

